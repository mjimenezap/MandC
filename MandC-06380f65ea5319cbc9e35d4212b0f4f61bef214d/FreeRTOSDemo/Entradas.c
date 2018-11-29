#include <p33FJ128MC802.h>
#include <stdint.h> // define los tipos uint8_t, ...
#include <stdio.h>  // Para usar sprintf

#include "DriverPicTrainer/config.h"
#include "Hora.h"
#include "Serie.h"
#include "Interrupciones.h"

/* Includes del Kernel. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

// Funciones privadas al módulo
uint8_t LeeEntradas(void);

// Inicializa el módulo Entradas, configurando los 4 bits menos
// significativos del puerto B como entradas digitales.
void InicializaEntradas(void)
{
    AD1PCFGL = 0xFFFF;
    TRISB = 0x000F;
}

// Tarea para enviar el estado de las entradas por el puerto serie.
// Sólo se envían cuando hay un cambio de estado.
void EnviaEntradas(void *pvParameters)
{
  char cadena[20]; // Guarda el mensaje a transmitir
  uint8_t entradas;
  static uint8_t entradas_ant;
  extern SemaphoreHandle_t sem_serie;

  entradas_ant = LeeEntradas();
  while(1){
    entradas = LeeEntradas();
    if(entradas_ant != entradas){ // Sólo imprime si
                                  // cambian las entradas
      sprintf(cadena, "Entradas: %02x\n", entradas);
      if(xSemaphoreTake(sem_serie, (portTickType) 1000) == pdTRUE ){
        // Se tiene el semáforo: se puede acceder al
        // puerto serie
        putsUART(cadena);
        // Se suelta el semáforo
        xSemaphoreGive(sem_serie);
      }else{
        // Después de 1000 ticks no se ha obtenido el
        // semáforo. Se podría dar un aviso o
        // simplemente no hacer nada como en este caso
      }
      entradas_ant = entradas;
    }
  }
}

// Devuelve los 4 bits menos significativos del puerto B
uint8_t LeeEntradas(void)
{
    return PORTB&0x0F;
}
