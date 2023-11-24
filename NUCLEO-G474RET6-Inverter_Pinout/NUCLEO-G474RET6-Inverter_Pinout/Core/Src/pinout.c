/*
 * pinout.c
 *
 *  Created on: Oct 3, 2023
 *      Author: thier
 */

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

// Structure pour stocker les informations sur chaque broche
typedef struct {
	char name[20];     // Nom de la broche
	char function[20]; // Fonction de la broche
	char state[20];    // État de la broche (par exemple, IN, OUT, HIGH, LOW, etc.)
} PinInfo;

// Tableau contenant les informations sur chaque broche
PinInfo pins[] = {
		{"PA8", "U_PWM_H", "OUT"},
		{"PA9", "V_PWM_H", "OUT"},
		{"PA10", "W_PWM_H", "OUT"},
		{"PB13", "U_PWM_L", "OUT"},
		{"PB14", "V_PWM_L", "OUT"},
		{"PB15", "W_PWM_L", "OUT"},
		{"PA2", "USART2_TX", "OUT"},
		{"PA3", "USART2_RX", "IN"},
		// Ajoutez d'autres broches et leurs informations ici
};

/**
 * @brief Affiche les informations sur la configuration des broches.
 */
void pinout() {
	HAL_UART_Transmit(&huart2, "Pinout:\r\n", strlen("Pinout:\r\n"), HAL_MAX_DELAY);

	// Parcours toutes les broches et affiche leurs informations
	for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
		char message[100];
		snprintf(message, sizeof(message), "Pin %s: Function - %s, State - %s\r\n", pins[i].name, pins[i].function, pins[i].state);
		HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
	}
}

