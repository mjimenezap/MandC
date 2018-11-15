/* 
 * File:   main.c
 * Author: mjime
 *
 * Created on November 15, 2018, 7:56 PM
 */

// Directivas del precompilador
#include <p33FJ128MC802.h>
#include <stdio.h>
#include "adc.h"
#include "config.h"
#include "Idle.h"
#include "uart.h"
#include "sensor.h"
#include "pwm.h"

#define PIN_ADC 0
#define TS 1 //1 décima de milisegundo => 0.1 ms
#define BAUD_RATE 115200
#define TAM_TR_UART 250
#define TAM_REC_UART 250
#define PR_INT_TX 5

#define FRECUENCIA 50//  50 Hz, 20 ms
#define PIN_PWM (1<<15)//PIN RB15

#define Mando0  15//Se corresponde con 0.3 ms de 2 ms
#define Mando45 38//Se corresponde con 0.75 ms de 2 ms
#define Mando90 60//Se corresponde con 1.2 ms de 2 ms


void InitDevices(void);
void Ensayo(int i);

void main(void) {
    
    //Decalración de variables
    char str[15];
    unsigned int luxes;
    int contador = 0;

    // Inicialización de dispositivos
    InitDevices();  

    // Bucle scan //Cada ejecución del código es 0.1 ms
    while(1){
        

        if((contador >= 1800) && (contador <= 2000)){ //Primera parte entre 1.8 y 2 seg
              luxes = LeerLuxes(PIN_ADC);
              sprintf(str,"45, %d;\n", luxes);
              putsUART(str);

              
        } else if ((contador >= 2000) && (contador <= 2200)){ // Segunda parte entre 2 y 2.2 seg
            luxes = LeerLuxes(PIN_ADC);
            sprintf(str,"90, %d;\n", luxes);
            putsUART(str);

        }
        
        Ensayo(contador);
        contador = contador +1;
        if (contador == 4000){
            contador = 0;
        }
        tareaIdle();


    }
        

    return;
}

void Ensayo(int i){

    switch(i){
            case 0:
                setDcPWM(PIN_PWM, Mando45);
                break;
                
            case 2000:
                setDcPWM(PIN_PWM, Mando90);
                break;
                
            case 4000: // Cada 4 segundos repetimos el ensayo
                i = 0;
                break;
            default:
                ;
        }
  
    return;
}

void InitDevices(void){
    
    inicializarADCPolling(1<<PIN_ADC);
    inicializarReloj();
    inicializarTareaIdle(TS);
    inicializarUART(BAUD_RATE);
    inicializarPWM(PIN_PWM, FRECUENCIA);
    activarPWM(PIN_PWM);
    setFrecuencia(FRECUENCIA);

    setDcPWM(PIN_PWM, Mando0);
    __delay_us(100000);  //Para que espere 1 segundo en la posición de reset
   
    return;
}

