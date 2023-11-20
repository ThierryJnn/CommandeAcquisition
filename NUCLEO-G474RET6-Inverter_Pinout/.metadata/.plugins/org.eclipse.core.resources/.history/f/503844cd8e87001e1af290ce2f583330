/*
 * speed.c
 *
 *  Created on: Oct 20, 2023
 *      Author: thier
 */

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

extern int Power;
int vit=512;
void speed(int digit) {
	int oldspeed=vit;
	int i=0;
	char message[30];
	if (Power==0){
		HAL_UART_Transmit(&huart2, "Moteur éteint\r\n", strlen("Moteur éteint\r\n"), HAL_MAX_DELAY);
		return 0;
	}

    vit = digit;
    if (vit>1024) {
    	vit=1024;
    	HAL_UART_Transmit(&huart2, "Vitesse abaissée au max: 1023\r\n", strlen("Vitesse abaissée au max: 1023\r\n"), HAL_MAX_DELAY);
    }
    if(oldspeed<vit){
    	for(i=oldspeed;i<vit;i=i+1){
    		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1024-i);
    		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,i);
    		snprintf(message, sizeof(message), "i: %d \r\n", i);
    		HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
    		HAL_Delay(20);
    	}
    }
    if(oldspeed>vit){
    	for(i=oldspeed;i>vit;i=i-1){
    		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1024-i);
    		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,i);
    		snprintf(message, sizeof(message), "i: %d \r\n", i);
    		HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
    		HAL_Delay(20);
    	}
    }
    int rapcycl= vit*100/1024;
    snprintf(message, sizeof(message), "Speed: %d \r\n", vit);
    HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
    snprintf(message, sizeof(message), "Rapport cyclique: %d \r\n", rapcycl);
    HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
}
