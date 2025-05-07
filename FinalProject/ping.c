/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include <stdio.h>
#include "driverlib/interrupt.h"
#include "uart.h"

// Global shared variables
// Use external declarations in the header file

volatile uint32_t g_start_time = 0;
volatile uint32_t g_end_time = 0;
volatile enum{LOW, HIGH, DONE} g_state = LOW; // State of ping echo pulse

void ping_init (void){

        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

        while (!(SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R3));
        while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1));

        GPIO_PORTB_AFSEL_R |= (1 << 3);
        GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB3_M;
        GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;
        GPIO_PORTB_DEN_R |= (1 << 3);
        GPIO_PORTB_DIR_R &= ~(1 << 3);

        TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
        TIMER3_CFG_R = TIMER_CFG_16_BIT;
        TIMER3_TBMR_R = TIMER_TBMR_TBMR_CAP | TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR;
        TIMER3_CTL_R = TIMER_CTL_TBEVENT_BOTH;
        TIMER3_TBPR_R = 0xFF;
        TIMER3_TBILR_R = 0xFFFF;
        TIMER3_IMR_R |= TIMER_IMR_CBEIM;
        TIMER3_ICR_R = TIMER_ICR_CBECINT;
        TIMER3_CTL_R |= TIMER_CTL_TBEN;

        IntRegister(INT_TIMER3B, TIMER3B_Handler);
        IntEnable(INT_TIMER3B);
        IntMasterEnable();
}

void ping_trigger (void){

    g_state = LOW;

    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
    TIMER3_IMR_R &= ~TIMER_IMR_CBEIM;
    GPIO_PORTB_AFSEL_R &= ~(1 << 3);

    GPIO_PORTB_DIR_R |= (1 << 3);
    GPIO_PORTB_DATA_R &= ~(1 << 3);
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= (1 << 3);
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~(1 << 3);

    TIMER3_ICR_R = TIMER_ICR_CBECINT;
    GPIO_PORTB_DIR_R &= ~(1 << 3);
    GPIO_PORTB_AFSEL_R |= (1 << 3);
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;
    TIMER3_CTL_R |= TIMER_CTL_TBEN;

}

void TIMER3B_Handler(void){


    if (TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
        TIMER3_ICR_R = TIMER_ICR_CBECINT;
        uart_sendStr("Echo received\n");
        if (g_state == LOW) {
            g_start_time = (TIMER3_TBR_R & 0xFFFF) | (TIMER3_TBPR_R << 16);
            g_state = HIGH;
        } else if (g_state == HIGH) {
            g_end_time = (TIMER3_TBR_R & 0xFFFF) | (TIMER3_TBPR_R << 16);
            g_state = DONE;
        }
    }
}

float ping_getDistance (uint32_t* pulse_out){
    ping_trigger();

    int timeout = 100000;
    while (g_state != DONE && timeout-- > 0) {
        timer_waitMicros(1);
    }

    if (g_state != DONE) {
        uart_sendStr("Ping timeout\n");
        *pulse_out = 0;
        return -1.0f;
    }

    uint32_t start = g_start_time;
    uint32_t end = g_end_time;
    uint32_t pulse_width;

    if (end >= start) {
        pulse_width = end - start;
    } else {
        pulse_width = (0xFFFFFF - start) + end;
        //lcd_printf("Overflow Occurred");
    }

    char buffer[50];
    *pulse_out = pulse_width;

    sprintf(buffer, "Pulse width: %d\n", pulse_width);
    uart_sendStr(buffer);

    float time_s = pulse_width / 16000000.0f;
//    float time_s = pulse_width / 62500.0f;
    float distance_cm = (time_s * 34300.0f) / 2.0f;

    sprintf(buffer, "Distance: %f cm\n", distance_cm);
    uart_sendStr(buffer);

    if (distance_cm > 300.0f || distance_cm < 2.0f) return -1.0f;

    return distance_cm * 100;
}
