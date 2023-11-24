/*
 * processCommand.c
 *
 *  Created on: Oct 3, 2023
 *      Author: thier
 */

#include "adc.h"
#include "tim.h"
#include "speed.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "pinout.h"
#include "startMotor.h"
#include "stopMotor.h"

int Power=0; // Initie la variable globale "Power" à 0 au démarrage du programme car le moteur est éteint.

/**
 * @brief Traite la commande reçue sur l'UART.
 *
 * @param command La commande à traiter.
 */
void processCommand(char* command) {
	// Vérifie si la commande est "help"
	if (strcmp(command, "help") == 0) {
		// Affiche le message d'aide
		HAL_UART_Transmit(&huart2, "Commandes disponibles :\r\n", strlen("Commandes disponibles :\r\n"), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, " - help : Affiche ce message d'aide\r\n", strlen(" - help : Affiche ce message d'aide\r\n"), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, " - pinout : Affiche la configuration des pins\r\n", strlen(" - pinout : Affiche la configuration des pins\r\n"), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, " - start : Demarre le moteur\r\n", strlen(" - start : Demarre le moteur\r\n"), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, " - stop : Arrete le moteur\r\n", strlen(" - start : Arrete le moteur\r\n"), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, " - speed xxxx : Regle la vitesse du moteur\r\n", strlen(" - speed xxxx : Regle la vitesse du moteur\r\n"), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, "	-- xxxx est compris entre 0 et 1024, a 512 le moteur est a l'arret\r\n", strlen("	-- xxxx est compris entre 0 et 1024, a 512 le moteur est a l'arret\r\n"), HAL_MAX_DELAY);
		// Ajoutez d'autres commandes et leurs descriptions ici
		return 0;
	}
	// Vérifie si la commande est "pinout"
	if (strcmp(command, "pinout") == 0) {
		pinout(); // Appeler la fonction pinout
		return 0;
	}
	// Vérifie si la commande est "start"
	if (strcmp(command, "start") == 0) {
		startMotor(); // Appeler la fonction startMotor
		return 0;
	}
	// Vérifie si la commande est "stop"
	if (strcmp(command, "stop") == 0) {
		stopMotor(); // Appeler la fonction stopMotor
		return 0;
	}
	// Vérifie si la commande commence par "speed"
	if (strncmp(command, "speed", 5) == 0) {
		// Extrait les quatre chiffres suivant "speed"
		int digit;
		if (sscanf(command + 5, "%d", &digit) == 1) {
			speed(digit); // Appeler la fonction speed avec les 4 chiffres en argument
		} else {
			HAL_UART_Transmit(&huart2, "Format incorrect. Utilisation : speed <chiffre1><chiffre2><chiffre3><chiffre4>\r\n", strlen("Format incorrect. Utilisation : speed <chiffre1><chiffre2><chiffre3><chiffre4>\r\n"), HAL_MAX_DELAY);
		}
	}
	else {
		HAL_UART_Transmit(&huart2, "Commande inconnue\r\n", strlen("Commande inconnue\r\n"), HAL_MAX_DELAY);
	}
	// Ajoutez d'autres conditions pour d'autres commandes si nécessaire
}
