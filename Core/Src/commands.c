/*
 * commands.c
 *
 *  Created on: Jun 19, 2026
 *      Author: Luke Fadel
 */
#include "main.h"
#include "commands.h"
#include <memory.h>

const uint8_t errorMsg[] = "\n\rCommand not found\r\n";


// command action functions
const uint8_t* helpCMD(void) {
	return (const uint8_t*)"HELP MENU HERE";
}

const Command_t commands[] = {
		{
				.keyword = (const uint8_t*)"HELP\n",
				.keyword_size = 4,
				.action = helpCMD
		},
};

uint8_t commandBuffer[255];

//public getter functions
uint8_t* getCommandBuffer(void) {
	return commandBuffer;
}

size_t getCommandSize(void) {
	return sizeof(commandBuffer);
}

//private
void parseCommand(UART_HandleTypeDef *huart, uint16_t size) {
	for (uint8_t i = 0; i < sizeof(commands)/sizeof(commands[0]) ; i++){
		// if the received command equals a command in the command list
	    HAL_UART_Transmit_IT(huart, (const uint8_t*)"h", 1);
		if (size == commands[i].keyword_size &&
		    memcmp(commandBuffer, commands[i].keyword, size) == 0)
		{
		    const uint8_t *response = commands[i].action();

		    //sending response
		    HAL_UART_Transmit_IT(huart, response, sizeof(response) - 1);
		    return;
		}
	}

	// if no matching command was found
    HAL_UART_Transmit_IT(huart, errorMsg, sizeof(errorMsg) - 1);

}

//on receive
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	//restarting receive
	HAL_UARTEx_ReceiveToIdle_DMA(huart, commandBuffer, sizeof(commandBuffer));

	//parsing command
	parseCommand(huart, Size);
}

