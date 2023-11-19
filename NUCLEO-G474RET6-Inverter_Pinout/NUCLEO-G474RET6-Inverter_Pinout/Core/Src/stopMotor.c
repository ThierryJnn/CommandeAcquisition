/*
 * stopMotor.c
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
void stopMotor() {
    // Desactivez l'étage de puissance du moteur (code pour desactiver le moteur)
	if (Power==1){
			Power=0;
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,512);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1024-512);
			// Affichez le message "Power OFF" dans la console
			HAL_UART_Transmit(&huart2, "Power OFF\r\n", strlen("Power OFF\r\n"), HAL_MAX_DELAY);
			return 0;
		}
		else {
		}
	    HAL_UART_Transmit(&huart2, "Moteur déjà éteint\r\n", strlen("Moteur déjà éteint\r\n"), HAL_MAX_DELAY);
	}
