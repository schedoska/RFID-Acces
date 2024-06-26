#include "keypad.h"

int8_t last_pressed_index = NO_BUTTON_PRESSED;

void keypad_init(){
	for(uint8_t l_row = 0; l_row < NUM_OF_ROWS; l_row++)
		HAL_GPIO_WritePin(row_ports_array[l_row],
						  row_pins_array[l_row],
						  GPIO_PIN_SET);
}

int8_t keypad_scan_pressed_index(){
	for(uint8_t l_row = 0; l_row < NUM_OF_ROWS; l_row++){
		HAL_GPIO_WritePin(row_ports_array[l_row],
						  row_pins_array[l_row],
						  GPIO_PIN_RESET);
		for(uint8_t l_col = 0; l_col < NUM_OF_COLUMNS; l_col++){
			if(HAL_GPIO_ReadPin(column_ports_array[l_col],
								column_pins_array[l_col]) == GPIO_PIN_RESET){
				keypad_init();
				return (l_row * 3 + l_col);
			}
		}
		HAL_GPIO_WritePin(row_ports_array[l_row],
						  row_pins_array[l_row],
						  GPIO_PIN_SET);
	}
	return NO_BUTTON_PRESSED;
}

char keypad_index_to_char(int8_t index){
	if(index < 0 || index > 11) return KEYPAD_ERROR_CHAR;
	else return keypad_index_char_array[index];
}

int8_t keypad_scan_pressed_index_filtered(){
	int8_t index = keypad_scan_pressed_index();
	if(index != last_pressed_index)
	{
		last_pressed_index = index;
		return index;
	}
	else
	{
		return NO_BUTTON_PRESSED;
	}
}




