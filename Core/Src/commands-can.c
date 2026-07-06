/*
 * commands-can.c
 *
 *  Created on: Jun 30, 2026
 *      Author: Luke Fadel
 */

#include "commands-can.h"
#include "main.h"
#include "stm32f091xc.h"
#include "stm32f0xx_hal_can.h"
#include "stm32f0xx_hal_spi.h"
#include "stm32f0xx_hal_uart.h"
#include <stdbool.h>

// TODO: store time of the last received message (can be done in the process
// function)

// private declarations
static volatile uint8_t queuedMessages = 0;
// 16 message queue
static CanRxMessage_t queue[16];
static CAN_HandleTypeDef *can;
static SPI_HandleTypeDef *spi;
static UART_HandleTypeDef *uart;

void canCommandsInit(CAN_HandleTypeDef *canInterface,
                     SPI_HandleTypeDef *displaySpiInterface,
                     UART_HandleTypeDef *serialLoggingInterface) {

  can = canInterface;
  spi = displaySpiInterface;
  uart = serialLoggingInterface;

  // configuring filter
  CAN_FilterTypeDef sFilterConfig = {0};

  sFilterConfig.FilterMaskIdHigh = 0;
  sFilterConfig.FilterMaskIdLow = 0;
  sFilterConfig.FilterIdHigh = 0;
  sFilterConfig.FilterIdLow = 0;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  sFilterConfig.SlaveStartFilterBank = 0;
  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;

  HAL_CAN_ConfigFilter(canInterface, &sFilterConfig);
  // starting device
  HAL_CAN_Start(canInterface);
  // enabling interrupt for can receive
  HAL_CAN_ActivateNotification(canInterface, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void canProcessCommands() {
  for (uint8_t msgIdx = 0; msgIdx < queuedMessages; msgIdx++) {
    // iterating through every message

    // logging the message to serial device
    HAL_UART_Transmit_IT(uart, queue[queuedMessages].data, 8);
  }
  queuedMessages = 0;
}

// callback for received message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  // queueing the command for processing in main loop
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &queue[queuedMessages].header,
                       queue[queuedMessages].data);
  queuedMessages++;
}
