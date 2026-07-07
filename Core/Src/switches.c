/*
 * switches.c
 *
 *  Created on: Jul 6, 2026
 *      Author: Luke Fadel
 */

#include "main.h"
#include "stm32f0xx.h"
#include "stm32f0xx_hal_can.h"
#include "stm32f0xx_hal_gpio.h"
#include <stdint.h>

// arrays 1-3 are for the last 3 states, array 4 is for the debounced state
static uint8_t switchState[4][5];
static uint8_t currentIdx;
static volatile uint8_t flag;

// reads, debounces, and transmits the state of switches. run in main loop
void processSwitches(CAN_HandleTypeDef *canInterface) {
  // reading current switch state
  switchState[currentIdx][0] = HAL_GPIO_ReadPin(SWITCH1_GPIO_Port, SWITCH1_Pin);
  switchState[currentIdx][1] = HAL_GPIO_ReadPin(SWITCH2_GPIO_Port, SWITCH2_Pin);
  switchState[currentIdx][2] = HAL_GPIO_ReadPin(SWITCH3_GPIO_Port, SWITCH3_Pin);
  switchState[currentIdx][3] = HAL_GPIO_ReadPin(SWITCH4_GPIO_Port, SWITCH4_Pin);
  switchState[currentIdx][4] = HAL_GPIO_ReadPin(SWITCH5_GPIO_Port, SWITCH5_Pin);

  // incrementing index
  currentIdx = (currentIdx + 1) % 3;

  uint8_t debouncedState = 0;

  // iterating through last 3 states
  for (uint8_t sw = 0; sw < 5; sw++) {
    // computing debounced state
    switchState[3][sw] = (switchState[0][sw] == switchState[1][sw] &&
                          switchState[1][sw] == switchState[2][sw])
                             ? switchState[0][sw]
                             : switchState[3][sw];

	 // packing payload
    debouncedState |= switchState[3][sw] << sw;
  }

  // transmitting debounced state
  // TODO: check how much of the function should be gated by the flag
  if (flag) {
    CAN_TxHeaderTypeDef header = {0};

    // 5 bits of data, one per switch
    header.DLC = 5;

    // as specified in protocol
    header.StdId = 0x140;

    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;

    // TODO: confirm this
    header.TransmitGlobalTime = DISABLE;

    uint32_t mailbox;

    HAL_CAN_AddTxMessage(canInterface, &header, &debouncedState, &mailbox);

    // resetting flag
    flag = 0;
  }
}

// interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    // setting flag
    flag = 1;
  }
}
