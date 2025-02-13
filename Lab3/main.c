/// Simple 'rotating banner' program
/**
 * This program prints a looping message to the LCD screen
 * @author Garrett Brix
 * @date 06/26/2012

 */

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "stdio.h"

void sendString (char x) {
    int i;
    char buffer[50];
    sprintf(buffer, "%c was sent \n", x);
    for (i = 0; i < strlen(buffer) - 1; i++){
        cyBot_sendByte(buffer[i]);
    }
}

void main () {
    //Initializations
    //oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    //oi_init(sensor_data); // do this only once at start of main()

    timer_init();   //Initialize timer for the LCD screen
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    //cyBot_uart_init(); //Initialize UART communication

    //Initialize scan struct
    cyBOT_init_Scan(0b0110);
    //cyBOT_SERVO_cal();
    right_calibration_value = 280000;
    left_calibration_value = 1204000;
    cyBOT_Scan_t scan;


    int i;
    for (i=0; i <= 180; i+=2){
        cyBOT_Scan(i, &scan);
    }


   //turn off for charging
    //oi_free(sensor_data);
}

