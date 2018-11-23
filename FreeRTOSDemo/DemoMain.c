// Directivas del precompilador
#include <p33FJ128MC802.h>
#include <stdint.h> // define los tipos uint8\_t, ...
#include <stdio.h>  // Para usar sprintf
#include <xc.h> // Para usar el Low Voltage Sleep

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

#define PIN_ADC_1 0
#define PIN_ADC_2 1
#define FRECUENCIA 50
#define PIN_PWM (1<<15)//PIN RB15
//UART 
#define BAUD_RATE 115200
#define TAM_TR_UART 250
#define TAM_REC_UART 250
#define PR_INT_TX 5

// Márgenes, umbrales y constantes del control
#define REF 0
#define NUM_MEDIDAS 16
#define UMBRAL_DORMIR 50
#define UMBRAL_REGPERM 100 //ms
#define MARGEN_REFPERM 10 // Luxes
#define MANDO_MAX 1050
#define MANDO_MIN 150

#define TS_CONTROL 0.001




// Prioridad de las tareas
//#define BUCLE_SCAN_PRIO (tskIDLE_PRIORITY + 1)
#define PRIO_CONTROL (tskIDLE_PRIORITY + 1)



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
 * microchipdeveloper.com/16bit:doze-idle-sleep

Preguntas para Miguel:
¿Porque luxes1 y luxes2 son globales?

1) Si el micro se despierta por el T1, entramos en un bucle infinito...
2) Como establecer las prioridades de las tareas? No queremos que se ejecute el control una vez se ha estabilizado,
   sino volver a BucleScan y de ahí al modo Sleep();
 */


/* Semáforos para sincronizar las tareas */
//static SemaphoreHandle_t sem_TomaDatos; // Semáforo para iniciar la lectura de los ADC
static SemaphoreHandle_t sem_TareaControl; //Para iniciar el cálculo del control una
//vez se han leído los ADC

// Prototipos funciones propias
void vApplicationIdleHook(void); //En esta función se va a poner el código para 
void InitTimer2(void);
void InitHardware(void);

void AplicarControl(void *pvParameters);

//Interrupciones
/*
* INT0
* Interrupción para despertar al sistema del estado de reposo/sleep
*/
void __attribute__((interrupt(no_auto_psv) )) _INT0Interrupt(void){
      
    //Despierta el sistema (esto lo hace tanto esta interrupción como la del Timer 1 (el del sistema operativo)))
    
    //Activación de interrupciones de timer2
    IEC0bits.T2IE = 1;
    
    IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag     
    return;
}

/*
* T2
* Interrupción cada tick del timer 2
* Permite hacer el control digital y timeout para poner en reposo al sistema
*/
void __attribute__((interrupt(no_auto_psv) )) _T2Interrupt(void){
    
    BaseType_t xTaskWoken = pdFALSE;
    
    //Dar semáforo a TareaControl
    xSemaphoreGiveFromISR(sem_TareaControl, &xTaskWoken);

    if(xTaskWoken == pdTRUE){
        taskYIELD (); /* Si el semÅforo ha despertado
                        una tarea, se fuerza un cambio
                        de contexto */
    }
    
    IFS0bits.T2IF = 0;    //Clear the Timer 2 interrupt flag     
    return;
}
 


/* Programa principal */
int main(void)
{
    //Inicializar Hardware
    InitHardware();

    // Inicializaciones
    vSemaphoreCreateBinary(sem_TareaControl);
 

    // Se crean las tareas
    /*xTaskCreate(BucleScan,
    (const signed portCHAR * const) "BucScan",
              BUCLE_SCAN_STACK_SIZE, NULL, BUCLE_SCAN_PRIO,
              (xTaskHandle * ) NULL ); // No entiendo esta tarea
    */
    xTaskCreate(AplicarControl, "AplCtrl", TAM_PILA, NULL,
                PRIO_CONTROL, NULL);
     

    vTaskStartScheduler(); 
                           
    return 0; 
}
/*

void BucleScan(void *pvParameters){ // Tarea principal
    // Declaracion de variables

    while(1){

        if (ContadorTimeoutSleep==UMBRAL_DORMIR){ 
            ContadorTimeoutSleep = 0;

            //Poner el micro en reposo
            vApplicationIdleHook();
        }

    
    }

    return; // Nunca se llega aquí
}

*/
void AplicarControl(void *pvParameters){ // Aplica control PI hasta alcanzar regimen permanente
    // Definición de variables
    static int cont;
    static unsigned int luxes1;
    static unsigned int luxes2;
    int lectura;
    float prop, mando;
    float integ = 0;
    float kprop = 0.8; //kprop = 0.8;
    float kinteg = 400 ; //kinteg = 611.16;
    float lectura_robusta = 0;
    
    //For debugging
    char str[80];
    int contador = 0;

    while(1){
        if(xSemaphoreTake(sem_TareaControl, (TickType_t) 1000)== pdTRUE ){
            // Lectura de medidas - esta tarea la incluimos dentro de la tarea de Control 
            //                      ya que no tiene un delay demasiado alto
            Disable();
            luxes2 = 2*LeerLuxes(PIN_ADC_2);
            luxes1 = LeerLuxes(PIN_ADC_1);
            Enable();

            // Calculamos la entrada para el control (diferencia de luxes y aplicamos control)
            lectura = luxes1-luxes2;
           
            
            // La referencia es 0, por lo que el control debe conseguir que dicha lectura se haga 
            // en las siguientes iteraciones. La propia lectura se considera el error.
            prop = kprop * (float)lectura;
            integ += (float)lectura * TS_CONTROL; 

            // Cálculo de mando
            mando = prop + kinteg * integ;
            
            if(contador == 100){
                sprintf(str,"Luxes 1 , 2: %d, %d Mando: %d;\n", luxes1,luxes2,mando);
                putsUART(str);
                contador = 0;
            }
            // Anti-Windup
            if (mando > MANDO_MAX) { 
                mando = MANDO_MAX;
                integ -= (float)lectura * TS_CONTROL;

            }else if (mando < MANDO_MIN) {
                mando = MANDO_MIN;
                integ -= (float)lectura * TS_CONTROL;
            }

            // Actuación sobre motor
            setDcPWM(PIN_PWM, mando);


            // Comprobar si la tarea de control ha finalizado HAY QUE MEJORAR ESTO PARA ESTABILIZARLO
            if (lectura < REF + MARGEN_REFPERM && lectura > REF - MARGEN_REFPERM){
                cont++;
                if (cont == UMBRAL_REGPERM){
                    cont = 0;
                    integ = 0;
                    

                }

            }else{
                cont = 0;
            }
            
            contador ++; //debug
        }
    }
    
        
}


void InitTimer2(void){
    T2CONbits.TON = 0; // Disable Timer
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b11; // Select 1:8 Prescaler
    T2CONbits.T32 = 0; // Disable 32-bit Timer mode
    TMR2 = 0x00; // Clear timer register
    PR2 =5000*1000*TS_CONTROL;// 1 ms con TS_CONTROL=0.001
    IPC1bits.T2IP = 0x02; // Set Timer 1 Interrupt Priority Level(priority less than timer 1 )
    IFS0bits.T2IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T2IE = 1; // Disable Timer1 interrupt
    T2CONbits.TON = 1; // Start Timer

    return;
}

/*
vApplicationIdleHook
activa el modo sleep del micro. Este dormirá hasta que salte la interrupcion INT0.
para permanecer dormido, es importante desactivar las interrupciones del timer2.
*/
void vApplicationIdleHook(void){
    //Apagar Interrupciones 
    IEC0bits.T2IE = 0;

    // Activamos el modo sueño
    Sleep();
}

void InitHardware(void){
    inicializarADCPolling(1<<PIN_ADC_1 | 1<<PIN_ADC_2);
    InicializarReloj();
    inicializarPWM(PIN_PWM, FRECUENCIA);
    activarPWM(PIN_PWM);
    setFrecuencia(FRECUENCIA);
    inicializarUART(BAUD_RATE);
    InitTimer2();

    return;
}



