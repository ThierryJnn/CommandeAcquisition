/*
 * startMotor.c
 *
 *  Created on: Oct 3, 2023
 *      Author: thier
 */

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

extern int Power;
void startMotor() {
    // Activez l'étage de puissance du moteur (code pour activer le moteur)
	if (Power==0){
		Power=1;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1024-512);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,512);
		// Affichez le message "Power ON" dans la console
		HAL_UART_Transmit(&huart2, "Power ON\r\n", strlen("Power ON\r\n"), HAL_MAX_DELAY);
		return 0;
	}
	else {
	}
    HAL_UART_Transmit(&huart2, "Moteur déjà allumé\r\n", strlen("Moteur déjà allumé\r\n"), HAL_MAX_DELAY);
}
