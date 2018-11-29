// -----------------------------------------------------------------------------
// --------------------------------- MÓDULO DE PWM  ----------------------------
// -----------------------------------------------------------------------------

/**
 * @file     pwm.c
 *
 * @author   Jaime Boal Martín-Larrauri, José Daniel Muñoz Frías
 *
 * @version  1.0.1
 *
 * @date     07/09/2015
 *
 * @brief    Módulo encargado de generar señales de PWM usando el módulo PWM
 *           del microcontrolador.
 */

// -----------------------------------------------------------------------------
#include <xc.h>
#include "pwm.h"
#include "config.h" // Para la definición de FCY

// -----------------------------------------------------------------------------
// ----------------------------------- FUNCIONES -------------------------------
// -----------------------------------------------------------------------------

void inicializarPWM(unsigned int bit_map, unsigned int frecuencia)
{
    P1TCONbits.PTSIDL  = 0;  // Continuar funcionando en modo suspensión
    P1TCONbits.PTOPS   = 0;  // Postescalado: 1:1 (aumentar el tiempo entre interrupciones)
    P1TCONbits.PTCKPS  = 0;  // Preescalado: 1:1
    P1TCONbits.PTMOD   = 0;  // Modo de funcionamiento: Free-running (sólo subida)
    PWM1CON1 = 0x0700;       // Pares de pines independientes y deshabilitados como MCPWM

    // Se configuran los bits de "override" para que se conecte la salida del
    // generador de PWM con las salidas correspondientes del puerto B.
    if(bit_map & (1<<15)){ // PWM1L1
        P1OVDCONbits.POVD1L = 1;
    }
    if(bit_map & (1<<14)){ // PWM1H1
        P1OVDCONbits.POVD1H = 1;
    }
    if(bit_map & (1<<13)){ // PWM1L2
        P1OVDCONbits.POVD2L = 1;
    }
    if(bit_map & (1<<12)){ // PWM1H2
        P1OVDCONbits.POVD2H = 1;
    }
    if(bit_map & (1<<11)){ // PWM1L3
        P1OVDCONbits.POVD3L = 1;
    }
    if(bit_map & (1<<10)){ // PWM1H3
        P1OVDCONbits.POVD3H = 1;
    }
    
    setFrecuencia(frecuencia);
    
    // Factor de servicio inicial nulo
    P1DC1 = 0;
    P1DC2 = 0;
    P1DC3 = 0;

    PWMCON2bits.IUE    = 0;	 // Sincronizar el cambio de factor de servicio con el periodo
    PWMCON2bits.OSYNC  = 1;	 // Sincronizar cambios en las salidas a través de OVDCON con el periodo
    PWMCON2bits.UDIS   = 0;	 // Permitir la modificación del factor de servicio y del periodo

    P1TCONbits.PTEN    = 1;  // Encender el módulo generador de PWM
}

void setFrecuencia(unsigned int frecuencia)
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

    P1TCONbits.PTCKPS = uc_preescalado - 1;
    P1TPER = ul_ptper & 0x7FFF;
}

// -----------------------------------------------------------------------------

void setDcPWM(unsigned int bit_map, unsigned int dc)
{
    unsigned int p1dcx;

    if (dc > 10000){
	dc = 10000;
    }
    // PTPER se multiplica por 2 porque un DC igual al
    // periodo implica un factor de servicio del 50%.
    p1dcx = (unsigned int)(2L*PTPER*dc/10000);

    if(bit_map & ( (1<<15)|(1<<14) )){ // se desea modificar el dc de un pin del
        // canal 1
        P1DC1 = p1dcx;
    }
    if(bit_map & ( (1<<13)|(1<<12) )){ // se desea modificar el dc de un pin del
        // canal 2
        P1DC2 = p1dcx;
    }
    if(bit_map & ( (1<<11)|(1<<10) )){ // se desea modificar el dc de un pin del
        // canal 2
        P1DC3 = p1dcx;
    }
}

// -----------------------------------------------------------------------------

void activarPWM(unsigned int bit_map)
{
    int n;

    for (n=15;n>9;n-=2){ // Recorremos los bit cuyas salidas pueden tener PWML
        if(bit_map &(1<<n)){
            PWM1CON1 |= ( 1<<((15-n)>>1) );
        }
    }
    for (n=14;n>9;n-=2){ // Recorremos los bit cuyas salidas pueden tener PWML
        if(bit_map &(1<<n)){
            PWM1CON1 |= ( 1<<(((14-n)>>1)+4) );
        }
    }
}
void desactivarPWM(unsigned int bit_map)
{
    int n;

    for (n=15;n>9;n-=2){ // Recorremos los bit cuyas salidas pueden tener PWML
        if(bit_map &(1<<n)){
            PWM1CON1 &= ~( 1<<((15-n)>>1) );
        }
    }
    for (n=14;n>9;n-=2){ // Recorremos los bit cuyas salidas pueden tener PWML
        if(bit_map &(1<<n)){
            PWM1CON1 &= ~( 1<<(((14-n)>>1)+4) );
        }
    }
}

