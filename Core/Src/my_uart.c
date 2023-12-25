/*
 * my_uart.c
 *
 *  Created on: Dec 6, 2023
 *      Author: HP
 */

#include"my_uart.h"
#define	MAX_BUFFER_SIZE	30
extern UART_HandleTypeDef huart2;
//receive data
uint8_t data = 0;
// get the cmd from ring buffer
static char command[10];

static struct {
	uint8_t data[MAX_BUFFER_SIZE];
	uint32_t head;
	uint32_t tail;
	bool full;
	bool flag;
} /*brief: ring buffer to save all data received
 *
 * */ring_buffer = { .flag = 0, .head = 0, .tail = 0, .full = 0 };

static void uart_UpdateHead(char a);
/*brief: save data from a variable to ring buffer
 * para: 	none
 * retval: 	none
 * */
static void uart_SaveData(void) {
	if (ring_buffer.head == ring_buffer.tail + 1) {
		ring_buffer.full = 1;
	}
	ring_buffer.data[ring_buffer.tail++] = data;
	if (ring_buffer.tail >= MAX_BUFFER_SIZE)
		ring_buffer.tail = 0;
}

void uart_SendBuffer(void) {
	char str[100];
	uint8_t len = sprintf(str, "head:%u\n", (unsigned int) ring_buffer.head);
	HAL_UART_Transmit(&huart2, (void*) str, len, 100);
	len = sprintf(str, "tail:%u\n", (unsigned int) ring_buffer.tail);
	HAL_UART_Transmit(&huart2, (void*) str, len, 100);
	len = sprintf(str, "flag:%u\n", (unsigned int) ring_buffer.flag);
	HAL_UART_Transmit(&huart2, (void*) str, len, 100);
	len = sprintf(str, "full:%u\n", (unsigned int) ring_buffer.full);
	HAL_UART_Transmit(&huart2, (void*) str, len, 100);
}
/*brief: transmit ADC val from stm32 to the screen
 * para: 	none
 * retval: 	none
 * */
void uart_SendAdcVal(void) {
	char str[20];
	uint16_t send = fsm_value();
	uint8_t len = sprintf(str, "!ADC=%u#\n", (unsigned int) send);
	HAL_UART_Transmit(&huart2, (void*) str, len, 100);
}
/*brief: initial uart
 * para: 	none
 * retval: 	none
 * */
void uart_init(void) {
	HAL_UART_Receive_IT(&huart2, &data, 1);
}
bool uart_IsFlag(void) {
	return ring_buffer.flag;
}
void uart_ResetFlag(void) {
	ring_buffer.flag = 0;
}
bool uart_IsFull(void) {
	return ring_buffer.full;
}
/*brief: reset the ring buffer
 * para: 	none
 * retval: 	none
 * */
void uart_ResetBuffer(void) {
	ring_buffer.flag = 0;
	ring_buffer.full = 0;
	ring_buffer.head = 0;
	ring_buffer.tail = 0;
}
/*brief: 	Get the string from head to tail
 * 			Find a char in that string
 * 			Update ring_buffer.head
 * para: 	a - the char is needed to find
 * retval: 	1 - found
 * 			0 - not found
 * */
bool uart_FindChar(char a) {
	if (ring_buffer.head <= ring_buffer.tail) {
		uint8_t size = ring_buffer.tail - ring_buffer.head;
		memcpy(command, ring_buffer.data + ring_buffer.head, size);
		command[size] = '\0';
	} else {
		uint8_t size = MAX_BUFFER_SIZE - ring_buffer.head;
		memcpy(command, ring_buffer.data + ring_buffer.head, size);
		memcpy(command + size, ring_buffer.data, ring_buffer.tail);
		command[size + ring_buffer.tail] = '\0';
	}
	if (strchr(command, a)) {
		uart_UpdateHead(a);
		return 1;
	}
	return 0;
}
char* uart_GetCommand(void) {
	return command;
}
static void uart_UpdateHead(char a) {
	while (ring_buffer.data[ring_buffer.head] != a) {
		ring_buffer.head++;
		if (ring_buffer.head >= MAX_BUFFER_SIZE)
			ring_buffer.head = 0;
	}
	ring_buffer.head++;
	if (ring_buffer.head >= MAX_BUFFER_SIZE)
		ring_buffer.head = 0;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == huart2.Instance) {
		HAL_UART_Transmit(&huart2, &data, sizeof(data), 10);
		if (!ring_buffer.full) {
			uart_SaveData();
		}
		ring_buffer.flag = 1;
		HAL_UART_Receive_IT(&huart2, &data, 1);
	}
}
