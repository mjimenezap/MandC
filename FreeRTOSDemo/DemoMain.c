#include <p33FJ128MC802.h>
#include <stdint.h> // define los tipos uint8\_t, ...
#include <stdio.h>  // Para usar sprintf

#include "DriverPicTrainer/config.h"
#include "DriverPicTrainer/adc.h"
#include "DriverPicTrainer/pwm.h"
#include "DriverPicTrainer/pwm2.h"

#include "Interrupciones.h" // Enable y Disable
#include "Serie.h"          // Módulos del sistema
#include "Hora.h"
#include "Entradas.h"

#include "FreeRTOS.h" // Includes del Kernel
#include "semphr.h"
#include "queue.h"
#include "task.h"

#define PRIO_SERIE 3
#define PRIO_IMP_HORA 2
#define PRIO_ENV_ENTR 1
#define TAM_PILA_SERIE 250 // Esta tarea necesita 100 bytes adicionales para una cadena
#define TAM_PILA 150

SemaphoreHandle_t sem_serie; // Semáforo para el acceso a la UART

#define PIN_ADC 0
#define FRECUENCIA 50
#define PIN_PWM (1<<15)//PIN RB15
#define BAUD_RATE 115200
#define TAM_TR_UART 250
#define TAM_REC_UART 250
#define PR_INT_TX 5

#define REF 0
#define UMBRAL_DORMIR 100
#define UMBRAL_REGPERM 10
#define MARGEN_REFPERM 5 // Luxes

// Prototipos de funciones
void __attribute__((__interrupt__,no_auto_psv)) _MPWM1Interrupt(void);


//Interrupciones
void __attribute__((__interrupt__,no_auto_psv)) _MPWM1Interrupt(void){
    uint16_t adc;
           
    adc = LeerADC(PIN_ADC);
    
        
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
 *
 *
 *
 *
 *
 */



int main(void)
{
    //Incializar Hardware
    inicializarADCPolling(1<<PIN_ADC);
    InicializarReloj();
    inicializarPWM(PIN_PWM, FRECUENCIA);
    activarPWM(PIN_PWM);
    setFrecuencia(FRECUENCIA);
    inicializarUART(BAUD_RATE);

    int cont_controlado;


    // Se inicializa el semáforo
    vSemaphoreCreateBinary(sem_serie);

    // Se crean las tareas
    xTaskCreate(BucleScan,
    (const signed portCHAR * const) "BucScan",
              BUCLE_SCAN_STACK_SIZE, NULL, BUCLE_SCAN_PRIO,
              (xTaskHandle * ) NULL );

    xTaskCreate(EnviaEntradas, "EnvEntr", TAM_PILA, NULL,
                PRIO_ENV_ENTR, NULL);
    xTaskCreate(ProcesaRecSerie,   "PrSer", TAM_PILA_SERIE, NULL,
                PRIO_SERIE, NULL);
    

    vTaskStartScheduler(); // y por último se arranca el
                           // planificador.
    return 0; // En teoría no se debe llegar nunca aquí.
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


void AplicarControl(void){ // Aplica control PI hasta alcanzar regimen permanente
    //Definición de variables
    int cont;
    int lectura;

    cont_controlado=0;

    while(1){
        // Lectura - protegida
        Disable();

        Enable();

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

