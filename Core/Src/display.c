/*
 * display.c
 *
 *  Created on: Jun 17, 2026
 *      Author: Luke Fadel
 */

#include "main.h"

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

void DISPLAY_OVERWRITE(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y, uint8_t *image) {

}

void DISPLAY_OR(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y, uint8_t *image) {

}
