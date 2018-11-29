/* 
 * File:   Serie.h
 * Author: daniel
 *
 * Created on 8 de noviembre de 2014, 11:29
 */

#ifndef SERIE_H
#define	SERIE_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Inicializa la UART y las colas usadas para enviar y recibir caracteres de la
 * UART.
 */
void InicializarSerie(void);

/**
 * Tarea para procesar los caracteres recibidos por el puerto serie. En esta
 * demo se limita a visualizar los 4 bits menos significativos del primer
 * carácter de la cadena recibida por los LEDs de la placa.
 *
 * @param pvParameters No usado. Ha de dejarse a NULL.
 */
void ProcesaRecSerie(void *pvParameters);

/**
 * Transmite una cadena de caracteres por la UART.
 *
 * @param pcad cadena de caracteres a transmitir
 */

void putsUART(char *pcad);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIE_H */

