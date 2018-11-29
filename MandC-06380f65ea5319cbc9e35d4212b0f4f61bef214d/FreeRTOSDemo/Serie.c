#include <p33FJ128MC802.h>
#include <stdint.h>

#include "Hora.h"
#include "Interrupciones.h"
#include "DriverPicTrainer/config.h"

// Includes del Kernel.
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

void InicializarUART(unsigned long baudrate);
void ProcesaMensaje(char *pmensaje);

QueueHandle_t cola_env;      // Cola para enviar caracteres a la UART
QueueHandle_t cola_rec;      // Cola para recibir de la UART
#define TAM_COLA 100         // Tamaño de la cola
#define TAM_MSG sizeof(char) // y de los elementos de la cola

void InicializarSerie(void)
{
  // Como se van a usar los LEDs, se configura el puerto aquí
  TRISB =  0x0FFF;
  // Primero se crean las colas. Ambas del mismo tamaño
  cola_rec = xQueueCreate(TAM_COLA, TAM_MSG);
  cola_env = xQueueCreate(TAM_COLA, TAM_MSG);
  if((cola_rec == NULL) || (cola_env == NULL)){
      // Error fatal: Nos bloqueamos
      while(1);
  }
  InicializarUART(115200); // Inicialización del puerto serie
}

void ProcesaRecSerie(void *pvParameters)
{
  static char mensaje[100];
  static uint8_t indice=0;
  char car_rec;
    
  while(1){
    if(xQueueReceive(cola_rec, &car_rec, portMAX_DELAY)
        == pdTRUE ){
      // Se ha recibido un carácter de la cola.
      // Se almacena en la cadena mensaje
      mensaje[indice] = car_rec;
      if(mensaje[indice] == '\n'){
        // El \verb|\|n indica el final del mensaje
        mensaje[indice+1] = '\0';
        ProcesaMensaje(mensaje);
        indice = 0;
      }else{
        indice++;
      }
    }
  }
}

void ProcesaMensaje(char *pmensaje)
{
    // Saca el primer carácter por los LEDS de la placa
    PORTB &= 0x0FFF;
    PORTB |= ( (~(*pmensaje))&0x0F)<<12;
}

void putsUART(char *pcad){
    // Primero se copia la cadena a la cola de envío
    while(*pcad != '\0'){
        xQueueSend(cola_env, pcad, (TickType_t)1000);
        pcad++;
    }

    IEC0bits.U1TXIE = 1;  // Habilitar las interrupciones de transmisión
    IFS0bits.U1TXIF = 1;  // Provocar una interrupción para emprezar a transmitir
}
/**
 * Inicializa la UART.
 *
 * Se inicializa la UART para usar una trama de 8 bits de datos sin paridad y
 * con un bit de stop. El módulo usa interrupciones tanto para la recepción como
 * para la transmisión. La comunicación con las rutinas de interrupción se
 * realiza mediante dos colas.
 *
 * @param baudrate Baudrate de la uart en baudios
 */
#define PR_INT_TX 4
#define PR_INT_RX 4
void InicializarUART(unsigned long baudrate)
{
    U1BRG = (FCY/baudrate)/16 - 1;  // Velocidad de transmisión

    U1MODEbits.STSEL   = 0;   // Bits de parada: 1
    U1MODEbits.PDSEL   = 0;   // Bits de datos: 8, Paridad: Ninguna
    U1MODEbits.ABAUD   = 0;   // Deshabilitar la detección de velocidad
    U1MODEbits.BRGH    = 0;   // Low Speed mode
    U1MODEbits.UEN     = 0;   // Usar sólo U1TX y U1RX
    U1MODEbits.USIDL   = 0;   // Mantener el módulo activo en modo reposo

    U1STAbits.UTXISEL0 = 0;   // Interrupcion de Tx: Cuando se vacía U1TXREG
    U1STAbits.UTXISEL1 = 1;
    U1STAbits.URXISEL  = 0;   // Interrupcion de Rx: Al recibir un caracter

    IPC3bits.U1TXIP = PR_INT_TX; // Prioridad de la interrupción de transmisión
    IPC2bits.U1RXIP = PR_INT_RX; // Prioridad de la interrupción de recepción

    IFS0bits.U1TXIF    = 0;   // Borrar la bandera de la interrupción Tx
    IFS0bits.U1RXIF    = 0;   // Borrar la bandera de la interrupción Rx

    IEC0bits.U1TXIE    = 0;   // Deshabilitar las interrupciones de transmisión
    IEC0bits.U1RXIE    = 1;   // Habilitar las interrupciones de recepción

    U1MODEbits.UARTEN  = 1;   // Habilitar el módulo UART
    U1STAbits.UTXEN    = 1;   // Habilitar la transmisión (Sólo si UARTEN = 1)

    // Se configuran los pines del micro por los que se conecta la UART USB
    __builtin_write_OSCCONL(OSCCON & 0xBF);  // Desbloquear el PPS
    RPINR18bits.U1RXR = 5;  // Asignar U1RX al pin 14 que es RP5
    RPOR2bits.RP4R    = 3;  // Asignar U1TX al pin 11 que es RP4
    __builtin_write_OSCCONL(OSCCON | 0x40);  // Bloquear el PPS
}

/**
 * Rutina de atención a la interrupción de la UART asociada a la recepción.
 */
void __attribute__((interrupt,no_auto_psv))
                  _U1RXInterrupt(void)
{
  char car_rec;
  BaseType_t xHigherPriorityTaskWoken;

  car_rec = U1RXREG;
  xQueueSendFromISR(cola_rec, &car_rec,
                    &xHigherPriorityTaskWoken);

  IFS0bits.U1RXIF = 0; // Borra la bandera de interrupción
  if(xHigherPriorityTaskWoken == pdTRUE ){
    taskYIELD(); // Si el envío a la cola ha despertado
                 // una tarea, se fuerza un cambio de
                 // contexto
  }
}

/**
 * Rutina de atención a la interrupción de la UART asociada a la transmisión.
 */
void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
    char car_emi;
    BaseType_t xHigherPriorityTaskWoken;

    IFS0bits.U1TXIF = 0;  // Borrar la bandera de la interrupción

    if(xQueueReceiveFromISR(cola_env, &car_emi,
                    &xHigherPriorityTaskWoken)==pdTRUE){

        U1TXREG = car_emi;
    }else{
        // Cuando se vacía la cola hay que inhabilitar las interrupciones de
        // transmisión, pues si no, nos metemos en un bucle
        IEC0bits.U1TXIE = 0;

    }
}
