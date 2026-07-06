/*
 * commands-can.h
 *
 *  Created on: Jun 30, 2026
 *      Author: Luke Fadel
 */

#ifndef INC_COMMANDS_CAN_H_
#define INC_COMMANDS_CAN_H_

#include "main.h"

void canCommandsInit(CAN_HandleTypeDef *canInterface,
                     SPI_HandleTypeDef *displaySpiInterface,
                     UART_HandleTypeDef *serialLoggingInterface);
void canProcessCommands();

typedef struct {
  CAN_RxHeaderTypeDef header;
  uint8_t data[8];
} CanRxMessage_t;

#endif /* INC_COMMANDS_CAN_H_ */
