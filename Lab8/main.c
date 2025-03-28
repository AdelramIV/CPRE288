 /*
 *
 *
 *  for CprE 288 lab 6
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "Timer.h"
#include "lcd.h"
#include "uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1
#include "cyBot_Scan.h"
#include "stdio.h"
#include "math.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "adc.h"


void sendString (char string[50]) {
    int i;
    char buffer[50];
    sprintf(buffer, "%s \n\r", string);
    for (i = 0; i < strlen(buffer) - 1; i++){
        uart_sendChar(buffer[i]);
    }
}

int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();
	adc_init();
	cyBOT_init_Scan(0b0111);
//	cyBOT_SERVO_cal();
//	return 0;
    right_calibration_value = 238000;
    left_calibration_value = 1235500;

//	right_calibration_value = 327250; //bot 8
//	left_calibration_value = 1366750;

	cyBOT_Scan_t scan;
	char myChar = ' ';
	int i;
	int sample = 0;
	float distance = 0;
	int IRArray[5];
	float IRavg;



	 cyBOT_Scan(90, &scan);

	 while(true){
	     sample = adc_read();

	     while(myChar != 'g'){
	            myChar = uart_receive();
	      }

	     for(i = 0; i < 5; i++){
	         IRArray[i] = adc_read();
	     }
	     i = 0;
	     IRavg = (IRArray[0] + IRArray[1] +IRArray[2] + IRArray[3] + IRArray[4]) / 5.0;

	     distance = (31856822.642170)*pow(IRavg, -1.924221);
	     lcd_printf("Sample: %0.3f Distance: %0.3f", IRavg, distance);
	     myChar = ' ';

//	     timer_waitMillis(2000);
	 }





}
