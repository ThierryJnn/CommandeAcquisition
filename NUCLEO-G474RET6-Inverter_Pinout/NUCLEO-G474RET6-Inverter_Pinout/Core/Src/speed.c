/*
 * speed.c
 *
 *  Created on: Oct 20, 2023
 *      Author: thier
 */

/**
  ******************************************************************************
  * @file    speed.c
  * @brief   This file provides the code for managing the rotational speed and direction of the motor.
  ******************************************************************************
  */

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "mesure.h"

extern int Power; // Variable externe indiquant l'état de la puissance du moteur (allumé/éteint)
int vit=512; // Variable de vitesse initiée a sa valeur initiale de 512 (rapport cylique de 50% donc le moteur ne tourne pas)

/**
 * @brief Contrôle la vitesse du moteur.
 * @retval None
 * @param int digit
 * @note La valeur de la vitesse à régler est entre 0 et 1024,
 *  0 pour un rapport cyclique de 0%, 512 pour un rapport cyclique de 50% et
 *  1024 pour un rapport cyclique de 100%. Le moteur est à l'arret pour un rapport cylique de 50%.
 */
void speed(int digit) {
	int oldspeed=vit; // Stocke l'ancienne valeur de la vitesse
	int i=0; // Variable de boucle
	char message[30]; // Message à transmettre via l'UART

	if (Power==0){
		// Si le moteur est éteint, affiche un message et quitte la fonction
		HAL_UART_Transmit(&huart2, "Moteur éteint\r\n", strlen("Moteur éteint\r\n"), HAL_MAX_DELAY);
		return 0;
	}

	vit = digit; // Met à jour la vitesse avec la nouvelle valeur
	if (vit>1024) {
		// Si la vitesse demandée est supérieure à la limite, ajuste à la limite maximale
		vit=1024;
		HAL_UART_Transmit(&huart2, "Vitesse abaissée au max: 1023\r\n", strlen("Vitesse abaissée au max: 1023\r\n"), HAL_MAX_DELAY);
	}
	if(oldspeed<vit){
		// Augmente progressivement la vitesse du moteur
		for(i=oldspeed;i<vit;i=i+1){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1024-i);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,i);
			snprintf(message, sizeof(message), "i: %d \r\n", i);
			HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
			HAL_Delay(20);
		}
	}
	if(oldspeed>vit){
		// Diminue progressivement la vitesse du moteur
		for(i=oldspeed;i>vit;i=i-1){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1024-i);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,i);
			snprintf(message, sizeof(message), "i: %d \r\n", i);
			HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
			HAL_Delay(20);
		}
	}

	// Calcule et affiche le rapport cyclique
	int rapcycl= vit*100/1024;
	mesureSpeed(); // Mesure et affichage la vitesse actuelle mesurée par le codeur
	snprintf(message, sizeof(message), "Speed: %d \r\n", vit); // Formatte le message de vitesse calculé par le programme
	HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY); // Transmet le message de vitesse calculée via l'UART2
	snprintf(message, sizeof(message), "Rapport cyclique: %d \r\n", rapcycl); // Formatte le message de rapport cyclique calculé par le programme
	HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY); // Transmet le message calculé par le programme via l'UART2

}
