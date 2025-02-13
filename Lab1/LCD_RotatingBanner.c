/// Simple 'rotating banner' program
/**
 * This program prints a looping message to the LCD screen
 * @author Garrett Brix
 * @date 06/26/2012

 */

#include "Timer.h"
#include "lcd.h"

void shiftLeft(char data[]){
    int i = 0;
	for(i = 0; i < strlen(data) - 1; i++){
		data[i] = data[i+1];
	}
}

void lcd_RotatingBanner(char data[]){
    char newData[20] = "                    ";
	int i = 0;
    int k;

	//Infinite loop
	while(true){
	    //Adds the current character of given string to end of the line
	    if(i < strlen(data)){
	        newData[19] = data[i];
	    }
	    //if the end of the given string is reached, reset to blanks
	    else{
	        newData[19] = ' ';
	        if(newData[0] == ' '){
	            //check if printed line is all blank
	            for(k = 0; k < 20; k++){
	                if(newData[k] != ' '){
	                    break;
	                }
	            }
	            if(k == 20){
	                i = -1;
	            }
	        }
	    }

	    //print LCD text
		lcd_printf("%s", newData);
		timer_waitMillis(300);
		i++;

		shiftLeft(newData);
	}
}

int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 
	lcd_RotatingBanner("Microcontrollers are awesome!");
	
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}
