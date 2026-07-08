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
    {0, 3332},  {0, 6666},  {0, 9999},  {0, 13332},
    {0, 16666}, {0, 19999}, {0, 23391}, {0, 1666}};

// pass in a value from 0-7
void setAlarmFrequency(TIM_HandleTypeDef *alarmTimer, uint8_t frequencyIndex,
                       uint8_t dutyCycle) {

  if (frequencyIndex > 7) {
    return;
  }

  // changing the prescaler, autoreload, and pulse settings for the desired
  // frequency
  __HAL_TIM_SET_PRESCALER(alarmTimer, frequencySettings[frequencyIndex].psc);
  __HAL_TIM_SET_AUTORELOAD(alarmTimer, frequencySettings[frequencyIndex].arr);
  __HAL_TIM_SET_COMPARE(
      alarmTimer, TIM_CHANNEL_1,
      (uint16_t)((frequencySettings[frequencyIndex].arr * dutyCycle) / 0xFF));
}
