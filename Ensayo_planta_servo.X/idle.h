/**
 * @file     idle.h
 *
 * @author   José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     08/09/2015
 *
 * @brief    Funciones de manejo de la tarea Idle para un sistema basado en
 *            bucle de scan. Dicha tarea usa el timer 1 para llevar a cabo la
 *            temporización del bucle.
 */

#ifndef IDLE_H
#define	IDLE_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Inicializa la tarea Idle para usar el periodo de muestreo t_s
 *
 * En un sistema basado en bucle de scan es necesario usar un timer para
 * que las iteraciones del bucle duren siempre lo mismo. Esta función inicializa
 * el timer 1 con un periodo igual a t_s. Cuando se llame a la función
 * TareaIdle, ésta esperará al final de dicho periodo. Ojo, el valor máximo
 * del periodo de muestreo es de 423,5 ms; ya que es el máximo valor que
 * puede contar el timer 1 sin rebosar.
 *
 * @param[in] t_s periodo de muestreo en décimas de ms. El valor máximo es de
 *                4235, ya que es el valor máximo que puede contar el timer 1
 *                usando un prescaler de 256 sin rebosar.
 */

void inicializarTareaIdle(unsigned int t_s);

/**
 * Tarea Idle del bucle de scan
 *
 * Esta tarea se queda bloqueada hasta el final del periodo de muestreo, marcado
 * por el final de cuenta del timer 1. El periodo de muestreo se define con
 * la llamada a InicializarTareaIdle().
 */

void tareaIdle(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IDLE_H */

