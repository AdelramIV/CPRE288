/**
 * @file FinalProject.c
 * @author Team K-4
 *
 */

#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "servo.h"
#include <stdint.h>
#include "movement.h"
#include "open_interface.h"
#include "uart.h"
#include <string.h>
#include "adc.h"
#include "ping.h"
#include "scan.h"

//#define OBJ_START 60
//#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
//#define STABILITY_THRESHOLD_CM 0.5
//#define STABILITY_THRESHOLD_PULSE 100
//#define INVALID_LIMIT 5
#define PI 3.14592653589793

//typedef struct
//{
//    int number;
//    int initial_angle;
//    float distance;
//    int ang_width;
//    int arc_len;
//} object_t;

typedef struct {
    int leftAng;
    int rightAng;
    int midpoint;
    float dist;
    float linearWidth;

} object;

//float ir_scan(int angle){
//    float distance = 0;
//    int IRArray[5];
//    float IRavg;
//    int l = 0;
//
//    servo_move(angle);
//    for (l = 0; l < 5; l++)
//    {
//        IRArray[l] = adc_read();
//    }
//    l = 0;
//    IRavg = (IRArray[0] + IRArray[1] + IRArray[2] + IRArray[3] + IRArray[4])
//            / 5.0;
//
//    distance = (13974351.2214)*pow(IRavg, -1.4977) / 100 + 8; //1318-6
//
//    return distance;
//}

float ping_scan(int angle){
    float distance = 0;
    uint32_t pulse = 0;
//            , last_pulse = 0;
    servo_move(angle);
    timer_waitMillis(100);
    distance = ping_getDistance(&pulse);
//    last_pulse = pulse;
    return distance;
}

float calculateWidth(float distance, int left, int right) {
//    float thetaL = left * (PI/180.0);
//    float thetaR = right * (PI/180.0);
//
//    float width = distance * fabs((tan(thetaL) - tan(thetaR))/2);
//    return fabs(width);
    // Convert angles from degrees to radians
    float thetaL = left * (PI / 180.0);
    float thetaR = right * (PI / 180.0);

    // Calculate the horizontal distances at the left and right angles
    float xL = distance * tan(thetaL);  // Horizontal position at left angle
    float xR = distance * tan(thetaR);  // Horizontal position at right angle

    // Calculate the width as the difference in horizontal positions
    float width = fabs(xR - xL);  // Ensure positive width
    return width;
}

void scanObjects()
{
    int i = 0;
    int k = 0;
    int numObj = 0;
    object objArr[15];
    bool inObject = false;
    int IRArray[5];
    float IRavg;
    char str[100];

    servo_move(0);
    timer_waitMillis(500);
    //Scan 180 at 2 degrees per tick
    for (i = 0; i <= 180; i += 2)
    {
        servo_move(i);
        timer_waitMillis(50);
        //scan multiple times and avg IR value
        for (k = 0; k < 5; k++)
        {
            IRArray[k] = adc_read();
        }
        k = 0;
        IRavg = (IRArray[0] + IRArray[1] + IRArray[2] + IRArray[3] + IRArray[4])
                / 5.0;

        //start of object
        if (IRavg > (900) && !inObject)
        {
            objArr[numObj].rightAng = i;
            inObject = true;
        }

        //end of object
        else if (IRavg <= (900) && inObject || i >= 180 && inObject)
        {
            objArr[numObj].leftAng = i - 2;
            if (objArr[numObj].leftAng != objArr[numObj].rightAng)
            {
                objArr[numObj].midpoint = (objArr[numObj].leftAng
                        + objArr[numObj].rightAng) / 2;
                numObj++;
            }
            inObject = false;
        }
    }

    //Find distance to midpoint of each object
    for (i = 0; i < numObj; i++)
    {
        objArr[i].dist = ping_scan(objArr[i].midpoint);
        if (objArr[i].dist <= 0) {
            continue;
        }
        objArr[i].linearWidth = calculateWidth(objArr[i].dist, objArr[i].leftAng, objArr[i].rightAng);
    }

    if (numObj > 15) {
        uart_sendStr("Error: Too many objects detected.\n");
        return;
    }
    //output data to putty
    uart_sendStr("\nObject# | Angle | Distance | Width |\n");
    for (i = 0; i < numObj; i++)
    {
        sprintf(str, "     %d: Angle: %d Distance: %0.2f Width: %0.2f\n", i, (int)objArr[i].midpoint,
                objArr[i].dist, objArr[i].linearWidth);
        uart_sendStr(str);
    }
}

int main(void) {
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    timer_init();
    lcd_init();
    servo_init();
    adc_init();
    uart_init();
    ping_init();

    char command;

    servo_move(0);

    while(1){
        //Update command input
        command = uart_receive();

        //Action based on command
        switch(command){
            case 'q':
                //Scan front 180 with IR and PING
                uart_sendChar(command);

                //Create Objects and output to putty
                scanObjects();
                break;
            case 'w':
                //Move forward
                uart_sendChar(command);
                move_forward(sensor_data, 20);
                break;
            case 's':
                //Move backward
                uart_sendChar(command);
                move_backward(sensor_data, 20);
                break;
            case 'a':
                //Turn left
                uart_sendChar(command);
                turn_left(sensor_data, 20);
                break;
            case 'd':
                //Turn right
                uart_sendChar(command);
                turn_right(sensor_data, 20);
                break;
            case 'e':
                //Exit and close
                uart_sendChar(command);
                oi_free(sensor_data);
                return 0;
            default:
                break;
        }
    }
}

//char *str = malloc(20);
//    // Sets servo position to 0 deg
//    ir_scan(0, scanner);
//    timer_waitMillis(200);
//    // Scan for initial distance
//    ir_scan(0, scanner);
//    int obj_tracking = 0;
//
//    // object variables
//    object_t objects[5];
//    int object_number = 0;
//
//    // keeping track of past variables
//    // [0] is current, [5] is past
//    int distances[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//
//    // Start scanning algo
//    int k = 0;
//    int i;
//    for (k = 0; k < 180; k += 2)
//    {
//        // Scan
//        ir_scan(k, scanner);
//
//        // only read in from less than 2m away
//        // read in current distance
//        scanner->IR_raw_val = MIN(scanner->IR_raw_val, 200.0);
//        distances[0] = scanner->IR_raw_val;
//        sprintf(str, "%d", distances[0]);
//        uart_sendStr(str);
//
//        int result = distances[0] + distances[1] + distances[2] + distances[3]
//                + distances[4] - distances[5] - distances[6] - distances[7]
//                - distances[8] - distances[9];
//
//        // check for beginning of object (if filter conv array is big negative)
//        if (result < -OBJ_START && !obj_tracking)
//        {
//            // update info for obj
//            objects[object_number].number = object_number;
//            objects[object_number].initial_angle = k - 6;
//            objects[object_number].distance = distances[1];
//            obj_tracking = 1; // obj started
//        }
//        // check for end of object (if filter conv array is big positive and we are tracking an obj)
//        else if (result > OBJ_START && obj_tracking)
//        {
//            objects[object_number].ang_width = k - 8
//                    - objects[object_number].initial_angle;
//            objects[object_number].arc_len = objects[object_number].ang_width
//                    * objects[object_number].distance * 3.14 / 180.0;
//            object_number = MIN(object_number + 1, 5);
//            obj_tracking = 0;
//        }
//
//        //Sets the previous distances
//        distances[9] = distances[8];
//        distances[8] = distances[7];
//        distances[7] = distances[6];
//        distances[6] = distances[5];
//        distances[5] = distances[4];
//        distances[4] = distances[3];
//        distances[3] = distances[2];
//        distances[2] = distances[1];
//        distances[1] = distances[0];
//     }
