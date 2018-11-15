// -----------------------------------------------------------------------------
// -- INTERFAZ DEL MÓDULO UART (Universal Asynchronous Receiver Transmitter) ---
// -----------------------------------------------------------------------------

/**
 * @file     uart.h
 *
 * @author   Jaime Boal Martín-Larrauri, José Daniel Muñoz Frías
 *
 * @version  1.1.0
 *
 * @date     08/09/2015
 *
 * @brief    Módulo encargado de gestionar las comunicaciones USB.
 */

// -----------------------------------------------------------------------------

#ifndef _UART_H
#define _UART_H

// -----------------------------------------------------------------------------
// ------------------- PROTOTIPOS DE LAS FUNCIONES PÚBLICAS --------------------
// -----------------------------------------------------------------------------

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
void inicializarUART(unsigned long baudrate);

// -----------------------------------------------------------------------------

/**
 * Procesa los mensajes recibidos a través del puerto USB.
 */
void procesarUART(void);

/**
 * Transmite una cadena de caracteres por la UART.
 *
 * @param pcad cadena de caracteres a transmitir
 */

void putsUART(char *pcad);

/**
 * Obtiene un caracter de la UART si hay alguno disponible. Si no obtiene un \0
 *
 * @return char leído de la cola de recepción de la UART o \0 si la cola está
 *              vacía.
 */

char getcharUART(void);
#endif
