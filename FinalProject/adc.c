/*
 * adc.c
 *
 *  Created on: Mar 27, 2025
 *      Author: gbrix
 */


#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <adc.h>

//Initialize ADC module 0 and sequencer 3
void adc_init (void){

    //enable clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= 0x02;

    //enable clock for ADC0
    SYSCTL_RCGCADC_R |= 0x01;

    //wait for GPIOB and ADC0 peripherals to be ready
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    while ((SYSCTL_PRADC_R & 0x01) == 0) {};

    //enable alternate functions on port B pins
    GPIO_PORTB_AFSEL_R |= 0x8;

    //sets pin 4 to input
    GPIO_PORTB_DIR_R &= ~0x8;

    //disable digital functionality on port B pins
    GPIO_PORTB_DEN_R &= ~0x8;

    //enable analog function on port B pin
    GPIO_PORTB_AMSEL_R |= 0x8;

    //disable sequencer 3 to configure it
    ADC0_ACTSS_R &= ~0x0008;

    //specify software start mode for sequencer 3
    ADC0_EMUX_R &= ~0xF000;

    //configure input source for sequencer 3
    ADC0_SSMUX3_R = 10;

    //configure sample control bits 0110
    ADC0_SSCTL3_R = 0x0006;

    //Re-enable sequencer 3
    ADC0_ACTSS_R |= 0x0008;
}


//Busy-wait analog to digital converstion which outputs 12-bit result of ADC conversion
uint16_t adc_read (void){
    uint32_t result;

    ADC0_PSSI_R = 0x0008; // 1) initiate SS3

    while((ADC0_RIS_R&0x08)==0){}; // 2) wait for conversion done

    result = ADC0_SSFIFO3_R&0xFFF; // 3) read result

    ADC0_ISC_R = 0x0008; // 4) acknowledge completion

    return result;
}
