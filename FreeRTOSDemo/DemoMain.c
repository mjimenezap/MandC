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
#define FRECUENCIA 5000
#define PIN_PWM (1<<15)//PIN RB15

// Prototipos de funciones
void __attribute__((__interrupt__,no_auto_psv)) _MPWM1Interrupt(void);


//Interrupciones
void __attribute__((__interrupt__,no_auto_psv)) _MPWM1Interrupt(void){
    uint16_t adc;
           
    adc = LeerADC(PIN_ADC);
    
        
    return;
}



int main(void)
{
    //Incializar Hardware
    inicializarADCPolling(1<<PIN_ADC);
    InicializarReloj();
    InicializarHora();
    InicializaEntradas();
    InicializarSerie();
    inicializarPWM(PIN_PWM, FRECUENCIA);
    activarPWM(PIN_PWM);
    setFrecuencia(FRECUENCIA);


    // Se inicializa el semáforo
    vSemaphoreCreateBinary(sem_serie);

    // Se crean las tareas
    xTaskCreate(ImprimeHora,   "ImpHora", TAM_PILA, NULL,
                PRIO_IMP_HORA, NULL);
    xTaskCreate(EnviaEntradas, "EnvEntr", TAM_PILA, NULL,
                PRIO_ENV_ENTR, NULL);
    xTaskCreate(ProcesaRecSerie,   "PrSer", TAM_PILA_SERIE, NULL,
                PRIO_SERIE, NULL);
    

    vTaskStartScheduler(); // y por último se arranca el
                           // planificador.
    return 0; // En teoría no se debe llegar nunca aquí.
}
