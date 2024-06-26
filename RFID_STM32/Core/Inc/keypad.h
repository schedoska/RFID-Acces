#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "stm32l4xx_hal.h"

/*
 * ROW 1: | 1 - 2 - 3	|
 * ROW 2: | 4 - 5 - 6	|
 * ROW 3: | 7 - 8 - 9	|
 * ROW 4: | * - 0 - #	|
 ---------|-------------|
 	 	  | C1  C2  C3	|
 */

#define NUM_OF_COLUMNS 3
#define NUM_OF_ROWS 4

#define BUTTON_DEBOUNCE_TIME_MS 20

#define ROW_1_OUT_PORT GPIOC
#define ROW_2_OUT_PORT GPIOC
#define ROW_3_OUT_PORT GPIOB
#define ROW_4_OUT_PORT GPIOB
#define COLUMN_1_IN_PORT GPIOC
#define COLUMN_2_IN_PORT GPIOB
#define COLUMN_3_IN_PORT GPIOB

#define ROW_1_OUT_PIN GPIO_PIN_8
#define ROW_2_OUT_PIN GPIO_PIN_7
#define ROW_3_OUT_PIN GPIO_PIN_15
#define ROW_4_OUT_PIN GPIO_PIN_13
#define COLUMN_1_IN_PIN GPIO_PIN_6
#define COLUMN_2_IN_PIN GPIO_PIN_14
#define COLUMN_3_IN_PIN GPIO_PIN_12

static GPIO_TypeDef *row_ports_array[] = {ROW_1_OUT_PORT,
								   	   	  ROW_2_OUT_PORT,
										  ROW_3_OUT_PORT,
										  ROW_4_OUT_PORT};

static GPIO_TypeDef *column_ports_array[] = {COLUMN_1_IN_PORT,
									  	  	 COLUMN_2_IN_PORT,
											 COLUMN_3_IN_PORT};

static uint16_t row_pins_array[] = {ROW_1_OUT_PIN,
							 	 	ROW_2_OUT_PIN,
									ROW_3_OUT_PIN,
									ROW_4_OUT_PIN};

static uint16_t column_pins_array[] = {COLUMN_1_IN_PIN,
									   COLUMN_2_IN_PIN,
									   COLUMN_3_IN_PIN};

extern int8_t last_pressed_index;

#define NO_BUTTON_PRESSED -1
#define KEYPAD_ERROR_CHAR '^'

void keypad_init();
int8_t keypad_scan_pressed_index();
int8_t keypad_scan_pressed_index_filtered();

static char keypad_index_char_array[] = {'6','4','5','9','7','8','#','*','0','3','1','2'};

char keypad_index_to_char(int8_t index);

#endif /* KEYPAD_H_ */
