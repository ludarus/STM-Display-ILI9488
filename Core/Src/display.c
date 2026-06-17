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
void DISPLAY_DATA(SPI_HandleTypeDef *spi, uint8_t cmd, uint16_t size) {
	//setting DC pin to command mode (low)
	HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);

	//selecting SPI device
	DISPLAY_SELECT();

	//using SPI to transmit data
	HAL_SPI_Transmit(spi, &cmd, size, HAL_MAX_DELAY);

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
	DISPLAY_DATA(spi, 0x55, 1);

	// Column inversion for display longevity
	DISPLAY_CMD(spi, 0xB4);
	DISPLAY_DATA(spi, 0x01, 1);

	// NOTE: if unlock was required before, a lock is required now

	// display on
	DISPLAY_CMD(spi, 0x29);

}

void DISPLAY_WRITE(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y,
		uint8_t *image, bool overWrite) {

}
