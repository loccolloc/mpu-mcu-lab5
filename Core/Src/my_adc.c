/*
 * my_adc.c
 *
 *  Created on: Dec 6, 2023
 *      Author: HP
 */

#include "my_adc.h"
#ifdef TEST
uint16_t phong = 85;
#endif

extern ADC_HandleTypeDef hadc2;
uint16_t adc_GetValue(void) {
	//return 13;
#ifdef TEST
	phong++;
	return phong%4096;
#else

	return HAL_ADC_GetValue(&hadc2)%4096;
#endif
}

void adc_init(void) {
	HAL_ADC_Start(&hadc2);
}
