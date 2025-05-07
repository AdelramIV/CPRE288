#ifndef SCAN_H_
#define SCAN_H_



// Scan value
typedef struct{
    float sound_dist;  // Distance from Ping Sensor (Scan returns -1.0 if PING is not enabled)
    int IR_raw_val;    // Raw ADC value from IR sensor (Scan return -1 if IR is not enabled)
} Scan_t;



// Name: init_Scan
// Input: feature, int where each feature is associated with a bit as follows
//  Bit 0: 1 enable servo
//  Bit 1: 1 enable PING
//  Bit 2: 1 enable IR
//  Bit 3 - 31: Reserved (set to 0's)
// Example: init_Scan(0b0111); // Enables servo, PING, and IR
void scan_init(int feature);



// Point sensors to angle, and get a scan value
// Input Parameters:
// angle: Direction to point the Sensors for taking a measurement
// getScan : The location of a declared Scan_t structure
// NOTE 1: If PING is not enabled, then getScan.sound_dist returns -1.0
// NOTE 2: If IR is not enabled, then getScan.IR_raw_val returns -1
void scan(int angle, Scan_t* getScan);







////////////////////////////////////////////
//// Calibrate physical CyBOT            ///
////////////////////////////////////////////

// These will be different for each physical CyBOT servo
// where 0 degrees (right), and 180 degrees (left) is located
// These values can be found by running the servo_calibrate function.
// Once you know the values, then you can set them in main().
int right_calibration_value;
int left_calibration_value;



// Servo Calibrate value struct
typedef struct{
    int right;  // Right (0 degree) calibration value
    int left;   // Left (180 degree) calibration value
} SERVRO_cal_t;

#endif /* SCAN_H_ */
