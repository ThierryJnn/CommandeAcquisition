/*
 * stopMotor.c
 *
 *  Created on: Oct 3, 2023
 *      Author: thier
 */

/**
  ******************************************************************************
  * @file    stopMotor.c
  * @brief   This file provides the code for turning off the motor.
  ******************************************************************************
  */


#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

extern int Power; // Variable externe indiquant l'état de la puissance du moteur (allumé/éteint)

/**
 * @brief Désactive le moteur en arrêtant la génération des PWM.
 * @retval None
 * @param None
 * @note Si le moteur est déja éteint alors on affiche seulement un message l'indiquant.
 * Le moteur est éteint par la l'arrêt de generation des PWM.
 */
void stopMotor() {
	// Desactivez l'étage de puissance du moteur (code pour desactiver le moteur)
	if (Power==1){
		// Si le moteur est allumé, éteignez-le
		Power=0; // Met à jour l'état de la puissance du moteur
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // Arrête le PWM pour le canal 1 du TIM1
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); // Arrête le PWM pour le canal 2 du TIM1
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1); // Arrête le PWM complémentaire pour le canal 1 du TIM1
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2); // Arrête le PWM complémentaire pour le canal 2 du TIM1
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,512); // Réglez le rapport cyclique pour le canal 1 à l'arrêt
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1024-512); // Réglez le rapport cyclique pour le canal 2 à l'arrêt
		// Affichez le message "Power OFF" dans la console
		HAL_UART_Transmit(&huart2, "Power OFF\r\n", strlen("Power OFF\r\n"), HAL_MAX_DELAY);
		return; // Quittez la fonction après avoir éteint le moteur
	}
	else {
		// Si le moteur est déjà éteint, affichez un message correspondant
		HAL_UART_Transmit(&huart2, "Moteur déjà éteint\r\n", strlen("Moteur déjà éteint\r\n"), HAL_MAX_DELAY);
	}
}
