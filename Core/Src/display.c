/*
 * display.c
 *
 *  Created on: Jun 17, 2026
 *      Author: Luke Fadel
 *
 *  Created for ST7796S display driver
 *  https://www.displayfuture.com/Display/datasheet/controller/ST7796s.pdf
 */

#include "main.h"
#include <stdbool.h>
#include "image.h"
#include <stdbool.h>

// bit packing to save memory
static uint8_t screenCopy[((480 * 320) + 7) / 8];

// macros to set and access bits in the array
// sets pixel to 1
#define SET_PIXEL(array, bit) ((array)[(bit) / 8] |= (1u <<((bit) % 8))) // returns void
// sets pixel to 0
#define CLR_PIXEL(arr, bit) ((arr)[(bit)/8] &= ~(1u << ((bit) % 8)))
// shifting byte to desired bit and masking off the rest of the bit
#define GET_PIXEL(array, bit) (((array)[(bit) / 8] >> ((bit) % 8)) & 1u) // returns 0u or 1u
#define OR_PIXEL(arr, bit, val) ((arr)[(bit)/8] |= ((!!(val)) << ((bit) % 8)))

// LCD hardware Reset, active low
void DISPLAY_RESET(void) {
	//setting the reset pin to low to signal a reset
	HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_Port, DISPLAY_RESET_Pin,
			GPIO_PIN_RESET);

	//small delay
	HAL_Delay(10);

	//setting the pin to high (default state)
	HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_Port, DISPLAY_RESET_Pin, GPIO_PIN_SET);

	HAL_Delay(100);

}

// LCD chip select signal, active low
void DISPLAY_SELECT(void) {
	//setting the select pin to low
	HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET);
}

void DISPLAY_DESELECT(void) {
	//setting the select pin to high
	HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
}

//sends cmd to lcd
void DISPLAY_CMD(SPI_HandleTypeDef *spi, uint8_t cmd) {
	//setting DC pin to command mode (high)
	HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);

	//selecting SPI device
	DISPLAY_SELECT();

	//using SPI to transmit data
	HAL_SPI_Transmit(spi, &cmd, 1, HAL_MAX_DELAY);

	//deselecting SPI device
	DISPLAY_DESELECT();
}

//sends data to lcd
void DISPLAY_DATA(SPI_HandleTypeDef *spi, uint8_t *data, uint16_t size) {
	//setting DC pin to command mode (low)
	HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);

	//selecting SPI device
	DISPLAY_SELECT();

	//using SPI to transmit data
	HAL_SPI_Transmit(spi, data, size, HAL_MAX_DELAY);

	//deselecting SPI device
	DISPLAY_DESELECT();
}

// initializes the ST7796S
void DISPLAY_INIT(SPI_HandleTypeDef *spi) {

	HAL_Delay(200);
	// hardware reset
	DISPLAY_RESET();
	HAL_Delay(200);

	//software reset
	DISPLAY_CMD(spi, 0x01);
	HAL_Delay(120);

	// exit sleep mode
	DISPLAY_CMD(spi, 0x11);
	HAL_Delay(120);

	// NOTE: apparently an unlock sequence is required to use these commands

	// memory data access control - instruction 36h MADCTL
	DISPLAY_CMD(spi, 0x36);
	// MX = 0, MY = 0, MV = 0, ML = 0, RGB = 0, MH = 0
	DISPLAY_DATA(spi, 0x00, 1);

	// Interface Pixel Format - instruction 3Ah COLMOD
	DISPLAY_CMD(spi, 0x3A);
	// Lowest available is 16bit/pixel
	// 01010101
	uint8_t colmod = 0x55;
	DISPLAY_DATA(spi, &colmod, 1);

	// Column inversion for display longevity
	DISPLAY_CMD(spi, 0xB4);
	uint8_t inversion = 0x01;
	DISPLAY_DATA(spi, &inversion, 1);

	// NOTE: if unlock was required before, a lock is required now

	// display on
	DISPLAY_CMD(spi, 0x29);

}

void DISPLAY_WRITE(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y,
		Image_t *image, bool overWrite) {
	// set column address command
	DISPLAY_CMD(spi, 0x2A);
	// parameters: starting col MSB, starting col LSB, ending col MSB, ending col LSB
	uint8_t caset[] = { (uint8_t) (x >> 8), (uint8_t) x, (uint8_t) ((uint8_t) (x
			+ image->width) >> 8), (uint8_t) (x + image->width) };
	DISPLAY_DATA(spi, caset, 4);

	// set row address command
	DISPLAY_CMD(spi, 0x2B);
	// parameters: starting row MSB, starting row LSB, ending row MSB, ending row LSB
	uint8_t raset[] = { (uint8_t) (y >> 8), (uint8_t) y, (uint8_t) ((uint8_t) (y
			+ image->height) >> 8), (uint8_t) (y + image->height) };
	DISPLAY_DATA(spi, raset, 4);

	// decompressing image as a bit array. Storing statically as max size on heap to prevent ram overflows
	static uint8_t dcompImage[((320 * 480) + 7) / 8];
	uint32_t count = 0;
	for (uint32_t i = 0; i < image->size; i++) {
		for (uint8_t j = 0; j < image->data[i]; j++) {
			if (i % 2) {
				//pixel is high
				SET_PIXEL(dcompImage, count);
			} else {
				//pixel is low
				CLR_PIXEL(dcompImage, count);
			}
			count++;
		}
	}

	// modifying the cloned buffer
	for (uint16_t h = 0; h < image->height; h++) {
		for (uint16_t w = 0; w < image->width; w++) {
			uint32_t globalpos = 480 * (y + h) + x + w;
			uint32_t localpos = (image->width * h) + w;
			if (overWrite) {
				if (GET_PIXEL(dcompImage, localpos)) {
					SET_PIXEL(screenCopy, globalpos);
				} else {
					CLR_PIXEL(screenCopy, globalpos);
				}
			} else {
				OR_PIXEL(screenCopy, globalpos, GET_PIXEL(dcompImage,localpos));
			}
		}
	}

//	// write data command
//	DISPLAY_CMD(spi, 0x2C);
//
//	// setting to data mode
//	HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);

	// write to display code here

}
