/*
 * my_uart.h
 *
 *  Created on: Dec 6, 2023
 *      Author: HP
 */

#ifndef INC_MY_UART_H_
#define INC_MY_UART_H_

#include "my_define.h"
#include "my_fsm.h"
void uart_init(void);
bool uart_IsFlag(void);
void uart_ResetFlag(void);
void uart_ResetBuffer(void);
bool uart_IsFull(void) ;
bool uart_FindChar(char a);
char* uart_GetCommand(void);
void uart_SendAdcVal(void);
void uart_SendBuffer(void);
#endif /* INC_MY_UART_H_ */
