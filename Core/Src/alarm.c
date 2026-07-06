/*
 * alarm.c
 *
 *  Created on: Jul 6, 2026
 *      Author: Luke Fadel
 */

#include "alarm.h"
#include "main.h"
#include "stm32f0xx_hal_tim.h"
#include <stdint.h>

static const PwmState_t frequencySettings[8] = {
    // psc = 0, duty cycle = 50%
    {0, 3332, 1667},  {0, 6666, 3334},   {0, 9999, 5000},   {0, 13332, 6667},
    {0, 16666, 8334}, {0, 19999, 10000}, {0, 23391, 11696}, {0, 1666, 834}};

// pass in a value from 0-7
void setAlarmFrequency(TIM_HandleTypeDef *alarmTimer, uint8_t frequencyIndex) {
  if (frequencyIndex > 7) {
    return;
  }

  // changing the prescaler, autoreload, and pulse settings for the desired
  // frequency
  __HAL_TIM_SET_PRESCALER(alarmTimer, frequencySettings[frequencyIndex].psc);
  __HAL_TIM_SET_AUTORELOAD(alarmTimer, frequencySettings[frequencyIndex].arr);
  __HAL_TIM_SET_COMPARE(alarmTimer, TIM_CHANNEL_1,
                        frequencySettings[frequencyIndex].duty);
}
