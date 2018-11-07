#include <p33FJ128MC802.h>
#include <stdint.h>
#include <stdio.h>

#include "Hora.h"
#include "Serie.h"
#include "Interrupciones.h"

// Includes del Kernel.
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

typedef struct{
	uint8_t hor;
	uint8_t min;
	uint8_t seg;
}Hora_t;

static Hora_t hora_act={0,0,0};

SemaphoreHandle_t sem_hora; // semáforo para sincronización

void ImprimeHora(void *pvParameters)
{
  Hora_t copia_hora;
  char cadena[12];
  extern SemaphoreHandle_t sem_serie;

  while(1){
    if(xSemaphoreTake(sem_hora, (portTickType) 2000 )
       == pdTRUE ){
      // Ha saltado una nueva interrupción de tiempo
      Disable();
      copia_hora = hora_act;
      Enable();
      sprintf(cadena, "%02d:%02d:%02d\n", copia_hora.hor,
              copia_hora.min, copia_hora.seg);
      if(xSemaphoreTake(sem_serie, (TickType_t) 1000 )
         == pdTRUE ){
        putsUART(cadena); // Se tiene el semáforo: se puede
                          // acceder al puerto serie
        xSemaphoreGive(sem_serie); // Se suelta el semáforo
      }else{
        // Después de 1000 ticks no se ha obtenido el
        // semáforo.
      }
    }
  }
}

void InicializarHora(void)
{
    // Se inicializa el semáforo para sincronizar la Tarea
    // ImprimeHora con la interrupción del timer.
    vSemaphoreCreateBinary(sem_hora);

    // Se pide el semáforo para que hasta que no salte la
    // interrupción no se ejecute la tarea.
    xSemaphoreTake(sem_hora, (portTickType) 0 );

    // Se inicializa el T2 para generar una interrupción cada
    // 100 ms
    T2CON = 0;
	TMR2 = 0;
    // Como la operación siguiente es en tiempo de compilación no pasa nada por
    // usar coma flotante. La división por 64 es por el prescaler.
	PR2 = ( uint16_t ) ( configCPU_CLOCK_HZ*0.1/64 );

	// Como la interrupción del timer 2 usa la API del kernel, su prioridad ha
    // de ser la misma que la del kernel
	IPC1bits.T2IP = configKERNEL_INTERRUPT_PRIORITY;
	IFS0bits.T2IF = 0; // Borra el flag de interrupción
	IEC0bits.T2IE = 1; // Habilita la interrupción

	// Arranca el timer con el prescaler a 64 (prescaler bits a 2)
	T2CON = 0x8020;
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt( void )
{
    static int cien_ms = 0;
    BaseType_t  xTaskWokenByPost = pdFALSE;

    cien_ms++;
    if(cien_ms == 10){
        cien_ms = 0;
        hora_act.seg++;
        if(hora_act.seg == 60){
            hora_act.seg = 0;
            hora_act.min++;
            if(hora_act.min == 60){
                hora_act.min = 0;
                hora_act.hor++;
                if(hora_act.hor == 24){
                    hora_act.hor = 0;
                }
            }
        }
        // Desbloquea la tarea ImprimeHora()
        xSemaphoreGiveFromISR(sem_hora, &xTaskWokenByPost);
    }
    
    // Borrado del flag de interrupción
	IFS0bits.T2IF = 0;

    if(xTaskWokenByPost == pdTRUE ){
        taskYIELD(); // Si el semáforo ha despertado
                     // una tarea, se fuerza un cambio de
                     // contexto
    }
}