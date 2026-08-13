// sensor.h
#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"

void Sensor_Init(TIM_HandleTypeDef *htim_us,
                  GPIO_TypeDef *trig_port, uint16_t trig_pin,
                  GPIO_TypeDef *echo_port, uint16_t echo_pin);
void Sensor_Trigger(void);
void Sensor_EXTI_Callback(uint16_t GPIO_Pin);
uint32_t Sensor_GetDistanceCm(void);

#endif