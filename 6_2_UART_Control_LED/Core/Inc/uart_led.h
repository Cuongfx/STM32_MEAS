#ifndef UART_LED_H
#define UART_LED_H

#include "main.h"

void UART_LED_Init(UART_HandleTypeDef *huart);
void UART_LED_Process(void);
void UART_LED_RxCallback(UART_HandleTypeDef *huart);
void UART_LED_ErrorCallback(UART_HandleTypeDef *huart);

#endif