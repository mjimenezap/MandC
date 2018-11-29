// -----------------------------------------------------------------------------
// --------- MÓDULO UART (Universal Asynchronous Receiver Transmitter) ---------
// -----------------------------------------------------------------------------

/**
 * @file     uart.c
 *
 * @author   Jaime Boal Martín-Larrauri, José Daniel Muñoz Frías
 *
 * @version  1.1.0
 *
 * @date     08/09/2015
 *
 * @brief    Módulo encargado de gestionar las comunicaciones USB.
 */

// -----------------------------------------------------------------------------

#include <xc.h>
#include "uart.h"
#include "config.h" // Se define aquí FCY, necesario para configurar el baudrate


// -----------------------------------------------------------------------------
// -------------------------------- PARÁMETROS ---------------------------------
// -----------------------------------------------------------------------------

/// Tamaño de los vectores y colas
#define TAM_TR_UART  250  // Cola de transmisión
#define TAM_REC_UART 250  // Cola de recepción

/// Prioridad de las interrupciones (máx. 7 - mín. 1)
#define PR_INT_TX 5  // Transmisión
#define PR_INT_RX 6  // Recepción

// -----------------------------------------------------------------------------
// ----------------------- VARIABLES GLOBALES AL MÓDULO ------------------------
// -----------------------------------------------------------------------------

static unsigned char uc_cola_transmision[TAM_TR_UART];
static unsigned char uc_cola_recepcion[TAM_REC_UART];
static unsigned int ui_icabeza_tr  = 0;
static unsigned int ui_icola_tr    = 0;
static unsigned int ui_icabeza_rec = 0;
static unsigned int ui_icola_rec   = 0;

// -----------------------------------------------------------------------------
// ------------------- PROTOTIPOS DE LAS FUNCIONES PRIVADAS --------------------
// -----------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void ponerEnColaTransmisionUART(unsigned char uc_caracter);
void transmitirUART(void);

// -----------------------------------------------------------------------------
// ---------------------------- FUNCIONES PÚBLICAS -----------------------------
// -----------------------------------------------------------------------------

void inicializarUART(unsigned long baudrate)
{
    U1BRG = (FCY/baudrate)/16 - 1;  // Velocidad de transmisión

    U1MODEbits.STSEL   = 0;   // Bits de parada: 1
    U1MODEbits.PDSEL   = 0;   // Bits de datos: 8, Paridad: Ninguna
    U1MODEbits.ABAUD   = 0;   // Deshabilitar la detección de velocidad
    U1MODEbits.BRGH    = 0;   // Low Speed mode
    U1MODEbits.UEN     = 0;   // Usar sólo U1TX y U1RX
    U1MODEbits.USIDL   = 0;   // Mantener el módulo activo en modo reposo

    U1STAbits.UTXISEL0 = 0;   // Interrupcion de Tx: Cuando se vacía U1TXREG
    U1STAbits.UTXISEL1 = 1;
    U1STAbits.URXISEL  = 0;   // Interrupcion de Rx: Al recibir un caracter

    IPC3bits.U1TXIP = PR_INT_TX; // Prioridad de la interrupción de transmisión
    IPC2bits.U1RXIP = PR_INT_RX; // Prioridad de la interrupción de recepción

    IFS0bits.U1TXIF    = 0;   // Borrar la bandera de la interrupción Tx
    IFS0bits.U1RXIF    = 0;   // Borrar la bandera de la interrupción Rx

    IEC0bits.U1TXIE    = 0;   // Deshabilitar las interrupciones de transmisión
    IEC0bits.U1RXIE    = 1;   // Habilitar las interrupciones de recepción

    U1MODEbits.UARTEN  = 1;   // Habilitar el módulo UART
    U1STAbits.UTXEN    = 1;   // Habilitar la transmisión (Sólo si UARTEN = 1)

    // Se configuran los pines del micro por los que se conecta la UART USB
    __builtin_write_OSCCONL(OSCCON & 0xBF);  // Desbloquear el PPS
    RPINR18bits.U1RXR = 5;  // Asignar U1RX al pin 14 que es RP5
    RPOR2bits.RP4R    = 3;  // Asignar U1TX al pin 11 que es RP4
    __builtin_write_OSCCONL(OSCCON | 0x40);  // Bloquear el PPS

    TRISB |= 1<<5; // Configura RB5 como entrada, ya que es el U1RX
}

// -----------------------------------------------------------------------------

void procesarUART(void)
{
    // Mientras haya datos por procesar
    while (ui_icola_rec != ui_icabeza_rec) {
        // A rellenar por la aplicación.
    }

    transmitirUART();
}

void putsUART(char *pcad){
    while(*pcad != '\0'){
        ponerEnColaTransmisionUART(*pcad);
        pcad++; 
    }
    transmitirUART();
}

char getcharUART(void)
{
    char char_ret;

    if(ui_icola_rec != ui_icabeza_rec){ // Hay un carácter nuevo
        char_ret = uc_cola_recepcion[ui_icola_rec];
        ui_icola_rec++;
        if(ui_icola_rec == TAM_REC_UART){
            ui_icola_rec=0;
        }
    }else{ // Cola vacía
        char_ret = '\0';
    }
    
    return char_ret;
}
// -----------------------------------------------------------------------------
// ---------------------------- FUNCIONES PRIVADAS -----------------------------
// -----------------------------------------------------------------------------

/**
 * Rutina de atención a la interrupción de la UART asociada a la transmisión.
 */
void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;  // Borrar la bandera de la interrupción

    if (ui_icola_tr != ui_icabeza_tr){ // Hay datos en la cola
        U1TXREG = uc_cola_transmision[ui_icola_tr++];

        if (ui_icola_tr == TAM_TR_UART){
            ui_icola_tr = 0;
        }
    }else{ // Cola vacía. Se deshabilita la interrupción de transmisión para
           // que no estemos continuamente interrumpiendo al no tener datos
           // que transmitir.
        IEC0bits.U1TXIE = 0;
    }
}

// -----------------------------------------------------------------------------

/**
 * Rutina de atención a la interrupción de la UART asociada a la recepción.
 *
 * La rutina introduce el caracter recibido en la cola sólo si ésta no está
 * llena. Si lo está enciende el LED RB12 de la tarjeta para avisar al usuario.
 * en este caso el caracter recibido por la interrupción se pierde.
 */
void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;  // Borrar la bandera de la interrupción

    if( (ui_icabeza_rec+1 == ui_icola_rec) ||
        (ui_icabeza_rec+1 == TAM_REC_UART && ui_icola_rec == 0) ){
        // Cola llena
        PORTB &= ~(1<<12); // Enciende led RB12
    }else{
        uc_cola_recepcion[ui_icabeza_rec] = U1RXREG;
        ui_icabeza_rec++;
        if (ui_icabeza_rec == TAM_TR_UART){
            ui_icabeza_rec = 0;
        }
    }
}

// -----------------------------------------------------------------------------

/**
 * Coloca un dato en la cola de transmisión.
 *
 * La función introduce el carácter en la cola sólo si ésta no está llena. Si
 * lo está enciende el LED RB12 de la tarjeta para avisar al usuario. En este
 * caso el caracter enviado a la función se pierde.
 *
 * @param[in]  uc_caracter  Caracter que se quiere poner en cola.
 */
void ponerEnColaTransmisionUART(unsigned char uc_caracter)
{
    if( (ui_icabeza_tr+1 == ui_icola_tr) ||
        (ui_icabeza_tr+1 == TAM_TR_UART && ui_icola_tr == 0) ){
        // Cola llena
        PORTB &= ~(1<<12); // Enciende led RB12
    }else{
        uc_cola_transmision[ui_icabeza_tr] = uc_caracter;
        ui_icabeza_tr++;
        if (ui_icabeza_tr == TAM_TR_UART){
            ui_icabeza_tr = 0;
        }
    }
}

// -----------------------------------------------------------------------------

/**
 * Envía todos los datos almacenados en la cola de transmisión.
 */
void transmitirUART(void)
{
    IEC0bits.U1TXIE = 1;  // Habilitar las interrupciones de transmisión
    IFS0bits.U1TXIF = 1;  // Provocar una interrupción para emprezar a transmitir
}
