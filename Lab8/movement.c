/*
 * movement.c
 *
 *  Created on: Feb 6, 2025
 *      Author: gbrix
 */

#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "timer.h"

double move_forward (oi_t*sensor_data, double distance_mm){
    double sum = 0; // Tracks distance travelled
    oi_setWheels(200,200); // Move forward full speed

    while(sum < distance_mm) { // Checks if travelled 1 meter
        oi_update(sensor_data);
        sum += sensor_data -> distance; //pointer notation
        lcd_printf("%f", sum);
        if(sensor_data->bumpLeft){
            oi_setWheels(0,0);
            collision_backup_left(sensor_data);
            sum -= 150;
            oi_setWheels(200,200);
        }
        else if(sensor_data->bumpRight){
            oi_setWheels(0,0);
            collision_backup_right(sensor_data);
            sum -= 150;
            oi_setWheels(200,200);
        }
    }

    oi_setWheels(0,0); // stop
    timer_waitMillis(300);

    return sum;
}

double move_backward (oi_t*sensor_data, double distance_mm){
    double sum = 0; // Tracks distance travelled
    oi_setWheels(-200,-200); // Move forward full speed

    while(sum * -1 < distance_mm) { // Checks if travelled 1 meter
        oi_update(sensor_data);
        sum += sensor_data -> distance; //pointer notation
        lcd_printf("%f", sum);
    }

    oi_setWheels(0,0); // stop
    timer_waitMillis(300);

    return sum;
}

//Turn right a given degree
double turn_right(oi_t *sensor, double degrees) {
    oi_setWheels(-200, 200);
    double currDegree = 0;
    while(currDegree * -1 < degrees - 20){
        oi_update(sensor);
        currDegree += sensor -> angle; //pointer notation
        lcd_printf("%f", currDegree);
    }

    oi_setWheels(0,0); // stop
    timer_waitMillis(600);

    return degrees;
}

//Turn left a given degree
double turn_left(oi_t *sensor, double degrees) {
    oi_setWheels(200, -200);
    double currDegree = 0;
    while(currDegree < degrees - 20){
        oi_update(sensor);
        currDegree += sensor -> angle;
        lcd_printf("%f", currDegree);
    }
    oi_setWheels(0,0); // stop
    timer_waitMillis(600);

    return degrees;
}

void collision_backup_left(oi_t *sensor){
    move_backward(sensor, 150);
    turn_right(sensor, 90);
    move_forward(sensor, 250);
    turn_left(sensor, 90);
}

void collision_backup_right(oi_t *sensor){
    move_backward(sensor, 150);
    turn_left(sensor, 90);
    move_forward(sensor, 250);
    turn_right(sensor, 90);
}


//move in a square
void drive_square(oi_t *sensor_data){
    move_forward(sensor_data, 500);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 500);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 500);
    turn_right(sensor_data, 90);
    move_forward(sensor_data, 500);
}

