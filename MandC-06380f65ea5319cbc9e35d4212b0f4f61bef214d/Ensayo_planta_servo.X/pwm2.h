// -----------------------------------------------------------------------------// --------------------------- INTERFAZ DEL MÓDULO DE PWM ----------------------// -----------------------------------------------------------------------------

/**
 * @file     pwm2.h
 *
 * @author   Jaime Boal Martín-Larrauri, José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     07/09/2015
 *
 * @brief    Módulo encargado de generar señales de PWM usando el módulo PWM2
 *           del microcontrolador
 */

// -----------------------------------------------------------------------------

#ifndef _PWM2_H
#define _PWM2_H

// -----------------------------------------------------------------------------
// ---------------------- PROTOTIPOS DE LAS FUNCIONES PÚBLICAS -----------------
// -----------------------------------------------------------------------------

/**
 * Incializa el módulo PWM. La función configura los pines indicados mediante el
 * parámetro bit_map como salidas del módulo PWM. Dichas salidas se configuran
 * de modo independiente. Además el módulo configura el módulo PWM en el modo
 * free running a la frecuencia indicada mediante el parámetro frecuencia.
 *
 * @param bit_map Indica qué pines se conectan a las salidas del módulo PWM (1)
 *                 o se dejan como E/S digital (0). Por ejemplo si se desea usar
 *                 el pin RB15 y el RB10 como salidas PWM, bit_map será igual a
 *                 (1<<15)|(1<<10).
 * @param frecuencia Frecuencia en Hz de la señal PWM.
 */
void inicializarPWM2(unsigned int bit_map, unsigned int frecuencia);

/**
 * Define la frecuencia del módulo PWM. Nótese que la frecuencia es común para
 * las seis salidas del módulo PWM
 *
 * @param frecuencia Frecuencia en Hz de la señal PWM.
 */
void setFrecuencia2(unsigned int frecuencia);

/**
 * Define el factor de servicio de la señal PWM de una o varias salidas, las
 * cuales se definen mediante el parámetro bit_map. Nótese que las salidas RB15-
 * RB14, RB13-RB12 y RB11-RB10 comparten el mismo canal PWM, por lo que su
 * factor de servicio no puede ser distinto.
 *
 * @param bit_map Indica qué pines se conectan a las salidas del módulo PWM (1)
 *                 o se dejan como E/S digital (0). Por ejemplo si se desea usar
 *                 el pin RB15 y el RB10 como salidas PWM, bit_map será igual a
 *                 (1<<15)|(1<<10).
 * @param dc Factor de servicio en tanto por 10000. Por ejemplo si se
 *           desea un factor de servicio del 50%, el parámetro dc ha de
 *           valer 5000.
 */
void setDcPWM2(unsigned int bit_map, unsigned int dc);

/**
 * Activa las salidas PWM indicadas mediante el parámetro bit_map.
 *  
 * @param bit_map Indica qué pines se activan. Por ejemplo si se desean activar
 *                los pines RB15 y RB10, bit_map será igual a (1<<15)|(1<<10).
 */
void activarPWM2(unsigned int bit_map);

/**
 * Desactiva las salidas  PWM indicadas mediante el parámetro bit_map. Nótese
 * que la desactivación hace que la salida pase a estar controlada por el módulo
 * de E/S digital, por lo que tomará el valor indicado por el bit
 * correspondiente de PORTB.
 * @param bit_map Indica qué pines se desactivan. Por ejemplo si se desean 
 *                desactivar los pines RB15 y RB10, bit_map será igual a 
 *                (1<<15)|(1<<10).
 */
void desactivarPWM2(unsigned int bit_map);


#endif
