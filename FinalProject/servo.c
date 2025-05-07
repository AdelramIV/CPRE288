///*
// * servo.c
// *
// *  Created on: Apr 10, 2025
// *      Author: gbrix
// */

#include "servo.h"
#include "Timer.h"
#include <stdint.h>

 int leftVal = 311150;
 int rightVal = 283361;

void servo_init (void){
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCTIMER_R |= 0b10;
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};


    //--------- GPIO CONFIG ---------
    GPIO_PORTB_DIR_R |= 0b100000;
    GPIO_PORTB_DEN_R |= 0b100000;

    GPIO_PORTB_AFSEL_R |= 0b100000;
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;
    GPIO_PORTB_PCTL_R |= 0x700000;

    //--------- TIMER SETUP ----------
    TIMER1_CTL_R &= ~0x100;
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R |= 0b1010;
    TIMER1_TBMR_R &= ~0b0101;
    TIMER1_CTL_R &= ~0x4000;

    TIMER1_TBILR_R = 0xE200;

    TIMER1_TBPR_R = 0x4;

    TIMER1_TBMATCHR_R = 0xC000;
    TIMER1_TBPMR_R = 0x4;

    TIMER1_CTL_R |= 0x100;
}


void servo_move(uint16_t degrees){
    unsigned int lowCycles = leftVal - ((leftVal-rightVal)/180)*degrees;

    lcd_printf("%d", lowCycles);

    TIMER1_TBMATCHR_R = (lowCycles & 0xFFFF);
    TIMER1_TBPMR_R = lowCycles>>16;
}
