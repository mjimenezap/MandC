// -----------------------------------------------------------------------------
// --------------------------------- MÓDULO DE PWM  ----------------------------
// -----------------------------------------------------------------------------

/**
 * @file     pwm2.c
 *
 * @author   Jaime Boal Martín-Larrauri, José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     04/10/2016
 *
 * @brief    Módulo encargado de generar señales de PWM usando el módulo PWM2
 *           del microcontrolador.
 */

// -----------------------------------------------------------------------------
#include <xc.h>
#include "pwm2.h"
#include "config.h" // Para la definición de FCY

// -----------------------------------------------------------------------------
// ----------------------------------- FUNCIONES -------------------------------
// -----------------------------------------------------------------------------

void inicializarPWM2(unsigned int bit_map, unsigned int frecuencia)
{
    P2TCONbits.PTSIDL  = 0;  // Continuar funcionando en modo suspensión
    P2TCONbits.PTOPS   = 0;  // Postescalado: 1:1 (aumentar el tiempo entre interrupciones)
    P2TCONbits.PTCKPS  = 0;  // Preescalado: 1:1
    P2TCONbits.PTMOD   = 0;  // Modo de funcionamiento: Free-running (sólo subida)
    PWM2CON1 = 0x0700;       // Pares de pines independientes y deshabilitados como MCPWM

    // Se configuran los bits de "override" para que se conecte la salida del
    // generador de PWM con las salidas correspondientes del puerto B.
    if(bit_map & (1<<9)){ // PWM2L1
        P2OVDCONbits.POVD1L = 1;
    }
    if(bit_map & (1<<8)){ // PWM2H1
        P2OVDCONbits.POVD1H = 1;
    }
    
    setFrecuencia2(frecuencia);
    
    // Factor de servicio inicial nulo
    P2DC1 = 0;
 
    PWMCON2bits.IUE    = 0;	 // Sincronizar el cambio de factor de servicio con el periodo
    PWMCON2bits.OSYNC  = 1;	 // Sincronizar cambios en las salidas a través de OVDCON con el periodo
    PWMCON2bits.UDIS   = 0;	 // Permitir la modificación del factor de servicio y del periodo

    P2TCONbits.PTEN    = 1;  // Encender el módulo generador de PWM2
}

void setFrecuencia2(unsigned int frecuencia)
{
    unsigned long ul_ptper;
    unsigned char uc_preescalado = 0;

    do {
        // PTPER = FCY * PREESCALADO * Periodo - 1 (máx. 32767)
        ul_ptper = (unsigned long)(FCY/frecuencia)/(1 << 2*uc_preescalado) - 1;
        uc_preescalado++;
    } while (ul_ptper > 32767 && uc_preescalado <= 3);

    if (ul_ptper > 32767)
        ul_ptper = 32767;

    P2TCONbits.PTCKPS = uc_preescalado - 1;
    P2TPER = ul_ptper & 0x7FFF;
}

// -----------------------------------------------------------------------------

void setDcPWM2(unsigned int bit_map, unsigned int dc)
{
    unsigned int p2dc1;

    if (dc > 10000){
	dc = 10000;
    }
    // PTPER se multiplica por 2 porque un DC igual al
    // periodo implica un factor de servicio del 50%.
    p2dc1 = (unsigned int)(2L*P2TPER*dc/10000);

    if(bit_map & ( (1<<9)|(1<<8) )){ // se desea modificar el dc de un pin del
        // canal 1
        P2DC1 = p2dc1;
    }
}

// -----------------------------------------------------------------------------

void activarPWM2(unsigned int bit_map)
{
    if(bit_map &(1<<9)){ // Pin 9 es PWM2L1
        PWM2CON1 |=  1 << 0;
    }
    if(bit_map &(1<<8)){ // Pin RB8 es PWM2H1
        PWM2CON1 |=  1 << 4;
    }
}

void desactivarPWM2(unsigned int bit_map)
{
    if(bit_map &(1<<9)){ // Pin 9 es PWM2L1
        PWM2CON1 &= ~(1 << 0);
    }
    if(bit_map &(1<<8)){ // Pin RB8 es PWM2H1
        PWM2CON1 &= ~(1 << 4);
    }
}

