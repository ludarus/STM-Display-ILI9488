/*
 * alarm.h
 *
 *  Created on: Jul 6, 2026
 *      Author: joe
 */

#ifndef INC_ALARM_H_
#define INC_ALARM_H_

#include "main.h"

#define CLOCK0 14400
#define CLOCK1 7200
#define CLOCK2 4800
#define CLOCK3 3600
#define CLOCK4 2880
#define CLOCK5 2400
#define CLOCK6 2052
#define CLOCK7 28800

void setAlarmFrequency(TIM_HandleTypeDef *alarmTimer, uint8_t frequencyIndex, uint8_t dutyCycle);

typedef struct {
  uint16_t psc;
  uint16_t arr;
} PwmState_t;

#endif /* INC_ALARM_H_ */
