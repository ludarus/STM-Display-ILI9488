/*
 * commands-can.h
 *
 *  Created on: Jun 30, 2026
 *      Author: Luke Fadel
 */

#ifndef INC_COMMANDS_CAN_H_
#define INC_COMMANDS_CAN_H_

#include "main.h"
#include <stdbool.h>

// data format enums. TODO get correct values for these
#define KEY_PAD_DATA 1
#define LSB_OBJ_NUM 2
#define MSB_OBJ_NUM 3
#define INDEX 4
#define DUTY_CYCLE 5
#define FREQUENCY 6

// TODO get correct version string
const uint8_t version[8] = "DSP12345";

void canCommandsInit(CAN_HandleTypeDef *canInterface,
                     SPI_HandleTypeDef *displaySpiInterface,
                     UART_HandleTypeDef *serialLoggingInterface,
                     TIM_HandleTypeDef *alarmPWMTimerInterface);
void canProcessCommands(void);

typedef struct {
  CAN_RxHeaderTypeDef header;
  uint8_t data[8];
} CanRxMessage_t;

typedef struct {
  // the command number associated with this command
  uint8_t cmdNum;
  // a function pointer to a handle that executes when the command is called
  bool (*handle)(CanRxMessage_t *);
} CanCommand_t;

#endif /* INC_COMMANDS_CAN_H_ */
