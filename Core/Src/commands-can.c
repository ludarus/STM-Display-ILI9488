/*
 * commands-can.c
 *
 *  Created on: Jun 30, 2026
 *      Author: Luke Fadel
 */

#include "commands-can.h"
#include "main.h"

// store time of the last received message (can be done in the process function)

// private declarations
static uint8_t queuedMessages = 0;
// 16 message queue
static CanMessage_t queue[16];
static const CAN_HandleTypeDef *can;

void canCommandsInit(CAN_HandleTypeDef *canInterface) {
  // enabling interrupt for can receive
  HAL_CAN_ActivateNotification(canInterface, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void canProcessCommand() {
	for (uint8_t msgIdx = 0; msgIdx < queuedMessages; msgIdx++){
		// iterating through every message
	}
}

// callback for received message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  queuedMessages++;
  // queueing the command for processing in main loop
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &queue[queuedMessages].header,
                       queue[queuedMessages].data);
}
