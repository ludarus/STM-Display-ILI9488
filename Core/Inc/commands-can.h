/*
 * commands-can.h
 *
 *  Created on: Jun 30, 2026
 *      Author: Luke Fadel
 */

#ifndef INC_COMMANDS_CAN_H_
#define INC_COMMANDS_CAN_H_

#include "main.h"
#include "stm32f0xx_hal_def.h"
#include "stm32f0xx_hal_tim.h"
#include <stdbool.h>

HAL_StatusTypeDef
canCommandsInit(CAN_HandleTypeDef *canInterface,
                SPI_HandleTypeDef *displaySpiInterface,
                UART_HandleTypeDef *serialLoggingInterface,
                TIM_HandleTypeDef *alarmPWMTimerInterface,
                TIM_HandleTypeDef *backlightPWMTimerInterface);
HAL_StatusTypeDef canProcessCommands(void);

typedef struct {
  CAN_RxHeaderTypeDef header;
  uint8_t data[8];
} CanRxMessage_t;

typedef struct {
  // the command number associated with this command
  uint8_t cmdNum;
  // a function pointer to a handle that executes when the command is called
  HAL_StatusTypeDef (*handle)(CanRxMessage_t *);
} CanCommand_t;

#endif /* INC_COMMANDS_CAN_H_ */
