#include "scan.h"
#include <stdint.h>
//#include "tm4c123gh6pm.h"
#include "Timer.h"       // Your custom delay/us_delay
#include "adc.h"         // For reading IR
#include "ping.h"        // For Ping sensor control
#include "servo.h"       // For Servo control

//int right_calibration_value = 1300;
//int left_calibration_value = 3000;
uint32_t* pulse;

void scan_init(int feature) {
    if (feature & 0x01) {
        servo_init();  // Init PWM and GPIO for servo
    }
    if (feature & 0x02) {
        ping_init();   // Init ping sensor (uses timer capture)
    }
    if (feature & 0x04) {
        adc_init();    // Init ADC (for IR)
    }
}

void scan(int angle, Scan_t* getScan) {
    servo_move(angle); // Move servo to desired angle

    // Read Ping distance
    getScan->sound_dist = ping_getDistance(pulse);  // Returns distance in cm

    // Read IR sensor
    getScan->IR_raw_val = adc_read();  // Raw ADC value
}

