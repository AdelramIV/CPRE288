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

void main () {
    //Initializations
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    timer_init();   //Initialize timer for the LCD screen
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    move_forward(sensor_data, 2000);


    //turn off for charging
    oi_free(sensor_data);
}
