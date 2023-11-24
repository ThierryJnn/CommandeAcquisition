/*
 * startMotor.c
 *
 *  Created on: Oct 3, 2023
 *      Author: thier
 */

/**
  ******************************************************************************
  * @file    startMotor.c
  * @brief   This file provides the code for turning on the motor.
  ******************************************************************************
  */

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

extern int Power; // Variable externe indiquant l'état de la puissance du moteur (allumé/éteint)

/**
 * @brief Active le moteur en démarrant la génération des PWM.
 */
void startMotor() {
	// Activez l'étage de puissance du moteur (code pour activer le moteur)
	if (Power==0){
		// Si le moteur est éteint, activez-le
		Power=1; // Met à jour l'état de la puissance du moteur
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Démarre le PWM pour le canal 1 du TIM1
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // Démarre le PWM pour le canal 2 du TIM1
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); // Démarre le PWM complémentaire pour le canal 1 du TIM1
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2); // Démarre le PWM complémentaire pour le canal 2 du TIM1
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1024-512); // Réglez le rapport cyclique pour le canal 1
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,512); // Réglez le rapport cyclique pour le canal 2
		// Affichez le message "Power ON" dans la console
		HAL_UART_Transmit(&huart2, "Power ON\r\n", strlen("Power ON\r\n"), HAL_MAX_DELAY);
		return; // Quittez la fonction après avoir allumé le moteur
	}
	else {
		// Si le moteur est déjà allumé, affichez un message correspondant
		HAL_UART_Transmit(&huart2, "Moteur déjà allumé\r\n", strlen("Moteur déjà allumé\r\n"), HAL_MAX_DELAY);
	}
}
