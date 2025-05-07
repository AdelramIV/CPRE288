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
#include "movement.h"
#include "open_interface.h"

#define PI 3.14592653589793

typedef struct {
    int leftAng;
    int rightAng;
    int midpoint;
    float dist;
    float linearWidth;

} object;

float calculateWidth(float distance, int left, int right) {
    float thetaL = left * (PI/180.0);
    float thetaR = right * (PI/180.0);

    float width = distance * fabs((tan(thetaL) - tan(thetaR))/2);
    return fabs(width);
}

void sendString (int objectNum, int midpoint, float distance, float linearWidth) {
    int i;
    char buffer[100];
    sprintf(buffer, "Object %d: Midpoint: %d | Distance: %0.3f | Linear Width: %0.3f \n\r", objectNum, midpoint, distance, linearWidth);
    for (i = 0; i < strlen(buffer) - 1; i++){
        uart_sendChar(buffer[i]);
    }
}

object scanObjects() {
    cyBOT_Scan_t scan;

    int IRArray[3];
//   float dataArray[91][2];
    float IRavg;
//    float prevIR = 950;
//   int curWidth = 0;
//   int minWidth = 180;
//   int minDegrees = 0;
    int i, k = 0;
    int numObj = 0;
    object objArr[15];
    bool inObject = false;
    bool objectExists = false;

    //Scan 180 at 2 degrees per tick
    for(i = 0; i <= 45; i+=1){

         //scan multiple times and avg IR value
         for(k = 0; k < 3; k++){
             cyBOT_Scan(i, &scan);
             IRArray[k] = scan.IR_raw_val;
         }
         k = 0;
         IRavg = (IRArray[0] + IRArray[1] +IRArray[2]) / 3.0;

//        //store data
//        dataArray[j][0] = i;
//        dataArray[j][1] = IRavg;;
//        j++;
         //sendString(i, 1, IRavg, 1.1);

         //start of object
         if(IRavg > (1000) && !inObject){
             objArr[numObj].rightAng = i;
             inObject = true;
             objectExists = true;
         }

         //end of object
         else if(IRavg < (1000) && inObject){
             objArr[numObj].leftAng = i - 2;
             objArr[numObj].midpoint = (objArr[numObj].leftAng + objArr[numObj].rightAng) / 2;
             numObj++;
             inObject = false;
         }
//        prevIR = IRavg;
    }

    //Scan distance to each object and output data
    for(i = 0; i < numObj; i++){
        cyBOT_Scan(objArr[i].midpoint, &scan);
        objArr[i].dist = scan.sound_dist;
        objArr[i].linearWidth = calculateWidth(objArr[i].dist, objArr[i].leftAng, objArr[i].rightAng);
        sendString(i + 1, objArr[i].midpoint, objArr[i].dist, objArr[i].linearWidth);
    }


    if (objectExists == true){
        int smallestNum = 0;
        for(i = smallestNum + 1; i < numObj; i++){
            if(objArr[smallestNum].linearWidth < objArr[i].linearWidth){
                continue;
            }
            else {
                smallestNum = i;
            }
        }
        return objArr[smallestNum];
    } else {
        objArr[0] = (object){0, 0, 0, 0.0, 0.0};
        return objArr[0];
    }
}

int main(void) {
    timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();
	cyBOT_init_Scan(0b0111);
//	oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
//	oi_init(sensor_data); // do this only once at start of main()
//
//	cyBOT_SERVO_cal();
//	return 0;

	right_calibration_value = 290500; //1318-8
	left_calibration_value = 1261750;

	char myChar = ' ';
	object smallestObj;
	while(myChar != 'g'){
	    myChar = uart_receive();
	}

	 smallestObj = scanObjects();
//	 while (smallestObj.dist == 0) { //if no objects exist, move forward
//
//	     move_forward(sensor_data, 250);
//	     smallestObj = scanObjects();
//	 }
//
//	 do {
//         if(smallestObj.midpoint > 100){ //if the smallest obj on the left side
//             turn_left(sensor_data, abs(smallestObj.midpoint - 90));
//             move_forward(sensor_data, (smallestObj.dist * 10) - 250);
//             smallestObj = scanObjects();
//         }
//         else if (smallestObj.midpoint < 80) { //if the smallest obj is on the left side
//             turn_right(sensor_data, abs(smallestObj.midpoint - 90));
//             move_forward(sensor_data, (smallestObj.dist * 10) - 250);
//             smallestObj = scanObjects();
//         }
//         else{ //move forward if it is
//             move_forward(sensor_data, (smallestObj.dist * 10) - 250);
//             smallestObj = scanObjects();
//         }
//	 } while (smallestObj.dist > 20);
//
//	 //turn off for charging
//	 oi_free(sensor_data);
}
