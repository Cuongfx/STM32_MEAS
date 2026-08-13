// sensor.c
#include "sensor.h"

#define TRIG_PULSE_US 10
#define SENSOR_MIN_CM   2
#define SENSOR_MAX_CM 400


static TIM_HandleTypeDef *s_htim_us;
static GPIO_TypeDef *s_trig_port;
static uint16_t      s_trig_pin;
static GPIO_TypeDef *s_echo_port;
static uint16_t      s_echo_pin;
static volatile uint32_t s_echo_start = 0;
static volatile uint32_t s_echo_end = 0;
static volatile uint8_t s_echo_captured = 0;
static uint32_t s_distance_cm = SENSOR_MAX_CM;
static volatile uint8_t s_measurement_busy = 0;

void Sensor_Init(TIM_HandleTypeDef *htim_us,
                  GPIO_TypeDef *trig_port, uint16_t trig_pin,
                  GPIO_TypeDef *echo_port, uint16_t echo_pin)
{
  s_htim_us   = htim_us;
  s_trig_port = trig_port;
  s_trig_pin  = trig_pin;
  s_echo_port = echo_port;
  s_echo_pin  = echo_pin;

  HAL_TIM_Base_Start(s_htim_us);
}

static void delay_us(uint16_t us)
{
  uint16_t start = (uint16_t)__HAL_TIM_GET_COUNTER(s_htim_us);
  while ((uint16_t)((uint16_t)__HAL_TIM_GET_COUNTER(s_htim_us) - start) < us)
  {
  }
}

void Sensor_Trigger(void)
{
  if (s_measurement_busy) return;
  s_measurement_busy = 1;

  HAL_GPIO_WritePin(s_trig_port, s_trig_pin, GPIO_PIN_SET);
  delay_us(TRIG_PULSE_US);
  HAL_GPIO_WritePin(s_trig_port, s_trig_pin, GPIO_PIN_RESET);
}

void Sensor_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != s_echo_pin) return;

  if (HAL_GPIO_ReadPin(s_echo_port, s_echo_pin) == GPIO_PIN_SET)
  {
    s_echo_start = __HAL_TIM_GET_COUNTER(s_htim_us);
  }
  else
  {
    s_echo_end = __HAL_TIM_GET_COUNTER(s_htim_us);
    uint16_t pulse_width_us = (uint16_t)(s_echo_end - s_echo_start);
    uint32_t distance = (uint32_t)(0.017f * (float)pulse_width_us);
    if (distance < SENSOR_MIN_CM) distance = SENSOR_MIN_CM;
    if (distance > SENSOR_MAX_CM) distance = SENSOR_MAX_CM;
    s_distance_cm = distance;
    s_echo_captured = 1;
    s_measurement_busy = 0;
  }
}

uint32_t Sensor_GetDistanceCm(void)
{
  return s_distance_cm;
}

