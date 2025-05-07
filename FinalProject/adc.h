/*
 * adc.h
 *
 *  Created on: Mar 27, 2025
 *      Author: gbrix
 */

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>

//ADC initialization for module 0
void adc_init (void);

//ADC sample taking function
uint16_t adc_read (void);

#endif /* ADC_H_ */
