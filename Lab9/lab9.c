/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include <stdio.h>
#include <math.h>

#define STABILITY_THRESHOLD_CM 0.5
#define STABILITY_THRESHOLD_PULSE 100
#define INVALID_LIMIT 5 // Increase to make it less sensitive to noise

int main(void) {
    timer_init();
    lcd_init();
    ping_init();

    char buffer1[32], buffer2[32], buffer3[32];
    float dist = 0, last_dist = -1;
    uint32_t pulse = 0, last_pulse = 0;
    int invalid_count = 0;
    int displaying_object = 0;

    while (1) {
        dist = ping_getDistance(&pulse);

        if (dist > 0) {
            float echo_time_us = pulse / 16.0f;

            // Only update display if values change meaningfully
            if (!displaying_object ||
                fabs(dist - last_dist) > STABILITY_THRESHOLD_CM ||
                abs((int)pulse - (int)last_pulse) > STABILITY_THRESHOLD_PULSE) {

                lcd_clear();
                sprintf(buffer1, "Dist: %.2f cm", dist);
                sprintf(buffer2, "Pulse: %lu", pulse);
                sprintf(buffer3, "Echo: %.2f us", echo_time_us);
                lcd_printf("%s\n%s\n%s", buffer1, buffer2, buffer3);

                last_dist = dist;
                last_pulse = pulse;
                displaying_object = 1;
            }

            invalid_count = 0; // Reset after good reading
        } else {
            invalid_count++;
            if (invalid_count >= INVALID_LIMIT && displaying_object) {
                lcd_clear();
                lcd_printf("No Object");
                displaying_object = 0;
                last_dist = -1;
                last_pulse = 0;
            }
        }

        timer_waitMillis(200);
    }
}
