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
#include "uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1
#include "cyBot_uart.h"
#include "movement.h"
#include "open_interface.h"
#include "stdio.h"
#include "math.h"

int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();

	char buffer[50];
	char dataArr[20] = "                    ";
	int numChar = 0;
	char curChar;
	int i;
	
	//Infinite loop
	while(1){
	    //While we havent received 20 characters in a row
	    while(numChar != 20){
	        //Waiting for input
	        curChar = uart_receive();
	        //If input is enter, send data and reset
            if(curChar == '\r'){
                sprintf(buffer, "%c \n\r", curChar);
                uart_sendChar('\n');
                break;
            }
            //update array and return character to putty
	        dataArr[numChar] = curChar;
	        sprintf(buffer, "%c \n\r", curChar);
	        uart_sendChar(curChar);
	        numChar++;
	    }
	    numChar = 0;
	    lcd_clear();
	    lcd_puts(dataArr);
	    for(i = 0; i < sizeof(dataArr); i++){
	        dataArr[i] = ' ';
	    }
	}
}
