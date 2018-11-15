/**
 * @file     adc.c
 *
 * @author   José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     08/09/2015
 *
 * @brief    Módulo encargado de gestionar el conversor AD del microcontrolador
 */

#include <xc.h>
#include "adc.h"

void inicializarADCPolling(unsigned int input_pins)
{
    // Se habilitan los canales usados como entradas en los registros TRIS. si
    // no se hace asi, el pin sigue funcionando tambien como salida digital y
    // se interfiere con el ADC.
    TRISA |= (input_pins & 0x03); // AN0 y AN1 están en los bits 0 y 1 del puerto A
    TRISB |= (input_pins>>2) & 0xF; // AN2 a AN5 están en PB0 a PB3

    AD1PCFGL = ~input_pins;
    AD1CON3 = 0x109;
    AD1CON2 = 0x0;
    AD1CON1 = 0x80E0;
}


unsigned int leerADCPolling(unsigned int canal)
{
    AD1CHS0=canal;
    AD1CON1bits.SAMP=1;
    IFS0bits.AD1IF = 0;
    while(!IFS0bits.AD1IF)
        ;
    return ADC1BUF0;
}
