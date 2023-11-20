/*
 * mesure.c
 *
 *  Created on: Nov 20, 2023
 *      Author: thier
 */

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

uint32_t old_speed, new_speed, freq;
char vitesse[30];

void mesureSpeed(void){
	int i=0;
	for(i=0;i<30;i++){
		old_speed=__HAL_TIM_GET_COUNTER(&htim3);
		HAL_Delay(10);
		new_speed=__HAL_TIM_GET_COUNTER(&htim3);
		freq=(new_speed-old_speed)*735/495;
		sprintf(vitesse, "Vitesse : %d tpm \r\n", freq);
		HAL_UART_Transmit(&huart2, vitesse, strlen(vitesse), HAL_MAX_DELAY);
	}

}
