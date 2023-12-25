/*
 * my_fsm.c
 *
 *  Created on: Dec 6, 2023
 *      Author: HP
 */

#include "my_fsm.h"

#define RST_CMD	"!RST#"
#define OK_CMD	"!OK#"
static enum {
	READ, HANDLE
} command_parser_st;
static enum {
	WAIT, SEND
} uart_communiation_st;
static bool command_flag = 0;
static char command_data[20];
static uint16_t value;
/*
 * format command
 * !COM#*/
/*
 * brief: 	command_parser_fsm
 * para: 	none
 * retval: 	none*/
void command_parser_fsm(void) {
	switch (command_parser_st) {
	case READ:
		if (uart_IsFlag()) {
			if (uart_IsFull()) {
				uart_ResetBuffer();
			}
			if (uart_FindChar('#')) {
				command_parser_st = HANDLE;
			}
			uart_ResetFlag();
		}
		break;
	case HANDLE:
		command_flag = 1;
		strcpy(command_data, uart_GetCommand());
		command_parser_st = READ;
		break;
	}
}
/*
 * brief: 	uart_communiation_fsm
 * para: 	none
 * retval: 	none*/
void uart_communiation_fsm(void) {
	switch (uart_communiation_st) {
	case WAIT:
		value = adc_GetValue();
		if (command_flag) {
			if (!strcmp(command_data, RST_CMD)) {
				sch_add_task(uart_SendAdcVal, 0, 3 * ONE_SECOND);
				uart_communiation_st = SEND;
			}
			command_flag = 0;
		}
		break;
	case SEND:
		if (command_flag) {
			if (!strcmp(command_data, OK_CMD)){
				sch_remove_task(uart_SendAdcVal);
				uart_communiation_st = WAIT;
			}
			command_flag = 0;
		}
		break;
	}
}
uint16_t fsm_value(void){
	return value;
}
