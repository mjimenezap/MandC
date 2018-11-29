/**
 * @File     sensor.c
 *
 * @Date     21/09/2018
 * 
 * @Authors: Miguel Jimenez Aparicio y Cristian Gómez Peces
 *
 *
 * @Brief:   Este módulo permite manejar el sensor más comodamente.  
 * 
 * @MainFunction: LeerLuxes(canal); -> Hay que pasarle el canal por donde lee.
 */

#include "sensor.h" 
#include "DriverPicTrainer/adc.h"


#define N_MUESTRAS 17
unsigned int InterpolarSensor(unsigned int valor_adc){
    
    // Valores de la curva característica de acondicionamiento (Input data)
    static unsigned int vec_adc[N_MUESTRAS] = {304, 316, 338, 353, 372, 465, 496,
        564, 567, 592, 617, 648, 697, 744, 778, 818, 883};
    static unsigned int vec_lux[N_MUESTRAS] = {40, 60, 80, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000};
    
    // Declaración de variables intermedias
    int luxes;
    int intervalo = 0;
    
    // Declaración de la variable a devolver
    unsigned int res;
    
    // Obtención del intervalo en el que se encuentra el valor
    while(intervalo<N_MUESTRAS && valor_adc>vec_adc[intervalo])
        intervalo++;
    
    // Aseguramos que el valor leido se encuentre en el rango de funcionamiento
    if (intervalo == 0)
        intervalo++;
    else if (intervalo == N_MUESTRAS)
        intervalo--;
    
    // Interpolamos - Usamos casts para aproximar y no invertir valores negativos.
    luxes = vec_lux[intervalo-1] + ((float)(vec_lux[intervalo] - vec_lux[intervalo-1]))/(vec_adc[intervalo] - vec_adc[intervalo-1]) * (int)(valor_adc - vec_adc[intervalo-1]);
    
    // Filtro de luxes negativos (al ser unsigned int, no puede haber valores negativos)
    if (luxes<0)
        res = 0;
    else
        res = luxes;
    
    // Devolvemos resultado
    return res;
}

unsigned int LeerADC(unsigned int canal){
    return leerADCPolling(canal);
}

unsigned int LeerLuxes(unsigned int canal){
    unsigned int adc;
    adc = LeerADC(canal);
    return InterpolarSensor(adc);
    
}

