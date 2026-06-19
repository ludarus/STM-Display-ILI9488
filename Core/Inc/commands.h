/*
 * commands.h
 *
 *  Created on: Jun 18, 2026
 *      Author: Luke Fadel
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

uint8_t* getCommandBuffer(void);
size_t getCommandSize(void);

typedef struct {
	const uint8_t* keyword;
	const uint16_t keyword_size;
	const uint8_t* (*action)(void);
} Command_t;

#endif /* INC_COMMANDS_H_ */
