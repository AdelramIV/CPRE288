/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "servo.h"
#include <stdint.h>

int main(void) {
    timer_init();
    lcd_init();

    servo_init();

    servo_move(0);
    timer_waitMillis(2000);
    servo_move(90);
    timer_waitMillis(2000);
    servo_move(45);
    timer_waitMillis(2000);
    servo_move(150);
    timer_waitMillis(2000);
    servo_move(90);
    timer_waitMillis(2000);
    servo_move(180);
    timer_waitMillis(2000);

}
