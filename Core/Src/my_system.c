/*
 * my_system.c
 *
 *  Created on: Dec 6, 2023
 *      Author: HP
 */


#include"my_system.h"


#ifdef TEST
static void toggle_led(void){
	HAL_GPIO_TogglePin(DEBUG_LED_PORT, DEBUG_LED_PIN);
}
#endif

void init(void) {
	sch_init();
	uart_init();
	adc_init();
#ifdef TEST
	sch_add_task(toggle_led, 0, ONE_SECOND);

	//sch_add_task(uart_SendBuffer, 0, 1000);
#endif
	sch_add_task(command_parser_fsm, 0, 10);
	sch_add_task(uart_communiation_fsm, 0, 10);
}
void loop(void) {
	sch_dispatch();
}
