/*
 * my_fsm.h
 *
 *  Created on: Dec 6, 2023
 *      Author: HP
 */

#ifndef INC_MY_FSM_H_
#define INC_MY_FSM_H_

#include"my_scheduler.h"
#include "my_define.h"
#include"my_uart.h"
#include "my_adc.h"
uint16_t fsm_value(void);
void uart_communiation_fsm(void);
void command_parser_fsm(void);
#endif /* INC_MY_FSM_H_ */
