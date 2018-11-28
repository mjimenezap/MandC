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

#define PIN_ADC_1 0
#define PIN_ADC_2 1
#define PIN_ADC_5 4
#define PIN_ADC_6 5

//#define TS 1 //1 décima de milisegundo => 0.1 ms
#define TS 10 //10 décimas de milisegundo => 1 ms
#define BAUD_RATE 115200
#define TAM_TR_UART 250
#define TAM_REC_UART 250
#define PR_INT_TX 5

#define FRECUENCIA 50//  50 Hz, 20 ms
//#define PIN_PWM2 (1<<10)//PIN RB15 -> Esto estaba en este archivo... creo que no es RB15 sino 10
#define PIN_PWM1 0//(1<<15)//PIN RB15
#define PIN_PWM2 (1<<16)//PIN RB16

#define Mando0  150//Se corresponde con 0.3 ms de 20 ms
#define Mando45 380//Se corresponde con 0.75 ms de 20 ms
#define Mando90 600//Se corresponde con 1.2 ms de 20 ms
#define Mando135 825//Se corresponde con 1.65 ms de 20 ms
#define Mando180 1050//Se corresponde con 2.1 ms de 20 ms

void InitDevices(void);
void Ensayo(int i);

void main(void) {
    
    //Decalración de variables
    char str[30];
    unsigned int luxes;
    int contador = 0;
    unsigned int luxes1;
    unsigned int luxes2;
   // int debug = 0;
    int debug1 =0;
    int debug2= 0;

    // Inicialización de dispositivos
    InitDevices();  

    // Bucle scan //Cada ejecución del código es 0.1 ms
    while(1){
        

        //if((contador >= 1800) && (contador <= 2000)){ //Primera parte entre 1.8 y 2 seg PARA 0.1 ms
        if((contador >= 200) && (contador <= 500)){ //Primera parte entre 1.8 y 2 seg PARA 1 ms //180,200
            luxes1 = LeerLuxes(PIN_ADC_5); //LeerLuxes(PIN_ADC_1);
            luxes2 = LeerLuxes(PIN_ADC_6);// 2*LeerLuxes(PIN_ADC_2)
            //debug1++;
           // if(debug1==5){       // ==5 hallado de forma empírica
                sprintf(str,"45,%d, %d;\n", luxes1,luxes2);
                luxes = LeerLuxes(PIN_ADC_5)-LeerLuxes(PIN_ADC_6);//LeerLuxes(PIN_ADC_1)-LeerLuxes(PIN_ADC_2);
                //sprintf(str,"45, %d;\n", luxes);
                putsUART(str);
                debug1=0;
            //}

        //} else if ((contador >= 2000) && (contador <= 2200)){ // Segunda parte entre 2 y 2.2 seg PARA 0.1 ms
        } else if ((contador >= 500) && (contador <= 800)){ // Segunda parte entre 2 y 2.2 seg PARA 1 ms //200,220
            luxes1 = LeerLuxes(PIN_ADC_5); //LeerLuxes(PIN_ADC_1);
            luxes2 = LeerLuxes(PIN_ADC_6);// 2*LeerLuxes(PIN_ADC_2)
            //debug2++;
            //if(debug2==5){
                sprintf(str,"90,%d, %d;\n", luxes1,luxes2);
                luxes = LeerLuxes(PIN_ADC_5)-LeerLuxes(PIN_ADC_6);//LeerLuxes(PIN_ADC_1)-LeerLuxes(PIN_ADC_2);
                //sprintf(str,"135, %d;\n", luxes);
                putsUART(str);
                debug2=0;
            //}
        }
        
        /*debug++;
        if(debug==5000){
            debug=0;
            sprintf(str,"45,%d, %d;\n", luxes1,luxes2);
            putsUART(str);
        }
        */
        Ensayo(contador);
        contador = contador +1;
        //if (contador == 4000){ //PARA 0.1 ms
        if (contador == 1000){ //PARA 1 ms //400
            contador = 0;
        }
        tareaIdle();


    }
        

    return;
}

void Ensayo(int i){

    switch(i){
            case 0:
                setDcPWM(PIN_PWM2, Mando45);
                break;
                
            //case 2000: //PARA 0.1 ms
            case 500: //200
               setDcPWM(PIN_PWM2, Mando135);
                break;
                
            //case 4000: // Cada 4 segundos repetimos el ensayo PARA 0.1 ms
            case 1000: // Cada 4 segundos repetimos el ensayo PARA 1 ms    //400
                i = 0;
                break;
            default:
                ;
        }
  
    return;
}

void InitDevices(void){
    
    inicializarADCPolling(1<<PIN_ADC_1 | 1<<PIN_ADC_2 | 1<<PIN_ADC_5 | 1<<PIN_ADC_6);
    inicializarReloj();
    inicializarTareaIdle(TS);
    inicializarUART(BAUD_RATE);
    inicializarPWM(PIN_PWM1 | PIN_PWM2, FRECUENCIA);
    activarPWM(PIN_PWM22);
    setFrecuencia(FRECUENCIA);

    setDcPWM(PIN_PWM2, Mando45);
    //__delay_us(100000);  //Para que espere 1 segundo en la posición de reset
   
    return;
}

