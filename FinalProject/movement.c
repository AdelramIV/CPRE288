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
#include "uart.h"

double move_forward (oi_t*sensor_data, double distance_mm){
    double sum = 0; // Tracks distance travelled
    oi_setWheels(200,200); // Move forward full speed
//    char *str = malloc(20);

    while(sum < distance_mm) { // Checks if travelled given distance
        oi_update(sensor_data);
        sum += sensor_data -> distance; //pointer notation
        lcd_printf("%f", sum);
//        sprintf(str, "%d %d %d %d\n", sensor_data->cliffLeftSignal, sensor_data->cliffFrontLeftSignal, sensor_data->cliffRightSignal, sensor_data->cliffFrontRightSignal);
//        uart_sendStr(str);
        //If bump or cliff detected, stop and send message
        if(sensor_data->bumpLeft){
            uart_sendStr("Bump Detected Left\n");
            oi_setWheels(0,0);
            break;
        }
        if(sensor_data->bumpRight){
            uart_sendStr("Bump Detected Right\n");
            oi_setWheels(0,0);
            break;
        }
        if (sensor_data->lightBumperCenterLeft)
        {
            uart_sendStr("Bump Detected Center Left\n");
            oi_setWheels(0, 0);
            break;
        }
        if (sensor_data->lightBumperCenterRight)
        {
            uart_sendStr("Bump Detected Center Right\n");
            oi_setWheels(0, 0);
            break;
        }
        if(sensor_data->cliffLeftSignal > 2000 | sensor_data->cliffLeftSignal < 500){
            int signal = sensor_data->cliffLeftSignal;
            if (signal > 2000)
            {
                uart_sendStr("Cliff Detected Left\n");
            }
            else if (signal < 500)
            {
                uart_sendStr("Hole Detected Left\n");
            }
            oi_setWheels(0,0);
            timer_waitMillis(500);
            break;
        }
        if(sensor_data->cliffFrontLeftSignal > 2000 | sensor_data->cliffFrontLeftSignal < 500){
            int signal = sensor_data->cliffFrontRightSignal;
            if (signal > 2000)
            {
                uart_sendStr("Cliff Detected Front Left\n");
            }
            else if (signal < 500)
            {
                uart_sendStr("Hole Detected Front Left\n");
            }

            oi_setWheels(0,0);
            timer_waitMillis(500);
            break;
        }
        if(sensor_data->cliffRightSignal > 2000 | sensor_data->cliffRightSignal < 500){
            int signal = sensor_data->cliffRightSignal;
            if (signal > 2000)
            {
                uart_sendStr("Cliff Detected Right\n");
            }
            else if (signal < 500)
            {
                uart_sendStr("Hole Detected Right\n");
            }

            oi_setWheels(0,0);
            timer_waitMillis(500);
            break;
        }
        if(sensor_data->cliffFrontRightSignal > 2000 | sensor_data->cliffFrontRightSignal < 500){
            int signal = sensor_data->cliffFrontRightSignal;
            if (signal > 2000)
            {
                uart_sendStr("Cliff Detected Front Right\n");
            }
            else if (signal < 500)
            {
                uart_sendStr("Hole Detected Front Right\n");
            }
            oi_setWheels(0,0);
            timer_waitMillis(500);
            break;
        }
//        if(sensor_data->bumpLeft){
//            oi_setWheels(0,0);
//            collision_backup_left(sensor_data);
//            sum -= 150;
//            oi_setWheels(200,200);
//        }
//        else if(sensor_data->bumpRight){
//            oi_setWheels(0,0);
//            collision_backup_right(sensor_data);
//            sum -= 150;
//            oi_setWheels(200,200);
//        }
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
    while(currDegree * -1 < degrees){
        oi_update(sensor);
        currDegree += sensor -> angle; //pointer notation
        lcd_printf("%f", currDegree);

        //If cliff detected, stop moving and send message
        if(sensor->cliffLeft){
                    uart_sendStr("Cliff Detected Left\n");
                    oi_setWheels(0,0);
                    break;
                }
                if(sensor->cliffFrontLeft){
                    uart_sendStr("Cliff Detected Front Left\n");
                    oi_setWheels(0,0);
                    break;
                }
                if(sensor->cliffRight){
                    uart_sendStr("Cliff Detected Right\n");
                    oi_setWheels(0,0);
                    break;
                }
                if(sensor->cliffFrontRight){
                    uart_sendStr("Cliff Detected Front Right\n");
                    oi_setWheels(0,0);
                    break;
                }
    }

    oi_setWheels(0,0); // stop
    timer_waitMillis(600);

    return degrees;
}

//Turn left a given degree
double turn_left(oi_t *sensor, double degrees) {
    oi_setWheels(200, -200);
    double currDegree = 0;
    while(currDegree < degrees){
        oi_update(sensor);
        currDegree += sensor -> angle;
        lcd_printf("%f", currDegree);

        //If cliff detected, stop moving and send message
        if(sensor->cliffLeft){
                    uart_sendStr("Cliff Detected Left\n");
                    oi_setWheels(0,0);
                    break;
                }
                if(sensor->cliffFrontLeft){
                    uart_sendStr("Cliff Detected Front Left\n");
                    oi_setWheels(0,0);
                    break;
                }
                if(sensor->cliffRight){
                    uart_sendStr("Cliff Detected Right\n");
                    oi_setWheels(0,0);
                    break;
                }
                if(sensor->cliffFrontRight){
                    uart_sendStr("Cliff Detected Front Right\n");
                    oi_setWheels(0,0);
                    break;
                }
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
