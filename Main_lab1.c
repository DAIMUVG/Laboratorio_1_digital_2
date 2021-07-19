// Dispositivo:    PIC 16F887
// Hadward:        potenciometro, leds, botones, display 7 segs
// Autor:          Dylan Ixcayau 18486
// Fecha Creada:   16 de julio del 2021
// Ultima Actualizacion:    

// ----------------- Laboratorio No.1 ----------------------------------------

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//----------------------Importancion de librerias-------------------------------
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ADC.h"

#define _XTAL_FREQ 4000000

#define tesbit(var,bit)((var)&   (1<<(bit)))
#define tesbit(var,bit)((var)|=  (1<<(bit)))
#define tesbit(var,bit)((var)&=~ (1<<(bit)))

//-------------------Declaración de variables-----------------------------------
uint8_t v1 = 0;
uint8_t disp1 = 0;
uint8_t disp2 = 0;
uint8_t cont  = 0;
uint8_t P  = 0;

void setup(void);
int  tabla(int v1);

void __interrupt() interrupciones(void){
//----------------------------- INTERRUPCION DEL ADC ----------------------------
    /*if(PIR1bits.ADIF == 1){           //Verificamos si la bandera del IoCH es 1  
           P = ADRESH;        //Decrementamos el Puerto C cuando se preciona el boton 
    }
    PIR1bits.ADIF = 0;          //Limpiamos la bandera del IoCH
    */
    recibe_ADC();             //Llamamos a la funcion que se encuentra en la libreria
//----------------------------- INTERRUPCION TMR0 ------------------------------    
    if(T0IF == 1) {       //Verificamos si la bandera del TMR0 es 1
        TMR0  = 236;      //Colocamos el valor a TMR0 para que funcione la interrupcion cada 5ms
        T0IF  = 0;        //Reiniciamos la bandera del TMR0
        PORTE = 0x00;     //Encendemos el Puerto E
        if(cont == 0){       //Si el contador es 0
            v1  = disp1;   //El valor del display 1 se pasa a v1
            PORTD = tabla(v1); //v1 busca en la tabla y pasa el valor convertido
            RE0  = 1;        //Volvemos 0 el pin del puerto E
            cont = 1;        //Cambiamos el valor de cont para que pase al siguiente 
            return;
        }    
        if(cont == 1){ 
            v1 = disp2;
            PORTD = tabla(v1);
            RE1   = 1;
            cont = 0;
            return;
        }    
    }
    
    if (RBIF == 1){     //Verificamos la bandera de interrupcion del puerto B
        RBIF = 0;       //Regresamos la bandera a 0
        if (PORTBbits.RB0 == 0){    //verificamos si el boton 0 esta presionado
            PORTC ++;               //Incrementamos el puerto C
        }
        if (PORTBbits.RB1 == 0){
            PORTC --;               //Decrementamos el puerto C
        }
    }
    
}

//----------------------------Programa principal--------------------------------
void main(void){
     setup();                       //llamamos a la configuracion del pic
    __delay_us(50);                 //delay 50s
    ADCON0bits.GO = 1;              //Dejamos GO como 1
     while(1){                      //Ciclo while 
         disp1 = P & 0x0F;          //Pasamos los primeros 4 bits a la primer variable
         disp2 = (P>>4) & 0x0F;     //Pasamos los otro 4 bits a la segunda variable
         /*if(ADCON0bits.GO == 0){ 
        __delay_us(50);
         }
         ADCON0bits.GO = 1;
         */
        conversionADC();            //Llamamos a la funcion que se encuentra en la libreria
        if(P > PORTC){              // Si p es mayor al puerto C
            PORTAbits.RA1 = 1;      // Se enciende la alarma
        }
        if(P < PORTC || P == 0){    // Si p es menor o igual a 0
            PORTAbits.RA1 = 0;      // se apaga o no se enciende la alarma
        }
        
     }
     
}

//-------------------------Tabla de valores decimales--------------------------
int tabla (int v1){     //La tabla
 int w;                 //Declaramos w de tipo int
 switch (v1)   {         //Entramos a un tipo de "Menu" que se navega con v1
 
     case 0 :           //Opción 0
         w = 0b00111111;    //Numero 0
         break; 
     case 1 :           //Opción 1
         w = 0b00000110;    //Numero 1
         break;         
     case 2 :           //Opción 2
         w = 0b01011011;    //Numero 2
         break;             
     case 3 :           //Opción 3
         w = 0b01001111;    //Numero 3
         break; 
     case 4 :           //Opción 4
         w = 0b01100110;    //Numero 4
         break;
     case 5 :           //Opción 5
         w = 0b01101101;    //Numero 5
         break;
     case 6 :           //Opción 6
         w = 0b01111101;    //Numero 6
         break;
     case 7 :           //Opción 7
         w = 0b00000111;    //Numero 7
         break;
     case 8 :           //Opción 8
         w = 0b01111111;    //Numero 8
         break;
     case 9 :           //Opción 9
         w = 0b01101111;    //Numero 9
            break;              
     case 10 :
         w = 0b01110111;    //Letra A
            break;              
     case 11 :
         w = 0b01111100;    //Letra B
            break;              
     case 12 :
         w = 0b00111001;    //Letra C
            break;              
     case 13 :
         w = 0b01011110;    //Letra D
            break;              
     case 14 :
         w =  0b01111001;    //Letra E
            break;              
     case 15 :
         w = 0b01110001;    //Letra F
            break;              //Rompemos el menu
        default:;
         w = 0;
 }
 return w;
}

void setup(void){
    ANSEL = 0x01;     //Puerto A0 como entrada
    ANSELH = 0x00;    //Todo lo demás en 0
    
    TRISBbits.TRISB0 = 1;   // Habilito el puerto B0 como entrada
    TRISBbits.TRISB1 = 1;   // Habilito el puerto B1 como entrada
    TRISAbits.TRISA0 = 1;   // Habilito el puerto A0 como entrada
    TRISAbits.TRISA1 = 0;   // Habilito el puerto A1 como salida
    TRISD = 0x00;           //Los demas puertos los dejo como salidas
    TRISC = 0x00;           
    TRISE = 0x00;           
    
    PORTA = 0x00;           //Puertos como salidas
    PORTB = 0x00;           
    PORTC = 0x00;           
    PORTD = 0x00;           
    PORTE = 0x00;
    
//-------------------- Configuraciones del Reloj -------------------------------
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;       //Reloj a 4MHz
    OSCCONbits.SCS   = 1;       //Oscilador interno

//-------------------- Configuracion del ADC -----------------------------------
    ADCON1bits.ADFM  = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS  = 1;   //Configuracion del reloj del modulo 
    ADCON0bits.CHS   = 0;   //Seleccionamos el canal donde querramos empezar
    __delay_us(100);
    ADCON0bits.ADON  = 1;   //Encendemos el modulo ADC
    __delay_us(50);
    
//--------------------- Configuraciones del IoC --------------------------------
    OPTION_REGbits.nRBPU = 0;   //Pull up encendido
    WPUB = 0b00000011;          //Habilitamos las resistencias de los botones
    IOCBbits.IOCB0 = 1;         //Interrupciones de cada boton encendidos
    IOCBbits.IOCB1 = 1;
    
//------------------ Configuraciones de TMR0 ------------------------------------
    OPTION_REGbits.T0CS = 0;    //Ciclo de reloj interno
    OPTION_REGbits.PSA  = 0;    //Prescaler para el modulo de timer 0
    OPTION_REGbits.PS2  = 1;
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 1;    //Prescaler a 256
  
   
//-------------------- Configuracion del las interrupciones --------------------
    INTCONbits.GIE  = 1 ;       //Habilitamos las interrupciones globales
    INTCONbits.RBIF = 1 ;       //Dejamos la  bandera levantada
    INTCONbits.RBIE = 1 ;       //Encendemos las interrupciones del puerto B
    INTCONbits.T0IE = 1 ;       //Encendemos la interrupcion del timer0


}
