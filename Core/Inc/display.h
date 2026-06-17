/*
 * display.h
 *
 *  Created on: Jun 17, 2026
 *      Author: Luke Fadel
 */
#include <stdbool.h>

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

void DISPLAY_INIT(SPI_HandleTypeDef *spi);
void DISPLAY_RESET(void);
void DISPLAY_SELECT(void);
void DISPLAY_DESELECT(void);
void DISPLAY_CMD(SPI_HandleTypeDef *spi, uint8_t cmd);
void DISPLAY_DATA(SPI_HandleTypeDef *spi, uint8_t cmd, uint16_t size);
void DISPLAY_WRITE(SPI_HandleTypeDef *spi, uint16_t x, uint16_t y, uint8_t *image, bool overWrite);

#endif /* INC_DISPLAY_H_ */
