/*
 * movement.h
 *
 *  Created on: Feb 6, 2025
 *      Author: gbrix
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"
#include "lcd.h"

double move_forward (oi_t*sensor_data, double distance_mm);
double move_backward (oi_t*sensor_data, double distance_mm);

double turn_right(oi_t *sensor, double degrees);
double turn_left(oi_t *sensor, double degrees);

void collision_backup_left(oi_t *sensor);
void collision_backup_right(oi_t *sensor);

void drive_square(oi_t *sensor_data);


#endif /* MOVEMENT_H_ */
