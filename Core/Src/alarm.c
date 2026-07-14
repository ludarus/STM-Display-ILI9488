/*
 * alarm.c
 *
 *  Created on: Jul 6, 2026
 *      Author: Luke Fadel
 */

#include "alarm.h"
#include "stm32f0xx_hal_tim.h"
#include <stdint.h>

static const PwmSetting_t frequencySettings[8] = {
    // psc = 0 for highest granularity
    {0, 3332},  {0, 6666},  {0, 9999},  {0, 13332},
    {0, 16666}, {0, 19999}, {0, 23391}, {0, 1666}};

static PwmSetting_t alarmState = frequencySettings[0];

// pass in a value from 0-7 for frequency, and a value from 0-255 for duty cycle
void setAlarm(TIM_HandleTypeDef *alarmTimer, uint8_t frequencyIndex,
              uint8_t dutyCycle) {

  if (frequencyIndex > 7) {
    return;
  }

  // setting alarm state
  alarmState.psc = frequencySettings[frequencyIndex].psc;
  alarmState.arr = frequencySettings[frequencyIndex].arr;

  // changing the prescaler, autoreload, and pulse settings for the desired
  // frequency
  __HAL_TIM_SET_PRESCALER(alarmTimer, frequencySettings[frequencyIndex].psc);
  __HAL_TIM_SET_AUTORELOAD(alarmTimer, frequencySettings[frequencyIndex].arr);
  __HAL_TIM_SET_COMPARE(
      alarmTimer, TIM_CHANNEL_1,
      (uint16_t)((frequencySettings[frequencyIndex].arr * dutyCycle) / 0xFF));
}

// using 50% duty cycle as on. TODO confirm this
void enableAlarm(TIM_HandleTypeDef *alarmTimer) {
  __HAL_TIM_SET_PRESCALER(alarmTimer, alarmState.psc);
  __HAL_TIM_SET_AUTORELOAD(alarmTimer, alarmState.arr);
  __HAL_TIM_SET_COMPARE(alarmTimer, TIM_CHANNEL_1,
                        (uint16_t)((alarmState.arr) / 2));
}

void disableAlarm(TIM_HandleTypeDef *alarmTimer) {
  __HAL_TIM_SET_PRESCALER(alarmTimer, alarmState.psc);
  __HAL_TIM_SET_AUTORELOAD(alarmTimer, alarmState.arr);
  __HAL_TIM_SET_COMPARE(alarmTimer, TIM_CHANNEL_1, 0);
}
