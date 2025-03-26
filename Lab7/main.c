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
    int midpoint;
    float dist;

} object;

//float IRRawToDist(int raw){
//    float dist = 0;
//    return dist;
//}

void sendString (int objectNum, int midpoint, float distance) {
    int i;
    char buffer[50];
    sprintf(buffer, "Object %d: Midpoint: %d Distance: %0.3f \n\r", objectNum, midpoint, distance);
    for (i = 0; i < strlen(buffer) - 1; i++){
        cyBot_sendByte(buffer[i]);
    }
}

int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();
	cyBOT_init_Scan(0b0111);
//	cyBOT_SERVO_cal();
//	return 0;
//    right_calibration_value = 301000; 1318-10
//    left_calibration_value = 1288000;

	right_calibration_value = 232750; //1318-10
	left_calibration_value = 1193500;

	cyBOT_Scan_t scan;
	char myChar = ' ';
	int i;

	 while(myChar != 'g'){
	       myChar = uart_receive_nonBlocking();
	 }

	 int IRArray[3];
	 float dataArray[90][2];
	 float IRavg;
	 int curWidth = 0;
	 int minWidth = 180;
	 int minDegrees = 0;
	 int j, k = 0;
	 int numObj = 0;
	 object objArr[10];
	 bool inObject = false;


	 //Scan 180 at 2 degrees per tick
	 for(i = 0; i <= 180; i+=2){

	      //scan multiple times and avg IR value
	      for(k = 0; k < 3; k++){
	          cyBOT_Scan(i, &scan);
	          IRArray[k] = scan.IR_raw_val;
	      }
	      k = 0;
	      IRavg = (IRArray[0] + IRArray[1] +IRArray[2]) / 3.0;

	      //store data
	      dataArray[j][0] = i;
	      dataArray[j][1] = IRavg;;
	      j++;

	      //start of object
	      if(IRavg > 1000 && !inObject){
	          objArr[numObj].rightAng = i;
	          inObject = true;

	      }
	      //end of object
	      else if(IRavg < 1000 && inObject){
	          objArr[numObj].leftAng = i - 2;
	          objArr[numObj].midpoint = (objArr[numObj].leftAng + objArr[numObj].rightAng) / 2;
	          numObj++;
	          inObject = false;
	      }
	 }

//	 //Create objects based on IR scans
//	 int range = dataArray[0][1];
//	 for(i = 0; i < 90; i++ ){
//
//	     //If outside of threshold range, skip
//	     if(dataArray[i][1] < 1000){
//	         range = dataArray[i][1];
//	         continue;
//
//	     }
//
//	     // if current distance - old object distance is different, then its part of the same object
//	     if(fabs(dataArray[i][1] - range) < 50){
//	                 curWidth += 2;
//	                 range = dataArray[i][1];
//	     }
//
//	     //if the distance is greater than 65 then mark & compare objects
//	     else{
//	         range = dataArray[i][1]; //New Range for later comparison
//
//	         //create new object
//	         objArr[numObj].midpoint = (i * 2) - (curWidth / 2); //finds middle angle of object
//	         objArr[numObj].leftAng = i;
//	         objArr[numObj].rightAng = i - curWidth;
//
//
//	         //compares smallest object
//	         if(curWidth < minWidth){ //compares widths
//	                  minWidth = curWidth;
//	                  minDegrees = objArr[numObj].midpoint;
//	         }
//	         curWidth = 0; //resets
//	         numObj++; //object number
//	      }
//
//	 }

	 //Scan distance to each object and output data
	 for(i = 0; i < numObj; i++){
	     cyBOT_Scan(objArr[i].midpoint, &scan);
	     objArr[i].dist = scan.sound_dist;
	     sendString(i + 1, objArr[i].midpoint, objArr[i].dist);
	 }


}
