#include "uart_led.h"
#include <string.h>

static UART_HandleTypeDef *led_uart;
static uint8_t rx_byte;
static volatile uint8_t rx_cmd;
static volatile uint8_t rx_ready = 0;

static void UART_LED_Send(const char *msg)
{
  HAL_UART_Transmit(led_uart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void UART_LED_Init(UART_HandleTypeDef *huart)
{
  led_uart = huart;

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  UART_LED_Send("\r\nUART LED ready. Send 'a' = ON, 'b' = OFF\r\n");

  HAL_UART_Receive_IT(led_uart, &rx_byte, 1);
}

void UART_LED_RxCallback(UART_HandleTypeDef *huart)
{
  if (huart != led_uart)
  {
    return;
  }

  rx_cmd   = rx_byte;
  rx_ready = 1;

  HAL_UART_Receive_IT(led_uart, &rx_byte, 1);
}

void UART_LED_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart != led_uart)
  {
    return;
  }

  __HAL_UART_CLEAR_OREFLAG(led_uart);
  HAL_UART_Receive_IT(led_uart, &rx_byte, 1);
}

void UART_LED_Process(void)
{
  if (!rx_ready)
  {
    return;
  }

  uint8_t cmd = rx_cmd;
  rx_ready = 0;

  switch (cmd)
  {
    case 'a':
      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
      UART_LED_Send("LED ON\r\n");
      break;

    case 'b':
      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
      UART_LED_Send("LED OFF\r\n");
      break;

    case '\r':
    case '\n':
      break;

    default:
      UART_LED_Send("Please choose a to turn on LED or b to turn off LED\r\n");
      break;
  }
}