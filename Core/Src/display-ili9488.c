/*
 * display.c
 *
 *  Created on: Jun 23, 2026
 *      Author: Luke Fadel
 *
 *  Created for ILI9488 display driver
 *  https://www.mouser.com/pdfDocs/ILI9488_Data_Sheet_100.pdf
 *  LSB = first pixel in byte
 */

// includes
#include "character.h"
#include "image.h"
#include "main.h"
#include <display-ili9488.h>
#include <stdbool.h>
#include <stdint.h>

// state so main functions and callbacks can all access render state
static ImageTransferState_t state;

// lookup table for possible 3 byte packages
// speed testing variables
const uint32_t table[256] = {
    0x44444444,
    0x44444484,
    0x44444448,
    0x44444488,
    0x44448444,
    0x44448484,
    0x44448448,
    0x44448488,
    0x44444844,
    0x44444884,
    0x44444848,
    0x44444888,
    0x44448844,
    0x44448884,
    0x44448848,
    0x44448888,
    0x44844444,
    0x44844484,
    0x44844448,
    0x44844488,
    0x44848444,
    0x44848484,
    0x44848448,
    0x44848488,
    0x44844844,
    0x44844884,
    0x44844848,
    0x44844888,
    0x44848844,
    0x44848884,
    0x44848848,
    0x44848888,
    0x44484444,
    0x44484484,
    0x44484448,
    0x44484488,
    0x44488444,
    0x44488484,
    0x44488448,
    0x44488488,
    0x44484844,
    0x44484884,
    0x44484848,
    0x44484888,
    0x44488844,
    0x44488884,
    0x44488848,
    0x44488888,
    0x44884444,
    0x44884484,
    0x44884448,
    0x44884488,
    0x44888444,
    0x44888484,
    0x44888448,
    0x44888488,
    0x44884844,
    0x44884884,
    0x44884848,
    0x44884888,
    0x44888844,
    0x44888884,
    0x44888848,
    0x44888888,
    0x84444444,
    0x84444484,
    0x84444448,
    0x84444488,
    0x84448444,
    0x84448484,
    0x84448448,
    0x84448488,
    0x84444844,
    0x84444884,
    0x84444848,
    0x84444888,
    0x84448844,
    0x84448884,
    0x84448848,
    0x84448888,
    0x84844444,
    0x84844484,
    0x84844448,
    0x84844488,
    0x84848444,
    0x84848484,
    0x84848448,
    0x84848488,
    0x84844844,
    0x84844884,
    0x84844848,
    0x84844888,
    0x84848844,
    0x84848884,
    0x84848848,
    0x84848888,
    0x84484444,
    0x84484484,
    0x84484448,
    0x84484488,
    0x84488444,
    0x84488484,
    0x84488448,
    0x84488488,
    0x84484844,
    0x84484884,
    0x84484848,
    0x84484888,
    0x84488844,
    0x84488884,
    0x84488848,
    0x84488888,
    0x84884444,
    0x84884484,
    0x84884448,
    0x84884488,
    0x84888444,
    0x84888484,
    0x84888448,
    0x84888488,
    0x84884844,
    0x84884884,
    0x84884848,
    0x84884888,
    0x84888844,
    0x84888884,
    0x84888848,
    0x84888888,
    0x48444444,
    0x48444484,
    0x48444448,
    0x48444488,
    0x48448444,
    0x48448484,
    0x48448448,
    0x48448488,
    0x48444844,
    0x48444884,
    0x48444848,
    0x48444888,
    0x48448844,
    0x48448884,
    0x48448848,
    0x48448888,
    0x48844444,
    0x48844484,
    0x48844448,
    0x48844488,
    0x48848444,
    0x48848484,
    0x48848448,
    0x48848488,
    0x48844844,
    0x48844884,
    0x48844848,
    0x48844888,
    0x48848844,
    0x48848884,
    0x48848848,
    0x48848888,
    0x48484444,
    0x48484484,
    0x48484448,
    0x48484488,
    0x48488444,
    0x48488484,
    0x48488448,
    0x48488488,
    0x48484844,
    0x48484884,
    0x48484848,
    0x48484888,
    0x48488844,
    0x48488884,
    0x48488848,
    0x48488888,
    0x48884444,
    0x48884484,
    0x48884448,
    0x48884488,
    0x48888444,
    0x48888484,
    0x48888448,
    0x48888488,
    0x48884844,
    0x48884884,
    0x48884848,
    0x48884888,
    0x48888844,
    0x48888884,
    0x48888848,
    0x48888888,
    0x88444444,
    0x88444484,
    0x88444448,
    0x88444488,
    0x88448444,
    0x88448484,
    0x88448448,
    0x88448488,
    0x88444844,
    0x88444884,
    0x88444848,
    0x88444888,
    0x88448844,
    0x88448884,
    0x88448848,
    0x88448888,
    0x88844444,
    0x88844484,
    0x88844448,
    0x88844488,
    0x88848444,
    0x88848484,
    0x88848448,
    0x88848488,
    0x88844844,
    0x88844884,
    0x88844848,
    0x88844888,
    0x88848844,
    0x88848884,
    0x88848848,
    0x88848888,
    0x88484444,
    0x88484484,
    0x88484448,
    0x88484488,
    0x88488444,
    0x88488484,
    0x88488448,
    0x88488488,
    0x88484844,
    0x88484884,
    0x88484848,
    0x88484888,
    0x88488844,
    0x88488884,
    0x88488848,
    0x88488888,
    0x88884444,
    0x88884484,
    0x88884448,
    0x88884488,
    0x88888444,
    0x88888484,
    0x88888448,
    0x88888488,
    0x88884844,
    0x88884884,
    0x88884848,
    0x88884888,
    0x88888844,
    0x88888884,
    0x88888848,
    0x88888888 
};
// static const uint8_t speed1[480 * 320 / 4] = { 0 };
// static const uint8_t speed2[480 * 320 / 4] = { 0 };
// uint8_t speedCount = 0;

//--------------------------------------------------------------------------------
// private functions

// LCD hardware Reset, active low
void ILI9488_RESET(void) {
  // setting the reset pin to low to signal a reset
  HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_Port, DISPLAY_RESET_Pin, GPIO_PIN_RESET);

  // small delay
  HAL_Delay(10);

  // setting the pin to high (default state)
  HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_Port, DISPLAY_RESET_Pin, GPIO_PIN_SET);

  // another delay to be safe
  HAL_Delay(100);
}

// LCD chip select signal, active low
void ILI9488_SELECT(void) {
  // setting the select pin to low
  HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
}

void ILI9488_DESELECT(void) {
  // setting the select pin to high
  HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
}

// sends command to lcd
void ILI9488_CMD(SPI_HandleTypeDef *spi, uint8_t cmd) {
  // setting DC pin to command mode (low)
  HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);

  // selecting SPI device
  ILI9488_SELECT();

  // using SPI to transmit data
  //
  HAL_SPI_Transmit(spi, &cmd, 1, HAL_MAX_DELAY);

  // deselecting SPI device
  ILI9488_DESELECT();
}

// sends data to lcd
void ILI9488_DATA(SPI_HandleTypeDef *spi, uint8_t *data, uint16_t size) {
  // setting DC pin to command mode (high)
  HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);

  // selecting SPI device
  ILI9488_SELECT();

  // using SPI to transmit data
  HAL_SPI_Transmit(spi, data, size, HAL_MAX_DELAY);

  // deselecting SPI device
  ILI9488_DESELECT();
}

//--------------------------------------------------------------------------------
// public functions

// initializes the ST7796S
void ILI9488_INIT(SPI_HandleTypeDef *spi) {
  // safety delays. can probably be removed
  HAL_Delay(200);

  // hardware reset
  ILI9488_RESET();
  HAL_Delay(200);

  // software reset
  ILI9488_CMD(spi, 0x01);
  HAL_Delay(120);

  // exit sleep mode
  ILI9488_CMD(spi, 0x11);
  HAL_Delay(120);

  // backlight on
  HAL_GPIO_WritePin(DISPLAY_LED_GPIO_Port, DISPLAY_LED_Pin, GPIO_PIN_SET);

  // configuring extended command set for spi write
  ILI9488_CMD(spi, 0xF7);
  uint8_t unlockData[2] = {0xA9, 0x51, 0x2C, 0x82};
  ILI9488_DATA(spi, &unlockData[0], 2);

  // memory data access control - instruction 36h MADCTL
  ILI9488_CMD(spi, 0x36);
  // same as st7796s
  uint8_t madctl = 0x20;
  ILI9488_DATA(spi, &madctl, 1);

  // Interface Pixel Format - instruction 3Ah COLMOD
  ILI9488_CMD(spi, 0x3A);
  // Lowest available is 3bit/pixel
  // 00000001
  // format: R G B 0 R G B 0
  // each byte = two pixels due to padding
  uint8_t colmod = 0x01;
  ILI9488_DATA(spi, &colmod, 1);

  // Column inversion for display longevity
  // update: seems to cause flicker on this display?
  // 1 dot mode
  //	ILI9488_CMD(spi, 0xB4);
  //	uint8_t inversion = 0x01;
  //	ILI9488_DATA(spi, &inversion, 1);

  // display on
  ILI9488_CMD(spi, 0x29);
}

// testing function to see fill direction and measure min refresh speed
// currently designed for reliability and testing purposes, but can be altered
// to be fast
void ILI9488_FILL(SPI_HandleTypeDef *spi) {
  // set column address command
  ILI9488_CMD(spi, 0x2A);
  // parameters: starting col MSB, starting col LSB, ending col MSB, ending col
  // LSB
  uint8_t caset[] = {

      (uint8_t)(0),

      (uint8_t)(0),

      (uint8_t)(ILI9488_WIDTH >> 8),

      (uint8_t)(ILI9488_WIDTH & 0xFF)};

  ILI9488_DATA(spi, caset, 4);

  // set page address command
  ILI9488_CMD(spi, 0x2B);
  // parameters: starting row MSB, starting row LSB, ending row MSB, ending row
  // LSB
  uint8_t paset[] = {

      (uint8_t)(0),

      (uint8_t)(0),

      (uint8_t)(ILI9488_HEIGHT >> 8),

      (uint8_t)(ILI9488_HEIGHT & 0xFF)};

  ILI9488_DATA(spi, paset, 4);

  // write data command
  ILI9488_CMD(spi, 0x2C);

  // setting to data mode
  HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);

  // selecting spi device
  ILI9488_SELECT();
  // each byte contains two bits
  uint8_t padded = 238;
  for (uint32_t r = 0; r < 76800; r++) {
    HAL_SPI_Transmit(spi, &padded, 1, HAL_MAX_DELAY);
  }
  //	HAL_Delay(500);
  //	HAL_SPI_Transmit_DMA(spi, speed1, sizeof(speed1));

  //	speedCount = 1;
  ILI9488_DESELECT();
}

// speed testing function for ILI9488_FILL
// void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
//	if (hspi->Instance == SPI1) {
//		if (speedCount < 1){
//			ILI9488_DESELECT();
//		}else{
//			HAL_SPI_Transmit_DMA(hspi, speed2, sizeof(speed2));
//			speedCount--;
//		}
//	}
//}

// fast refreshing using chunking
bool ILI9488_REFRESH(SPI_HandleTypeDef *spi) {
  // checking if the display is already being modified
  if (!state.currentlyDrawing) {
    // setting status to busy

    state.currentlyDrawing = true;

    // set column address command
    ILI9488_CMD(spi, 0x2A);
    // parameters: starting col MSB, starting col LSB, ending col MSB, ending
    // col LSB
    uint8_t caset[] = {

        (uint8_t)(0),

        (uint8_t)(0),

        (uint8_t)((ILI9488_WIDTH - 1) >> 8),

        (uint8_t)((ILI9488_WIDTH - 1) & 0xFF)};

    ILI9488_DATA(spi, caset, 4);

    // set row address command
    ILI9488_CMD(spi, 0x2B);
    // parameters: starting row MSB, starting row LSB, ending row MSB, ending
    // row LSB
    uint8_t raset[] = {

        (uint8_t)(0),

        (uint8_t)(0),

        (uint8_t)((ILI9488_HEIGHT - 1) >> 8),

        (uint8_t)((ILI9488_HEIGHT - 1) & 0xFF)};

    ILI9488_DATA(spi, raset, 4);

    // write data command
    ILI9488_CMD(spi, 0x2C);

    // setting to data mode
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);

    // selecting spi device
    ILI9488_SELECT();

    state.activeBuf = 0;
    state.imageProgress = 0;
    // setting state to reuse the other callback
    state.dcompImage_SIZE = ILI9488_WIDTH * ILI9488_HEIGHT;
    state.imageTarget = state.dcompImage_SIZE / 2;
    state.width = ILI9488_WIDTH / 8;
    state.height = ILI9488_HEIGHT;
    state.x = 0;
    state.y = 0;

    // PRELOADING FIRST TWO CHUNKS
    for (uint32_t i = 0; i < CHUNK / 4; i++) {
      ((uint32_t *)state.buf[state.activeBuf])[i] = table[state.screenCopy[i]];
    }

    state.imageProgress += CHUNK;
    state.activeBuf = !state.activeBuf;
    uint16_t shift = state.imageProgress / 4;
    for (uint32_t i = 0; i < CHUNK / 4; i++) {
      ((uint32_t *)state.buf[state.activeBuf])[i] =
          table[state.screenCopy[i + shift]];
    }

    // transmitting the first chunk, then the interrupt will transmit the
    // second chunk before loading the third etc
    HAL_SPI_Transmit_DMA(spi, state.buf[!state.activeBuf], CHUNK);

    return true;
  } else {
    return false;
  }
}

// testing function to make sure written images are in the right spot
// can be altered for speed later in development
bool ILI9488_REFRESH_DEBUG(SPI_HandleTypeDef *spi) {
  // checking if the display is already being modified
  if (!state.currentlyDrawing) {
    // setting status to busy

    state.currentlyDrawing = true;

    // set column address command
    ILI9488_CMD(spi, 0x2A);
    // parameters: starting col MSB, starting col LSB, ending col MSB, ending
    // col LSB
    uint8_t caset[] = {

        (uint8_t)(0),

        (uint8_t)(0),

        (uint8_t)((ILI9488_WIDTH - 1) >> 8),

        (uint8_t)((ILI9488_WIDTH - 1) & 0xFF)};

    ILI9488_DATA(spi, caset, 4);

    // set row address command
    ILI9488_CMD(spi, 0x2B);
    // parameters: starting row MSB, starting row LSB, ending row MSB, ending
    // row LSB
    uint8_t raset[] = {

        (uint8_t)(0),

        (uint8_t)(0),

        (uint8_t)((ILI9488_HEIGHT - 1) >> 8),

        (uint8_t)((ILI9488_HEIGHT - 1) & 0xFF)};

    ILI9488_DATA(spi, raset, 4);

    // write data command
    ILI9488_CMD(spi, 0x2C);

    // setting to data mode
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);

    // selecting spi device
    ILI9488_SELECT();

    //		for (uint32_t i = 0; i < (480 * 320) / 2; i++) {
    //			uint8_t px = (
    //					GET_PIXEL(state.screenCopy, i * 2) ?
    // 0xE0000      : 0) 					|
    // (GET_PIXEL(state.screenCopy, 1 + i * 2) ?
    // 14 : 0); 			HAL_SPI_Transmit(spi, &px, 1,
    // HAL_MAX_DELAY);
    //
    //		}

    // using lookup table to expand bit packed pixels into the correct data
    // pattern for the display
    // R G B P R G B P
    for (uint32_t i = 0; i < sizeof(state.screenCopy); i++) {
      HAL_SPI_Transmit(spi, (uint8_t *)&table[state.screenCopy[i]], 4,
                       HAL_MAX_DELAY);
    }
    ILI9488_DESELECT();
    state.currentlyDrawing = false;
    return true;
  } else {
    return false;
  }
}
bool ILI9488_LOAD_IMAGE_DEBUG(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y,
                              Image_t *image, bool overWrite) {
  state.currentlyLoading = true;
  // decompressing image to cloned buffer
  uint32_t startTime = HAL_GetTick();
  // size of visible image
  state.dcompImage_SIZE = 0;

  // all pixels in image including ones that are clipped off by the edge of
  // copying state variables for compiler optimization (pointer aliasing)
  uint16_t row = 0;                 // in pixels
  uint16_t col = 0;                 // in pixels
  uint16_t imgWidth = image->width; // in pixels
  uint32_t decompiledImageSize = 0; // in pixels
  uint8_t *imgData = image->data;

  // iterating through compressed image
  for (uint32_t i = 0; i < image->size; i++) {

    // iterating through the current RLE value
    for (uint8_t j = 0; j < imgData[i]; j++) {
      // if the current pixel is in bounds of the screen
      if (col + x < ILI9488_WIDTH && row + y < ILI9488_HEIGHT) {
        // calculating screen pixel index from current position within image
        uint32_t globalpos = ILI9488_WIDTH * (y + row) + x + col;

        if (i % 2) {
          // pixel is high, 1 % 2 = 1
          //  will always be high in overwrite and OR mode
          SET_PIXEL(state.screenCopy, globalpos);
        } else if (overWrite) {
          // pixel is low and overwrite is on. if in OR mode, just leave
          // current pixel as it is
          CLR_PIXEL(state.screenCopy, globalpos);
        }
        decompiledImageSize++;
      }
      // incrementing the column and width
      if (++col == imgWidth) {
        col = 0;
        row++;
      }
    }
  }

  uint32_t finalTime = HAL_GetTick() - startTime;

  state.x = x;
  state.y = y;
  state.width = image->width;
  state.height = image->height;
  state.dcompImage_SIZE = decompiledImageSize;

  // temporary call for convenience. TODO remove later
  ILI9488_DRAW(spi);
  state.currentlyLoading = false;

  return true;
}

// loads image to screen buffer
bool ILI9488_LOAD_IMAGE(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y,
                        Image_t *image, bool overWrite) {
  state.currentlyLoading = true;
  // decompressing image to cloned buffer
  uint32_t startTime = HAL_GetTick();
  // size of visible image
  state.dcompImage_SIZE = 0;

  // all pixels in image including ones that are clipped off by the edge of
  // copying state variables for compiler optimization (pointer aliasing)
  uint16_t row = 0;                            // in pixels
  uint16_t col = 0;                            // in pixels
  uint16_t imgWidth = image->width;            // in pixels
  uint32_t decompiledImageSize = 0;            // in pixels
  uint32_t pxOffset = (y * ILI9488_WIDTH) + x; // in pixels
  uint8_t *imgData = image->data;
  bool isColInBounds = x + imgWidth < ILI9488_WIDTH;

  // iterating through compressed image
  for (uint32_t i = 0; i < image->size; i++) {

    // faster loading algorithm
    bool isOn = i % 2;
    uint8_t remainingPx = imgData[i]; // in pixels

    while (remainingPx > 0) {
      // filling all the pixels on the current row of the image
      // if the thing is out of range in the y direction, always normal
      //--------------------------------FLAG
      uint16_t chunk;
      if (isColInBounds || !(y + row < ILI9488_HEIGHT) ||
          (x + col >= ILI9488_WIDTH)) {
        chunk = imgWidth - col;
      } else {
        chunk = ILI9488_WIDTH - (x + col);
      }
      // if chunk is more than the remaining pixels needed
      chunk = remainingPx > chunk ? chunk : remainingPx;
      remainingPx -= chunk;
      // incrementing row and column
      // checking if current position is in the bounds of the screen to load the
      // correct pixels
      if (x + col < ILI9488_WIDTH && y + row < ILI9488_HEIGHT) {
        uint16_t thisChunk = chunk;

        // loading pixels
        decompiledImageSize += thisChunk;

        // uint32_t globalpos =
        //     pxOffset + (row * ILI9488_WIDTH) + col; // in pixels
        uint32_t globalpos = (uint32_t)(y + row) * ILI9488_WIDTH + (x + col);

        // leading
        // if the current position isn't byte aligned
        if (globalpos % 8 != 0) {
          uint8_t leading = 8 - (globalpos % 8);
          leading = leading > thisChunk ? thisChunk : leading;
          if (isOn) {
            // write pixels
            // state.screenCopy[globalpos / 8] |= (0xFF >> leading) << leading;
            state.screenCopy[globalpos / 8] |= 0xFF << (8 - leading);
          } else if (overWrite) {
            // clear pixels
            // state.screenCopy[globalpos / 8] =
            //     (state.screenCopy[globalpos / 8] << leading) >> leading;
            state.screenCopy[globalpos / 8] &= 0xFF >> leading;
          }
          thisChunk -= leading;
          globalpos += leading;
        }

        // filling middle
        for (uint8_t byte = 0; byte < thisChunk / 8; byte++) {
          if (isOn) {
            // write byte
            state.screenCopy[globalpos / 8] = 0xFF;
          } else if (overWrite) {
            // clear byte
            state.screenCopy[globalpos / 8] = 0;
          }

          globalpos += 8;
        }

        // filling trailing
        uint8_t trailing = thisChunk % 8;
        if (trailing != 0) {
          if (isOn) {
            state.screenCopy[globalpos / 8] |= 0xFF >> (8 - trailing);
          } else if (overWrite) {
            // state.screenCopy[globalpos / 8] &= 0xFF >> (8 - trailing);
            state.screenCopy[globalpos / 8] &= 0xFF << trailing;
          }
        }
      }
      if ((col += chunk) >= imgWidth) {
        col = 0;
        row++;
      }
    }
  }

  uint32_t finalTime = HAL_GetTick() - startTime;

  state.x = x;
  state.y = y;
  state.width = image->width;
  state.height = image->height;
  state.dcompImage_SIZE = decompiledImageSize;

  // temporary call for convenience. TODO remove later
  ILI9488_DRAW(spi);
  state.currentlyLoading = false;

  return true;
}

bool ILI9488_LOAD_TEXT(
    SPI_HandleTypeDef *spi, uint16_t x, uint16_t y, uint8_t text[],
    Character_t *font,
    /*width of character in pixels*/ uint8_t characterWidth,
    /*number of characters in font*/ size_t fontSize,
    /*height of character in pixels*/ size_t characterHeight) {

  // parsing the text

  uint32_t startTime = HAL_GetTick();
  uint16_t charCount = 0;
  uint32_t decompiledImageSize = 0; // in pixels
  // iterating through every inputted character
  for (uint16_t charIdx = 0; text[charIdx] != 0; charIdx++) {
    if (charIdx > (ILI9488_WIDTH - x) / characterWidth) {
      // cutting off string optimization
      break;
    }

    // loading character
    uint16_t startCol = ((charIdx * characterWidth) + x) / 8; // in bytes
    uint16_t col = 0;                                         // in bytes
    uint16_t row = 0;                                         // in pixels
    uint8_t *currentCharacter =
        (text[charIdx] < 32 || (size_t)(text[charIdx] - 32) >= fontSize)
            ? font[0].data
            : font[text[charIdx] - 32].data;
    uint16_t byteOffset = (ILI9488_SCALED_WIDTH * y) + startCol; // in bytes
    uint16_t scaledCharacterWidth = characterWidth / 8;          // in bytes

    // loading one byte at a time. This can be done easily as the screen width
    // is a multiple of 8, the x coordinate is a multiple of 8, and the font
    // width is a multiple of 8
    for (uint32_t byte = 0; byte < (characterWidth * characterHeight) / 8;
         byte++) {
      uint32_t globalpos = byteOffset + (ILI9488_SCALED_WIDTH * row) + col;
      // if the current byte is in bounds of the screen
      if (col + startCol < ILI9488_SCALED_WIDTH && row + y < ILI9488_HEIGHT) {
        // or mode
        state.screenCopy[globalpos] |= currentCharacter[byte];
        decompiledImageSize += 8;
      }
      // incrementing colmn and row
      if (++col >= scaledCharacterWidth) {
        col = 0;
        row++;
      }
    }
    charCount++;
  }

  uint32_t finalTime = startTime - HAL_GetTick();

  state.x = x;
  state.y = y;
  state.width = charCount * characterWidth;
  state.height = characterHeight;
  state.dcompImage_SIZE = decompiledImageSize;

  return true;
}

// x and y should be multiples of 8
// draws last loaded image to screen
// TODO: make a fast refresh
bool ILI9488_DRAW(SPI_HandleTypeDef *spi) {
  if (!state.currentlyDrawing) {
    // setting status to busy

    state.currentlyDrawing = true;
    // set column address command
    ILI9488_CMD(spi, 0x2A);
    // parameters: starting col MSB, starting col LSB, ending col MSB, ending
    // col LSB
    uint8_t caset[] = {

        (uint8_t)(state.x >> 8),

        (uint8_t)(state.x & 0xFF),

        (uint8_t)((state.x + state.width - 1 > ILI9488_WIDTH - 1
                       ? ILI9488_WIDTH - 1
                       : state.x + state.width - 1) >>
                  8),

        (uint8_t)((state.x + state.width - 1 > ILI9488_WIDTH - 1
                       ? ILI9488_WIDTH - 1
                       : state.x + state.width - 1) &
                  0xFF)

    };

    ILI9488_DATA(spi, caset, 4);

    // set row address command
    ILI9488_CMD(spi, 0x2B);
    // parameters: starting row MSB, starting row LSB, ending row MSB, ending
    // row LSB
    uint8_t raset[] = {

        (uint8_t)(state.y >> 8),

        (uint8_t)(state.y & 0xFF),

        (uint8_t)((state.y + state.height - 1 > ILI9488_HEIGHT - 1
                       ? ILI9488_HEIGHT - 1
                       : state.y + state.height - 1) >>
                  8),

        (uint8_t)((state.y + state.height - 1 > ILI9488_HEIGHT - 1
                       ? ILI9488_HEIGHT - 1
                       : state.y + state.height - 1) &
                  0xFF)};

    ILI9488_DATA(spi, raset, 4);

    if (state.x + state.width > ILI9488_WIDTH) {
      state.width = ILI9488_WIDTH - state.x;
    }

    if (state.y + state.height > ILI9488_HEIGHT) {
      state.height = ILI9488_HEIGHT - state.y;
    }

    state.x /= 8;
    state.width /= 8;

    // write data command
    ILI9488_CMD(spi, 0x2C);

    // setting to data mode
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);

    // selecting spi device
    ILI9488_SELECT();

    // double buffering

    // sending image data. chunking data for DMA and memory saving purposes
    state.imageProgress = 0;

    state.imageTarget = state.dcompImage_SIZE / 2;

    state.activeBuf = 0;
    // checking if chunking is required or not
    if (state.imageTarget <= CHUNK) {
      // not required

      // expanding to buffer
      // scan 8 pixels at once, = 4 bytes
      for (uint32_t i = 0; i < state.imageTarget / 4; i++) {
        // converting image space index to screen space index
        uint32_t globalpos =
            ILI9488_SCALED_WIDTH * (state.y + (i / state.width)) + state.x +
            (i % state.width);
        ((uint32_t *)state.buf[state.activeBuf])[i] =
            table[state.screenCopy[globalpos]];
      }

      HAL_SPI_Transmit_DMA(spi, state.buf[state.activeBuf], state.imageTarget);

      state.imageProgress = state.imageTarget;
    } else {
      // required
      // PRELOADING FIRST TWO CHUNKS
      for (uint32_t i = 0; i < CHUNK / 4; i++) {
        // converting image space index to screen space index
        uint32_t globalpos =
            ILI9488_SCALED_WIDTH * (state.y + (i / state.width)) + state.x +
            (i % state.width);
        ((uint32_t *)state.buf[state.activeBuf])[i] =
            table[state.screenCopy[globalpos]];
      }

      state.imageProgress += CHUNK;
      state.activeBuf = !state.activeBuf;
      uint16_t shift = state.imageProgress / 4;
      for (uint32_t i = 0;
           i < (state.imageTarget - state.imageProgress < CHUNK
                    ? (state.imageTarget - state.imageProgress) / 4
                    : CHUNK / 4);
           i++) {
        // converting image space index to screen space index
        uint32_t globalpos =
            ILI9488_SCALED_WIDTH * (state.y + ((i + shift) / state.width)) +
            state.x + ((i + shift) % state.width);
        ((uint32_t *)state.buf[state.activeBuf])[i] =
            table[state.screenCopy[globalpos]];
      }
      // transmitting the first chunk, then the interrupt will transmit the
      // second chunk before loading the third etc
      HAL_SPI_Transmit_DMA(spi, state.buf[!state.activeBuf], CHUNK);
      state.startTime = HAL_GetTick();
    }
    return true;
  } else {
    // called if function is already drawing when called
    return false;
  }
}

// callback that is called when HAL_SPI_Transmit_DMA finishes
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  if (hspi->Instance == SPI1) {
    if (state.imageProgress >= state.imageTarget) {
      // done drawing
      ILI9488_DESELECT();
      state.currentlyDrawing = 0;
      state.finalTime = HAL_GetTick() - state.startTime;
      return;
    } else if (state.imageTarget - state.imageProgress < CHUNK) {
      // partial chunk
      HAL_SPI_Transmit_DMA(hspi, state.buf[state.activeBuf],
                           state.imageTarget - state.imageProgress);
      // done drawing condition
      state.imageProgress += CHUNK;
    } else {
      // full chunk

      state.imageProgress += CHUNK;
      HAL_SPI_Transmit_DMA(hspi, state.buf[state.activeBuf], CHUNK);
      state.activeBuf = !state.activeBuf;
      const uint16_t shift = state.imageProgress / 4;
      const uint16_t y = state.y;
      const uint16_t x = state.x;
      const uint16_t width = state.width;
      // pointer to the active 4 byte section of memory
      uint32_t *dst = (uint32_t *)state.buf[state.activeBuf];
      for (uint32_t i = 0; i < CHUNK / 4; i++) {
        // converting image space index to screen space index
        // this line is heavily compiler optimized because any obvious
        // optimizations make it slower
        uint32_t globalpos =
            ILI9488_SCALED_WIDTH * (y + ((i + shift) / width)) + x +
            ((i + shift) % width);
        *dst++ = table[state.screenCopy[globalpos]];
      }
    }
  }
}
