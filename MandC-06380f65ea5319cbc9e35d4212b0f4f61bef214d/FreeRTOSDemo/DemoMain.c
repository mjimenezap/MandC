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
#include "Serie.h"          // MÃ³dulos del sistema
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
#define PIN_ADC_5 4
#define PIN_ADC_6 5

#define FRECUENCIA 50
#define PIN_PWM1 (1<<15)//PIN RB15
#define PIN_PWM2 (1<<13)//PIN RB10
//UART 
#define BAUD_RATE 115200
#define TAM_TR_UART 250
#define TAM_REC_UART 250
#define PR_INT_TX 5

// MÃ¡rgenes, umbrales y constantes del control
#define REF 0
#define NUM_MEDIDAS 16
#define UMBRAL_DORMIR 50
#define UMBRAL_REGPERM 500 //ms
#define MARGEN_REFPERM 100 // Luxes  //10
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
 * BucleScan con la prioridad mÃ¡s baja. Pone a dormir el micro tras alacanzar el timeout especificado para el timer
 * Inicio con semaforo
 *
 * La forma de salir de esta tarea es mediante la interrupciÃ³n (prop1orcionada por hardware) ante un cambio en
 * de iluminosidad. Asignamos una ISR a esta interrupciÃ³n, que se encargarÃ¡ de:
 *      - Deshabilitar la flag de interrupciÃ³n.
 *      - Meter dato en cola, para que comience la tarea del control. 
 *
 * La tarea de control, segÃºn se ejecuta (antes del while), deshabilita la interrupciÃ³n para no volver a meterse mientras se realiza el control.
 * Al final de la tarea (cuando se alcanza timeout del rÃ©gimen permanente), se vuele al bucle scan.
 *
 * El control vuelve al bucle scan ...Â¿como?
 *
 *
 *
 *Mode PRO
 * 
 * Tenemos una interrupciÃ³n por hardware que enciende el sistema
 * 
 * AhÃ­ arrancamos el Timer 2
 * 
 * Las interrupciones del timer 2 sueltan un semÃ¡foro para que la tarea de Toma
 *  de datos pueda ejecutarse
 *
 *La tarea de Toma de datos lee los ADC y libera otro semÃ¡foro que permite que el control pueda ejecutarse. 
 * AdemÃ¡s, libera el semÃ¡foro que la une con la interrupciÃ³n del timer.
 *
 *La tarea control ejecuta el control y cambia el PWM. Cuando lo hace libera el semÃ¡foro que
 * la habÃ­a habilitado.
 * 
 * De alguna manera, contamos un time out y apagamos el sistema 
 * microchipdeveloper.com/16bit:doze-idle-sleep

Preguntas para Miguel:
Â¿Porque luxes1 y luxes2 son globales?

1) Si el micro se despierta por el T1, entramos en un bucle infinito...
2) Como establecer las prioridades de las tareas? No queremos que se ejecute el control una vez se ha estabilizado,
   sino volver a BucleScan y de ahÃ­ al modo Sleep();
 */


/* SemÃ¡foros para sincronizar las tareas */
//static SemaphoreHandle_t sem_TomaDatos; // SemÃ¡foro para iniciar la lectura de los ADC
static SemaphoreHandle_t sem_TareaControl; //Para iniciar el cÃ¡lculo del control una
//vez se han leÃ­do los ADC

// Prototipos funciones prop1ias
void vApplicationIdleHook(void); //En esta funciÃ³n se va a poner el cÃ³digo para 
void InitTimer2(void);
void InitHardware(void);

void AplicarControl(void *pvParameters);

//Interrupciones
/*
* INT0
* InterrupciÃ³n para despertar al sistema del estado de reposo/sleep
*/
void __attribute__((interrupt(no_auto_psv) )) _INT0Interrupt(void){
      
    //Despierta el sistema (esto lo hace tanto esta interrupciÃ³n como la del Timer 1 (el del sistema operativo)))
    
    //ActivaciÃ³n de interrupciones de timer2
    IEC0bits.T2IE = 1;
    
    IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag     
    return;
}

/*
* T2
* InterrupciÃ³n cada tick del timer 2
* Permite hacer el control digital y timeout para poner en reposo al sistema
*/
void __attribute__((interrupt(no_auto_psv) )) _T2Interrupt(void){
    
    BaseType_t xTaskWoken = pdFALSE;
    
    //Dar semÃ¡foro a TareaControl
    xSemaphoreGiveFromISR(sem_TareaControl, &xTaskWoken);

    if(xTaskWoken == pdTRUE){
        taskYIELD (); /* Si el semÃ…foro ha despertado
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

    return; // Nunca se llega aquÃ­
}

*/
void AplicarControl(void *pvParameters){ // Aplica control PI hasta alcanzar regimen permanente
    // DefiniciÃ³n de variables
    static int cont1,cont2;
    static unsigned int luxes1,luxes2,luxes3,luxes4;
    int lectura1,lectura2;
    float prop1, mando1;
    float prop2, mando2;
    float integ1 = 0;
    float integ2 = 0;
    float kprop1 = 8; //kprop1 = 0.8;
    float kinteg1 = 800 ; //kinteg1 = 611.16; 400
    float kprop2 = -3.59; 
    float kinteg2 = -137.3 ; 
    int reg_permanente1 = 0; // flag para comprobar si estamos en regimen permanente
    int reg_permanente2 = 0;

    //For debugging
    char str[80];
    int contador1 = 0;
    int contador2 = 0;

    while(1){
        if(xSemaphoreTake(sem_TareaControl, (TickType_t) 1000)== pdTRUE ){
            // Lectura de medidas - esta tarea la incluimos dentro de la tarea de Control 
            //                      ya que no tiene un delay demasiado alto
            Disable();
            luxes2 = 2*LeerLuxes(PIN_ADC_2);
            luxes1 = LeerLuxes(PIN_ADC_1);
            luxes3 = LeerLuxes(PIN_ADC_5);
            luxes4 = LeerLuxes(PIN_ADC_6);
            Enable();

            // Calculamos la entrada para el control (diferencia de luxes y aplicamos control)
            lectura1 = luxes1-luxes2;
            lectura2 = luxes3-luxes4;
           
            // Regimen permanente en el control de cabeceo 
            if (lectura1 < REF + MARGEN_REFPERM && lectura1 > REF - MARGEN_REFPERM){
                cont1++;
                if(cont1==UMBRAL_REGPERM){
                    reg_permanente1 = 1;
                    cont1=0;
                }

            }else{
                cont1 = 0;
                reg_permanente1 = 0;
            }
            
            // Regimen permanente en el control de giro base
            if (lectura2 < REF + MARGEN_REFPERM && lectura2 > REF - MARGEN_REFPERM){
                cont2++;
                if(cont2==UMBRAL_REGPERM){
                    reg_permanente2 = 1;
                    cont2=0;
                }

            }else{
                cont2 = 0;
                reg_permanente2 = 0;
            }
            

            if(!reg_permanente1){

                // La referencia es 0, por lo que el control debe conseguir que dicha lectura se haga 
                // en las siguientes iteraciones. La prop1ia lectura se considera el error.
                prop1 = kprop1 * (float)lectura1;
                integ1 += (float)lectura1 * TS_CONTROL; 

                // CÃ¡lculo de mando1
                mando1 = prop1 + kinteg1 * integ1;
                
                if(contador1 == 800){
                    sprintf(str,"Luxes 1, 2: %d, %d mando1: %d;\n", luxes1,luxes2,mando1);
                    putsUART(str);
                    contador1 = 0;
                }
                // Anti-Windup
                if (mando1 > MANDO_MAX) { 
                    mando1 = MANDO_MAX;
                    integ1 -= (float)lectura1 * TS_CONTROL;

                }else if (mando1 < MANDO_MIN) {
                    mando1 = MANDO_MIN;
                    integ1 -= (float)lectura1 * TS_CONTROL;
                }

                // ActuaciÃ³n sobre motor
                setDcPWM(PIN_PWM1, mando1);

            }
            
            //if(1){
                if (!reg_permanente2){
                // Control del giro base

                prop2 = kprop2 * (float)lectura2;
                integ2 += (float)lectura2 * TS_CONTROL; 

                // CÃ¡lculo de mando1
                mando2 = prop2 + kinteg2 * integ2;
                
                if(contador2 == 800){
                    sprintf(str,"Luxes 3 , 4: %d, %d mando2: %d;\n", luxes3,luxes4,mando2);
                    putsUART(str);
                    contador2 = 0;
                }
                // Anti-Windup
                if (mando2 > MANDO_MAX) { 
                    mando2 = MANDO_MAX;
                    integ2 -= (float)lectura2 * TS_CONTROL;

                }else if (mando2 < MANDO_MIN) {
                    mando2 = MANDO_MIN;
                    integ2 -= (float)lectura2 * TS_CONTROL;
                }

                // ActuaciÃ³n sobre motor
                setDcPWM(PIN_PWM2, mando2);
                }
            }
            
            
            contador1 ++; //debug
            contador2 ++;
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
activa el modo sleep del micro. Este dormirÃ¡ hasta que salte la interrupcion INT0.
para permanecer dormido, es importante desactivar las interrupciones del timer2.
*/
void vApplicationIdleHook(void){
    //Apagar Interrupciones 
    IEC0bits.T2IE = 0;

    // Activamos el modo sueÃ±o
    Sleep();
}

void InitHardware(void){
    inicializarADCPolling(1<<PIN_ADC_1 | 1<<PIN_ADC_2 | 1<<PIN_ADC_5 | 1<<PIN_ADC_6);    
    InicializarReloj();
    /*
    inicializarPWM(PIN_PWM1, FRECUENCIA);
    activarPWM(PIN_PWM1);
    setFrecuencia(FRECUENCIA);
     
    
    inicializarPWM2(PIN_PWM2, FRECUENCIA);
    activarPWM2(PIN_PWM2);
    setFrecuencia2(FRECUENCIA);
    */
    
    inicializarPWM(PIN_PWM1 | PIN_PWM2, FRECUENCIA);
    activarPWM(PIN_PWM1 | PIN_PWM2);
    setFrecuencia(FRECUENCIA);
    
    inicializarUART(BAUD_RATE);
    InitTimer2();

    return;
}



