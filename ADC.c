/*
 * File:   ADC.c
 * Author: Dylan Ixcayau 18486
 *
 * Created on July 16, 2021, 5:42 PM
 */
//-----------------Importancion de Librerias------------------------------------
#define _XTAL_FREQ 4000000
#include<stdint.h>
#include <xc.h>
#include <pic16f887.h>
#include "ADC.h"

//-------------------Declaracion de variables-----------------------------------
uint8_t P  = 0;

//-----------------------------Funcion para recibir los datos del ADC-----------
void recibe_ADC(void){
    if(PIR1bits.ADIF == 1){           //Verificamos si la bandera del ADC  
           P = ADRESH;        //Pasamos el valor de ADC a la variable P
    }
    PIR1bits.ADIF = 0;          //Limpiamos la bandera del ADC
}
//-------------------------Funcion de conversion--------------------------------
void conversionADC(void){
    if(ADCON0bits.GO == 0){     //verificamos la bandera de conversion del ADC
        __delay_us(50);         //Dejamos un delay
         }
         ADCON0bits.GO = 1;     //reiniciamos la bandera a su valor inicial
}
    
