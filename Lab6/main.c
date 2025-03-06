/**
 *
 *
 * Template file for CprE 288 lab 6
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

//#include "Timer.h"
//#include "lcd.h"
//#include "uart.h"  // Functions for communicating between CyBot and Putty (via UART)
//                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1
//#include "cyBot_uart.h"
//#include "cyBot_Scan.h"
//#include "stdio.h"
//#include "math.h"
//#include <stdbool.h>
//#include "driverlib/interrupt.h"
//
//
//int main(void) {
//	timer_init(); // Must be called before lcd_init(), which uses timer functions
//	lcd_init();
//	uart_init();
//	cyBOT_init_Scan(0b0111);
//	//cyBOT_SERVO_cal();
//	//return 0;
//    right_calibration_value = 301000;
//    left_calibration_value = 1300000;
//	cyBOT_Scan_t scan;
//	char myChar = ' ';
//	int i;
//
//	//Infinite loop
//	    while(myChar != 'g'){
//	            myChar = uart_receive_nonBlocking();
//	    }
//
//	   for(i = 0; i <= 180; i+=2){
//	       cyBOT_Scan(i, &scan);
//	       if(uart_receive_nonBlocking() == 's'){
//	           break;
//	       }
//	   }
//}
