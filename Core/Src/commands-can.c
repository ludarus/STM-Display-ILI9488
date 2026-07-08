/*
 * commands-can.c
 *
 *  Created on: Jun 30, 2026
 *      Author: Luke Fadel
 */

#include "commands-can.h"
#include "alarm.h"
#include "display-ili9488.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"
#include "stm32f0xx_hal_def.h"
#include "stm32f0xx_hal_flash.h"
#include "stm32f0xx_hal_flash_ex.h"
#include "stm32f0xx_hal_spi.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_uart.h"
#include <stdbool.h>

// includes for every image
#include "File_002_ObjNum_001_NEW_6_17_26.h"
#include "File_005_ObjNum_004_480x320_6_18_26.h"
#include "File_006_ObjNum_005_480x320_6_18_26_C.h"
#include "File_007_ObjNum_006_480x320_6_18_26.h"
#include "File_008_ObjNum_007_96x190_6_18_26.h"
#include "File_009_ObjNum_008_96x190_6_18_26.h"
#include "File_010_ObjNum_009_96x190_6_18_26.h"
#include "File_011_ObjNum_010_96x190_6_18_26.h"
#include "File_012_ObjNum_011_96x190_6_18_26.h"
#include "File_013_ObjNum_012_96x190_6_18_26.h"
#include "File_014_ObjNum_013_96x190_6_18_26.h"
#include "File_015_ObjNum_014_96x190_6_18_26.h"
#include "File_016_ObjNum_015_96x190_6_18_26.h"
#include "File_017_ObjNum_016_96x190_6_18_26.h"
#include "File_018_ObjNum_017_96x190_6_16_26.h"
#include "File_019_ObjNum_018_48x208_6_19_26.h"
#include "File_020_ObjNum_019_48x208_6_19_26.h"
#include "File_021_ObjNum_020_48x208_6_19_26.h"
#include "File_022_ObjNum_021_48x208_6_19_26.h"
#include "File_023_ObjNum_022_48x208_6_19_26.h"
#include "File_024_ObjNum_023_48x208_6_19_26.h"
#include "File_025_ObjNum_024_48x208_6_19_26.h"
#include "File_026_ObjNum_025_48x208_6_19_26.h"
#include "File_027_ObjNum_026_48x208_6_19_26.h"
#include "File_028_ObjNum_027_48x208_6_19_26.h"
#include "File_029_ObjNum_028_48x208_6_19_26.h"
#include "File_030_ObjNum_029_64x25_6_19_26.h"
#include "File_031_ObjNum_030_64x25_6_19_26.h"
#include "File_032_ObjNum_031_96x190_6_19_26.h"
#include "File_033_ObjNum_032_96x190_6_19_26.h"
#include "File_034_ObjNum_033_96x190_6_19_26.h"
#include "File_035_ObjNum_034_96x190_6_19_26.h"
#include "File_036_ObjNum_035_96x190_6_19_26.h"
#include "File_037_ObjNum_036_96x190_6_19_26.h"
#include "File_038_ObjNum_037_96x190_6_19_26.h"
#include "File_039_ObjNum_038_96x190_6_19_26.h"
#include "File_040_ObjNum_039_96x190_6_19_26.h"
#include "File_041_ObjNum_040_96x190_6_19_26.h"
#include "File_042_ObjNum_041_96x190_6_19_26.h"
#include "File_043_ObjNum_042_144x208_6_19_26.h"
#include "File_044_ObjNum_043_144x208_6_19_26.h"
#include "File_045_ObjNum_044_144x208_6_19_26.h"
#include "File_046_ObjNum_045_144x208_6_19_26.h"
#include "File_047_ObjNum_046_144x208_6_19_26.h"
#include "File_048_ObjNum_052_144x183_6_19_26.h"
#include "File_049_ObjNum_053_144x183_6_19_26.h"
#include "File_050_ObjNum_054_144x183_6_19_26.h"
#include "File_051_ObjNum_055_144x183_6_19_26.h"
#include "File_052_ObjNum_057_144x30_6_19_26.h"
#include "File_053_ObjNum_077_48x30_6_19_26.h"
#include "File_054_ObjNum_087_48x255_6_19_26.h"
#include "File_055_ObjNum_090_480x320_6_18_26.h"
#include "File_056_ObjNum_091_480x320_6_18_26.h"
#include "File_057_ObjNum_092_480x320_6_18_26.h"
#include "File_058_ObjNum_093_480x320_6_18_26.h"
#include "File_059_ObjNum_094_480x320_6_18_26.h"
#include "File_062_ObjNum_097_480x40_6_19_26.h"
#include "File_063_ObjNum_098_176x33_6_19_26.h"
#include "File_064_ObjNum_099_48x63_6_19_26.h"
#include "File_065_ObjNum_104_16x30_6_19_26.h"
#include "File_066_ObjNum_109_96x80_6_19_26.h"
#include "File_067_ObjNum_114_64x103_6_19_26.h"
#include "File_068_ObjNum_119_64x103_6_19_26.h"
#include "File_069_ObjNum_124_64x103_6_19_26.h"
#include "File_070_ObjNum_129_96x80_6_19_26.h"
#include "File_071_ObjNum_134_176x188_6_19_26.h"
#include "File_072_ObjNum_135_480x320_6_18_26.h"
#include "File_073_ObjNum_136_480x320_6_18_26.h"
#include "File_074_ObjNum_138_48x143_6_19_26.h"
#include "File_075_ObjNum_140_368x40_6_19_26.h"
#include "File_076_ObjNum_143_368x40_6_19_26.h"
#include "File_077_ObjNum_147_480x320_6_18_26.h"
#include "File_078_ObjNum_148_480x320_6_18_26.h"
#include "File_079_ObjNum_149_480x320_6_17_26.h"
#include "SYSFAIL_480x320_Gemini.h"

// TODO: store time of the last received message (can be done in the process
// 	|-> Is this the actual UTC time or the tick of the system?
// function)

// TODO get correct version string
const uint8_t version[8] = "DSP12345";

// private declarations
static volatile uint8_t queuedMessages = 0;
// 16 message queue
static CanRxMessage_t queue[48];

// static brightness members
static uint32_t brightnessTick;
static uint8_t brightnessVal;

// interfaces
static CAN_HandleTypeDef *can;
static SPI_HandleTypeDef *spi;
static UART_HandleTypeDef *uart;
static TIM_HandleTypeDef *alarmTimer;
static TIM_HandleTypeDef *backlightTimer;

// handles. TODO finish them when given the objnum and groupnum to image mapping
HAL_StatusTypeDef DispBackCmd(CanRxMessage_t *msg) {
  // cmdNum = 0x83
  // data format = LSB_OBJ_NUM, MSB_OBJ_NUM
  // assuming this means DLC = 2 bytes

  // extracting LSB byte
  uint8_t lsb = msg->data[0];

  // extracting MSB byte
  uint16_t msb = msg->data[1] << 8;

  uint16_t objNum = lsb | msb;

  // display according image
  // ILI9488_LOAD_IMAGE(spi, uint16_t x, uint16_t y, const Image_t *image, bool
  // overWrite, bool draw)

  return HAL_OK;
}

HAL_StatusTypeDef DispTextCmd(CanRxMessage_t *msg) {
  // static function members for persistent scope
  static uint8_t remainingChars = 0;
  static uint8_t target = 0;
  static uint8_t charArray[256] = {0};
  static uint16_t objNum = 0;

  // cmdNum = 0x84

  // checking if DLC is 0
  if (msg->header.DLC == 0) {
    // ignore command
    return HAL_OK;
  }

  if (remainingChars == 0 || msg->data[0] == 0) {
    // should have a value of 0
    uint8_t First_Pkt_Flag = msg->data[0];
    // number of characters in the string
    remainingChars = msg->data[1];
    target = remainingChars;
    // obj number
    uint8_t lsb = msg->data[2];
    uint16_t msb = msg->data[3] << 8;

    objNum = lsb | msb;

    // filling up the 4 bytes of charInfo contained within this packet
    uint8_t fill = (remainingChars > 4) ? 4 : remainingChars;
    for (uint8_t i = 0; i < fill; i++) {
      charArray[i] = msg->data[i + 4];
      remainingChars--;
    }
  }
  // continuation of previous packet
  else {
    uint8_t fill = (remainingChars > 8) ? 8 : remainingChars;
    for (uint8_t i = 0; i < fill; i++) {
      charArray[i + (target - remainingChars)] = msg->data[i];
      remainingChars--;
    }
  }

  // end condition
  if (remainingChars == 0) {
    // return ILI9488_LOAD_TEXT(spi, uint16_t x, uint16_t y, charArray,target,
    // const Character_t *font, uint8_t characterWidth, size_t fontSize, size_t
    // characterHeight, true)
  }

  return HAL_OK;
}

HAL_StatusTypeDef DispBMCmd(CanRxMessage_t *msg) {

  // cmdNum = 0x85
  // data format = LSB_OBJ_NUM, MSB_OBJ_NUM
  // assuming this means DLC = 2 bytes

  // extracting LSB byte
  uint8_t lsb = msg->data[0];

  // extracting MSB byte
  uint16_t msb = msg->data[1] << 8;

  uint16_t objNum = lsb | msb;

  // display according image
  // ILI9488_LOAD_IMAGE(spi, uint16_t x, uint16_t y, const Image_t *image, bool
  // overWrite, bool draw)

  return HAL_OK;
}

HAL_StatusTypeDef DispGrpCmd(CanRxMessage_t *msg) {

  // cmdNum = 0x86
  // data format = LSB_OBJ_NUM, MSB_OBJ_NUM
  // assuming this means DLC = 2 bytes

  // extracting LSB byte
  uint8_t lsb = msg->data[0];

  // extracting MSB byte
  uint16_t msb = msg->data[1] << 8;

  uint16_t grpNum = lsb | msb;

  uint8_t index = msg->data[2];

  // display according image
  // ILI9488_LOAD_IMAGE(spi, uint16_t x, uint16_t y, const Image_t *image, bool
  // overWrite, bool draw)
  return HAL_OK;
}

HAL_StatusTypeDef SendVerCmd(CanRxMessage_t *msg) {
  // cmdNum 0x87

  CAN_TxHeaderTypeDef versionHeader = {0};

  // 8 byte string
  versionHeader.DLC = 8;

  // as specified in protocol
  versionHeader.StdId = 0x168;

  versionHeader.IDE = CAN_ID_STD;
  versionHeader.RTR = CAN_RTR_DATA;

  // TODO: confirm this
  versionHeader.TransmitGlobalTime = DISABLE;

  uint32_t mailbox;

  return HAL_CAN_AddTxMessage(can, &versionHeader, version, &mailbox);
}

HAL_StatusTypeDef SysFailCmd(CanRxMessage_t *msg) {
  // cmdNum 0x88
  return ILI9488_LOAD_IMAGE(spi, 0, 0, &SYSFAIL_480x320_Gemini, true, true);
}

HAL_StatusTypeDef BrightCmd(CanRxMessage_t *msg) {
  // cmdNum 0x89
  brightnessVal = msg->data[0];

  // setting brightness
  HAL_TRY(ILI9488_BRIGHTNESS(spi, backlightTimer, brightnessVal));

  // setting flag to wait 5 seconds
  brightnessTick = HAL_GetTick();

  return HAL_OK;
}

HAL_StatusTypeDef AlarmCmd(CanRxMessage_t *msg) {
  // assuming dutyCycle is an 8 bit number where 0 is off and 255 is 100%
  uint8_t dutyCycle = msg->data[0];
  uint8_t frequency = msg->data[1];

  setAlarmFrequency(alarmTimer, frequency, dutyCycle);

  return HAL_OK;
}

// list of commands to be received
static const CanCommand_t commands[] = {
    // display background image
    {.cmdNum = 0x83, .handle = DispBackCmd},
    // display text
    {.cmdNum = 0x84, .handle = DispTextCmd},
    // display image
    {.cmdNum = 0x85, .handle = DispBMCmd},
    // display group
    {.cmdNum = 0x86, .handle = DispGrpCmd},
    // send version
    {.cmdNum = 0x87, .handle = SendVerCmd},
    // system failure
    {.cmdNum = 0x88, .handle = SysFailCmd},
    // adjust brightness
    {.cmdNum = 0x89, .handle = BrightCmd},
    // alarm
    {.cmdNum = 0x8A, .handle = AlarmCmd},
};

HAL_StatusTypeDef
canCommandsInit(CAN_HandleTypeDef *canInterface,
                SPI_HandleTypeDef *displaySpiInterface,
                UART_HandleTypeDef *serialLoggingInterface,
                TIM_HandleTypeDef *alarmPWMTimerInterface,
                TIM_HandleTypeDef *backlightPWMTimerInterface) {

  can = canInterface;
  spi = displaySpiInterface;
  uart = serialLoggingInterface;
  alarmTimer = alarmPWMTimerInterface;
  backlightTimer = backlightPWMTimerInterface;

  // configuring filter
  CAN_FilterTypeDef sFilterConfig = {0};

  // Keep filter open for now. TODO flesh out filter once full protocol is
  // resolved
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

  HAL_TRY(HAL_CAN_ConfigFilter(canInterface, &sFilterConfig));
  // starting device
  HAL_TRY(HAL_CAN_Start(canInterface));
  // enabling interrupt for can receive
  HAL_TRY(
      HAL_CAN_ActivateNotification(canInterface, CAN_IT_RX_FIFO0_MSG_PENDING));

  return HAL_OK;
}

HAL_StatusTypeDef canProcessCommands(void) {
  // if it's been 5 seconds since last brightness change
  if (brightnessTick != 0 && HAL_GetTick() - brightnessTick >= 5000) {
    brightnessTick = 0;
    // write current brightness value to flash
    FLASH_EraseInitTypeDef erase;
    uint32_t pageError;

    // unlocking flash
    HAL_TRY(HAL_FLASH_Unlock());

    // erase the last page of flash memory
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = BRIGHTNESS_PAGE_ADDR;
    erase.NbPages = 1;

    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&erase, &pageError);
    if (status != HAL_OK) {
      HAL_TRY(HAL_FLASH_Lock());
      return status;
    }

    // minimum flash write resolution is 16 bit (halfword)
    uint16_t halfword = (uint16_t)brightnessVal | 0xFF00u;
    HAL_TRY(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, BRIGHTNESS_PAGE_ADDR,
                              halfword));

    HAL_TRY(HAL_FLASH_Lock());

    // diagnostic
    static uint8_t diagnosticMsg[] = "Successfully wrote brightness to flash";
    HAL_UART_Transmit_IT(uart, diagnosticMsg, 38);
  }

  // iterating through every message
  uint8_t snapshot = queuedMessages;
  for (uint8_t msgIdx = 0; msgIdx < snapshot; msgIdx++) {

    // logging the message to serial device. Not urgent so doesn't need error
    // handling
    HAL_UART_Transmit_IT(uart, queue[msgIdx].data, 8);

    queuedMessages--;

    uint8_t cmdNum = (uint8_t)(queue[msgIdx].header.StdId >> 3);
    // assuming the commandnums are contiguous and in the correct order
    if (cmdNum >= commands[0].cmdNum &&
        cmdNum <=
            commands[0].cmdNum + (sizeof(commands) / sizeof(commands[0])) - 1) {

      // executing command
      HAL_TRY(commands[cmdNum - commands[0].cmdNum].handle(&queue[msgIdx]));

    } else {
      // if no commands match
      // Maybe return a better error than this?
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

// callback for received message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  // queueing the command for processing in main loop
  if (queuedMessages < 48) {
    // no error handling in interrupt. Possibly do something to fix this
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &queue[queuedMessages].header,
                         queue[queuedMessages].data);
  }
  queuedMessages++;
}
