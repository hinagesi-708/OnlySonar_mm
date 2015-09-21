
/* 
 * File:   slave_for_OnlySonar_mm_12f1822.c
 * Author: kayoko
 *
 * Created on 2015/09/21, 13:32
 */
//  http://www.geocities.jp/zattouka/GarageHouse/micon/MPLAB/16F1827/USS/USS.htm

/********************************************
 *  compiler    MPLAB XC8(v1.34)            *
 *  PIC         PIC12F1822                  *
 *  clock        16MHz(INTIO)               *
 *                                          *
 *  use_port                                *
 *                __________                *
 *          Vdd---|1  ●   8|---Vss         *
 * (RA5)mmSonar---|2       7|---(RA0)       *
 *      (RA4)×---|3       6|---SCL(RA1)    *
 *      (RA3)×---|4       5|---SDA(RA2)    *
 *                ==========                *
 ********************************************/
#include <xc.h>
#include "I2C_slave.h"

#pragma config CLKOUTEN = OFF
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config CP       = OFF
#pragma config BOREN    = ON
#pragma config FCMEN    = OFF
#pragma config MCLRE    = ON
#pragma config CPD      = OFF
#pragma config IESO     = OFF
#pragma config FOSC     = INTOSC

#pragma config LVP      = ON

#define _XTAL_FREQ 16000000


void init();

int main(void) {
    init();
    I2C_init();
    
    int mm;
    int i2c_ans;

    while (1) {
        mm = 0;
        i2c_ans = 0;
        mm = Pls_mm();
        mm = (mm + 0.18621)/0.251626;
        mm = (mm - 18.33333)/0.980642;
        mm = (mm - 5.4)/1.002109;
        i2c_ans = mm;
            
        send_data[0] = i2c_ans % 0x100;     //dat1 = (char)data;
        send_data[1] = i2c_ans / 0x100;     //dat2 = (char)data >> 8;
//      data = dat2 * 0x100 + dat1; 読み出しの際
        __delay_ms(50);
    }
    return (0);
}

void init() {
    OSCCONbits.IRCF = 0b1111;       //Set oscillator 16MHz
    ANSELA  = 0x00;                 //Set RA pins digital
    TRISA0 = 1;
    TRISA5 = 1;

    PORTA = 0x00;                   //Set PORTA Low
    return;
}

int Pls_mm(){
    long time_mm = 0;
    while(RA5 == 1);
    while(RA5 == 0);
    while(RA5 == 1){
        __delay_us(1);
        time_mm ++;
//        if(time_mm > 5000) break;
    }
    return time_mm;
}

static void interrupt forinterrupt(){
    #include "I2C_slave_int.h"
}
