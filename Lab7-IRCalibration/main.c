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
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "stdio.h"
#include "math.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

typedef struct {
    int leftAng;
    int rightAng;
    float midpoint;
    float dist;

} object;

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
	cyBOT_init_Scan(0b0111);
//	cyBOT_SERVO_cal();
//	return 0;
    right_calibration_value = 238000;
    left_calibration_value = 1219750;

//	right_calibration_value = 327250; //bot 8
//	left_calibration_value = 1366750;

	cyBOT_Scan_t scan;
	char myChar = ' ';
	int i;
	char str[50];
	int j = 0;

	 while(myChar != 'g'){
	       myChar = uart_receive_nonBlocking();
	 }

	 int IRArray[90];

	 for(i = 0; i < 180; i+=2){
	     cyBOT_Scan(i, &scan);
	     IRArray[j] = scan.IR_raw_val;

	     sprintf(str, "%d. IR: %d", i, IRArray[j]);
	     sendString(str);
	     j++;
	 }





}
