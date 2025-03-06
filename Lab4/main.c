/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1
#include "movement.h"
#include "open_interface.h"
#include "stdio.h"
#include "math.h"

void sendString (char string[50]) {
    int i;
    char buffer[50];
    sprintf(buffer, "%s \n\r", string);
    for (i = 0; i < strlen(buffer) - 1; i++){
        cyBot_sendByte(buffer[i]);
    }
}

int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	cyBot_uart_init();

	char string[50] = " ";
	int curBut = 0;
	
	while(1)
	{
	    while(button_getButton() == curBut){}
	    switch (button_getButton()) {
		case 0:
		    break;

	        case 1:
	            curBut = 1;
	            strcpy(string, "hello world");
	            sendString(string);
	            break;

	        case 2:
	            curBut = 2;
	            strcpy(string, "hotdog");
	            sendString(string);
	            break;

	        case 3:
	            curBut = 3;
	            strcpy(string, "Pemdas");
	            sendString(string);
	            break;

	        case 4:
	            curBut = 4;
	            strcpy(string, "CPRE");
	            sendString(string);
	            break;
	        default:
	            break;
	        }


	}

}
