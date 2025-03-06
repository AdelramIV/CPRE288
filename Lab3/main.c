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
#include "math.h"

void sendString (int objectNum, float angle, float distance, int objectWidth) {
    int i;
    char buffer[50];
    sprintf(buffer, "Object %d: Angle: %0.3f Distance: %0.3f Width: %d \n\r", objectNum, angle, distance, objectWidth);
    for (i = 0; i < strlen(buffer) - 1; i++){
        cyBot_sendByte(buffer[i]);
    }
}

float smallestWidth (float data[90][2]){
    int i = 0;
    float minWidth = 180.0; // current minimun width of all obejects
    float curWidth = 0.0;  // Width of current object scanning
    float range = data[0][1]; //comparison value to check distacne between 2 objects
    float minDegrees; //middle of the Smallest in degrees
    int objectNum = 0;
    float midAngle; //angle of a generic object
    float midDistance; //Distance of a generic object

    for(i = 1; i < 90; i++){
        if(data[i][1] > 50.0){ //Base case for far away object
            range = data[i][1];
            continue;
        }
        if(fabs(data[i][1] - range) < 2){ // if current distance - old object distance is different, then its part of the same object
            curWidth += 2;
        }
        else{ //if the distance is greater than 2 then mark & compare objects
            range = data[i][1]; //New Range for later comparison

            if(curWidth > 8){
                midAngle = (i * 2) - (curWidth / 2); // Find remainder
                midDistance = midAngle - ((int)midAngle % 2); //Subtract to find the middle
                sendString(objectNum, midAngle, data[(int)midDistance/2][1], (int)curWidth); //finds middle
                objectNum++; //object number

                if(curWidth < minWidth){ //compares widths
                    minWidth = curWidth;
                    minDegrees = midAngle;
                }
            }

            curWidth = 0.0; //resets

        }
    }

    return minDegrees;
}

void main () {
    //Initializations
    //oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    //oi_init(sensor_data); // do this only once at start of main()

    timer_init();   //Initialize timer for the LCD screen
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    cyBot_uart_init(); //Initialize UART communication

    //Initialize scan struct
    cyBOT_init_Scan(0b0111);
    //cyBOT_SERVO_cal();
    right_calibration_value = 337750;
    left_calibration_value = 1330000;
    cyBOT_Scan_t scan;

    char myChar = ' ';
    int i;

    while(myChar != 'm'){
        myChar = cyBot_getByte();
    }


    float dataArray[90][2];
    int j = 0;
    for(i = 0; i <= 180; i+=2){
        cyBOT_Scan(i, &scan);
        float distance = scan.sound_dist;
        //sendString(i , distance);
        dataArray[j][0] = i;
        dataArray[j][1] = distance;
        j++;

    }


    cyBOT_Scan(smallestWidth(dataArray), &scan);


   //turn off for charging
    //oi_free(sensor_data);
}

