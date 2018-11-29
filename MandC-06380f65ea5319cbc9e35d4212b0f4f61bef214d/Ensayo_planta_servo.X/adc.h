/**
 * @file     adc.h
 *
 * @author   José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     08/09/2015
 *
 * @brief    Módulo encargado de gestionar el conversor AD del microcontrolador
 */

#ifndef ADC_H
#define ADC_H

/**
 * Inicializar el conversor AD en modo pooling
 *
 * Inicializa el conversor AD para funcionar por polling. Es necesario pasarle
 * a la función un parámetro que indique qué entradas analógicas se van a
 * convertir. Para ello se usa el argumento poniendo a 1 los bits cuyas entradas
 * analógicas queramos usar. Por ejemplo si se van a usar las entradas AN0 y AN2
 * el argumento input_pins será igual a 0x05.
 *
 * @param input_pins bitmap con los pines que se van a usar como entradas
 *                    analógicas
 */
void inicializarADCPolling(unsigned int input_pins);

/**
 * Lee el canal indicado en el parámetro. Para ello lanza una conversión y se
 * bloquea a la espera de que finalice dicha conversión.
 *
 * @param canal Número del canal que se desea leer
 * @return Valor leído del ADC
 */
unsigned int leerADCPolling(unsigned int canal);

#endif
