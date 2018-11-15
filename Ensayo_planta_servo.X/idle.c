/**
 * @file     idle.c
 *
 * @author   José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     08/09/2015
 *
 * @brief    Funciones de manejo de la tarea Idle para un sistema basado en
 *           bucle de scan. Dicha tarea usa el timer 1 para llevar a cabo la
 *           temporización del bucle.
 */

#include "xc.h"
#include "config.h" // Se define aquí la frecuencia del micro FCY
#include "idle.h"

void inicializarTareaIdle(unsigned int t_s)
{
    unsigned char prescaler;
    TMR1 = 0;

    if(t_s<16){// no es necesario prescaler
        prescaler = 0; // Sin prescaler
        PR1 = ((unsigned long) t_s) * FCY / 10000L;
    }else if(t_s < 132){ // Se necesita un prescaler a 8
        prescaler = 1;
        PR1 = ((unsigned long) t_s) * (FCY/8) / 10000;
    }else if(t_s < 1058){ // Se necesita un prescaler a 64
        prescaler = 2;
        PR1 = ((unsigned long) t_s) * (FCY/64) / 10000;
    }else if(t_s < 4235){ // Se necesita un prescaler a 64
        prescaler = 3;
        PR1 = ((unsigned long) t_s) * (FCY/256) / 10000;
    }
    IFS0bits.T1IF = 0; // Borra el flag
    T1CON = 0x8000|(prescaler<<4); // Timer on, Prescaler
}

void tareaIdle(void)
{
    while (IFS0bits.T1IF == 0)
        ; // Espera fin del timer
    IFS0bits.T1IF = 0; // Borra el flag
}
