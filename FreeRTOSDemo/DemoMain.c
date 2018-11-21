#include <p33FJ128MC802.h>
#include <stdint.h> // define los tipos uint8\_t, ...
#include <stdio.h>  // Para usar sprintf

#include "DriverPicTrainer/config.h"
#include "DriverPicTrainer/adc.h"
#include "DriverPicTrainer/pwm.h"
#include "DriverPicTrainer/pwm2.h"
#include "DriverPicTrainer/uart.h"

#include "Interrupciones.h" // Enable y Disable
#include "Serie.h"          // Módulos del sistema
#include "Entradas.h"

#include "sensor.h"

#include "FreeRTOS.h" // Includes del Kernel
#include "semphr.h"
#include "queue.h"
#include "task.h"

#define PRIO_SERIE 3
#define PRIO_IMP_HORA 2
#define PRIO_ENV_ENTR 1
#define TAM_PILA_SERIE 250 // Esta tarea necesita 100 bytes adicionales para una cadena
#define TAM_PILA 150

/* SemÅforos para sincronizar las tareas */
static SemaphoreHandle_t sem_TomaDatos; // Semáforo para iniciar la lectura de los ADC
static SemaphoreHandle_t sem_AplicarControl; //Para iniciar el cálculo del control una
//vez se han leído los ADC


//Variables globales
 static   unsigned int luxes1;
 static   unsigned int luxes2;

#define PIN_ADC_1 0
#define PIN_ADC_2 1
#define FRECUENCIA 50
#define PIN_PWM (1<<15)//PIN RB15
#define BAUD_RATE 115200
#define TAM_TR_UART 250
#define TAM_REC_UART 250
#define PR_INT_TX 5
 
#define Enable() SET_CPU_IPL(0)
#define Disable() SET_CPU_IPL(7)
 
#define REF 0
#define UMBRAL_DORMIR 100
#define UMBRAL_REGPERM 10
#define MARGEN_REFPERM 5 // Luxes

// Prototipos de funciones
void __attribute__((__interrupt__,no_auto_psv)) _INT0Interrupt(void);
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
void vApplicationIdleHook( void ); //En esta función se va a poner el código para 
//mandar al micro a dormir

//Interrupciones
void __attribute__((__interrupt__,no_auto_psv)) _INT0Interrupt(void){
      
    //Despierta el sistema (esto lo hace tanto esta interrupción  como la del Timer 1 (el del sistema operativo)))
    
    //Inicialización Timer 2
    T2CONbits.TON = 0; // Disable Timer
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b11; // Select 1:8 Prescaler
    T2CONbits.T32 = 0; // Disable 32-bit Timer mode
    TMR2 = 0x00; // Clear timer register
    PR2 =5000;//For 1 ms 
    IPC1bits.T2IP = 0x02; // Set Timer 1 Interrupt Priority Level(priority less than timer 1 )
    IFS0bits.T2IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T2IE = 1; // Enable Timer1 interrupt
    T2CONbits.TON = 1; // Start Timer
    
    
    IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag     
    return;
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void){

    //Dar semáforo a Leer Datos
    xSemaphoreGive(sem_TomaDatos);
            
    IFS0bits.T2IF = 0;    //Clear the Timer 2 interrupt flag     
    return;
}
 


/* ESTRUCTURA DEL TASK SCHEDULER */
/*
 * Tenemos principalmente 2 tareas pricipales: BucleScan y AplicarControl
 *
 * BucleScan con la prioridad más baja. Pone a dormir el micro tras alacanzar el timeout especificado para el timer
 * Inicio con semaforo
 *
 * La forma de salir de esta tarea es mediante la interrupción (proporcionada por hardware) ante un cambio en
 * de iluminosidad. Asignamos una ISR a esta interrupción, que se encargará de:
 *      - Deshabilitar la flag de interrupción.
 *      - Meter dato en cola, para que comience la tarea del control. 
 *
 * La tarea de control, según se ejecuta (antes del while), deshabilita la interrupción para no volver a meterse mientras se realiza el control.
 * Al final de la tarea (cuando se alcanza timeout del régimen permanente), se vuele al bucle scan.
 *
 * El control vuelve al bucle scan ...¿como?
 *
 *
 *
 *Mode PRO
 * 
 * Tenemos una interrupción por hardware que enciende el sistema
 * 
 * Ahí arrancamos el Timer 2
 * 
 * Las interrupciones del timer 2 sueltan un semáforo para que la tarea de Toma
 *  de datos pueda ejecutarse
 *
 *La tarea de Toma de datos lee los ADC y libera otro semáforo que permite que el control pueda ejecutarse. 
 * Además, libera el semáforo que la une con la interrupción del timer.
 *
 *La tarea control ejecuta el control y cambia el PWM. Cuando lo hace libera el semáforo que
 * la había habilitado.
 * 
 * De alguna manera, contamos un time out y apagamos el sistema 
 */



int main(void)
{
    //Incializar Hardware
    inicializarADCPolling(1<<PIN_ADC_1 | 1<<PIN_ADC_2);
    InicializarReloj();
    inicializarPWM(PIN_PWM, FRECUENCIA);
    activarPWM(PIN_PWM);
    setFrecuencia(FRECUENCIA);
    inicializarUART(BAUD_RATE);

    // Se inicializan los semáforos
    vSemaphoreCreateBinary(sem_TomaDatos);
    vSemaphoreCreateBinary(sem_AplicarControl);
 
    // Se crean las tareas
    xTaskCreate(BucleScan,
    (const signed portCHAR * const) "BucScan",
              BUCLE_SCAN_STACK_SIZE, NULL, BUCLE_SCAN_PRIO,
              (xTaskHandle * ) NULL ); // No entiendo esta tarea

    xTaskCreate(AplicarControl, "AplCtrl", TAM_PILA, NULL,
                PRIO_ENV_ENTR, NULL);
    
    xTaskCreate(LeerDatos, "LData", TAM_PILA, NULL,
                PRIO_ENV_ENTR, NULL);
     

    vTaskStartScheduler(); // y por último se arranca el
                           // planificador.
    return 0; // En teoría no se debe llegar nunca aquí.
}

void vApplicationIdleHook( void ){
    //Apagar Timer 2 
    T2CONbits.TON = 0; 
    //sleep();????
}


void BucleScan(void){ // Tarea principal
    // Declaracion de variables

    while(1){

        if (cont_controlado==UMBRAL_DORMIR){ 
            cont_controlado = 0;

            //Poner el micro en reposo

        }

    }

    return; // Nunca se llega aquí
}

void LeerDatos(void){ // Aplica control PI hasta alcanzar regimen permanente
    //Definición de variables
    
    if(xSemaphoreTake(sem_TomaDatos, (TickType_t) 1000 )
    == pdTRUE ){
    
        Disable();
        luxes2 = 2*LeerLuxes(PIN_ADC_2);
        luxes1 = LeerLuxes(PIN_ADC_1);
        Enable();

         xSemaphoreGive(sem_AplicarControl);
    }
}


void AplicarControl(void){ // Aplica control PI hasta alcanzar regimen permanente
    
    if(xSemaphoreTake(sem_AplicarControl, (TickType_t) 1000 )
    == pdTRUE ){
    
        //Definición de variables
        int cont;
        int lectura;

        cont_controlado=0;
/*
        while(1){
            // Lectura - protegida
            Disable();

            Enable();
*/
            // Control

        
        
            // Regimen permanente
            if (lectura < REF + MARGEN_REFPERM && lectura > REF - MARGEN_REFPERM){
                cont++;
                if (cont == UMBRAL_REGPERM){
                    // Volver a la tarea 
                }

            }else{
                cont = 0;
            }


        }
        
        
        
    }
}

