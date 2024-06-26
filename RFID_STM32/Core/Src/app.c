#include "app.h"

APP_STATE current_app_state = DEFAULT_STATE;
APP_STATE prev_app_state = WRITE_PASSWORD_STATE;

uint8_t password_buffer_index = 0;
uint8_t menu_page_index = 0;

char password_buffer[4];
char temp_password_buffor[4];

uint8_t tim_flag  = 0;
uint8_t uart_flag = 0;

uint8_t RFID_buffer[RFID_FRAME_SIZE];

void app_default_state(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != DEFAULT_STATE)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		lcd_clear(i2c_handle);
		lcd_send_text("Doors closed", i2c_handle);
		prev_app_state = DEFAULT_STATE;
		uart_flag = 0;
	}

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char != '#' && pressed_key_char != '*')
		{
			password_buffer_index = 0;
			password_buffer[password_buffer_index++] = pressed_key_char;
			current_app_state = WRITE_PASSWORD_STATE;
		}
	}
	if(uart_flag == 1)
	{
		uart_flag = 0;
		if(find_tag(RFID_buffer + 7))
		{
			lcd_clear(i2c_handle);
			lcd_send_text("Access allowed", i2c_handle);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_Delay(DOORS_OPEN_TIME_MS);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			prev_app_state = WRITE_PASSWORD_STATE;
			uart_flag = 0;
		}
		else
		{
			lcd_clear(i2c_handle);
			lcd_send_text("Access denied", i2c_handle);
			HAL_Delay(ACCESS_DENIED_SCREEN_TIME_MS);
			prev_app_state = WRITE_PASSWORD_STATE;
			uart_flag = 0;
		}
	}
}

void app_write_password_state(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != WRITE_PASSWORD_STATE)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Password:", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_data('*', i2c_handle);
		prev_app_state = WRITE_PASSWORD_STATE;

		TIM2->ARR = MAX_PASSWORD_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
	}

	if(tim_flag == 1) current_app_state = DEFAULT_STATE;

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '#' || pressed_key_char == '*')
		{
			current_app_state = DEFAULT_STATE;
			return;
		}
		password_buffer[password_buffer_index++] = pressed_key_char;
		lcd_send_data('*', i2c_handle);
		if(password_buffer_index > 3)
		{
			if(app_is_password_correct())
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
				current_app_state = MENU_STATE_PAGE_1;
			}
			else
			{
				lcd_clear(i2c_handle);
				lcd_send_text("Wrong password", i2c_handle);
				HAL_Delay(WRONG_PASSWORD_SCREEN_TIME_MS);
				current_app_state = DEFAULT_STATE;
			}
		}
	}
}

void app_menu_state_page_1(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != MENU_STATE_PAGE_1)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Settings   [1/4]", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("Add user", i2c_handle);
		prev_app_state = MENU_STATE_PAGE_1;

		TIM2->ARR = MAX_MENU_PAGE_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
	}

	if(tim_flag == 1) current_app_state = DEFAULT_STATE;

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '0')
		{
			current_app_state = ADD_USER_STATE;
		}
		else if(pressed_key_char == '#')
		{
			current_app_state = MENU_STATE_PAGE_2;
		}
		else if(pressed_key_char == '*')
		{
			current_app_state = MENU_STATE_PAGE_4;
		}
	}
}

void app_menu_state_page_2(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != MENU_STATE_PAGE_2)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Settings   [2/4]", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("Delete user", i2c_handle);
		prev_app_state = MENU_STATE_PAGE_2;

		TIM2->ARR = MAX_MENU_PAGE_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
	}

	if(tim_flag == 1) current_app_state = DEFAULT_STATE;

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '0')
		{
			current_app_state = DELETE_USER_STATE;
		}
		else if(pressed_key_char == '#')
		{
			current_app_state = MENU_STATE_PAGE_3;
		}
		else if(pressed_key_char == '*')
		{
			current_app_state = MENU_STATE_PAGE_1;
		}
	}
}

void app_menu_state_page_3(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != MENU_STATE_PAGE_3)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Settings   [3/4]", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("Change passw.", i2c_handle);
		prev_app_state = MENU_STATE_PAGE_3;

		TIM2->ARR = MAX_MENU_PAGE_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
	}

	if(tim_flag == 1) current_app_state = DEFAULT_STATE;

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '0')
		{
			current_app_state = CHANGE_PASSWORD_STATE;
		}
		else if(pressed_key_char == '#')
		{
			current_app_state = MENU_STATE_PAGE_4;
		}
		else if(pressed_key_char == '*')
		{
			current_app_state = MENU_STATE_PAGE_2;
		}
	}
}

void app_menu_state_page_4(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != MENU_STATE_PAGE_4)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Settings   [4/4]", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("Clear database", i2c_handle);
		prev_app_state = MENU_STATE_PAGE_4;

		TIM2->ARR = MAX_MENU_PAGE_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
	}

	if(tim_flag == 1) current_app_state = DEFAULT_STATE;

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '0')
		{
			current_app_state = CLEAR_DATABASE_STATE;
		}
		else if(pressed_key_char == '#')
		{
			current_app_state = MENU_STATE_PAGE_1;
		}
		else if(pressed_key_char == '*')
		{
			current_app_state = MENU_STATE_PAGE_3;
		}
	}
}

void app_add_user_state(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != ADD_USER_STATE)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Add user:", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("Place card to add", i2c_handle);
		prev_app_state = ADD_USER_STATE;

		TIM2->ARR = MAX_ADD_USER_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
		uart_flag = 0;
		HAL_Delay(100);
	}

	if(tim_flag == 1) current_app_state = MENU_STATE_PAGE_1;

	if(uart_flag == 1)
	{
		uart_flag = 0;
		if(find_tag(RFID_buffer + 7))
		{
			lcd_clear(i2c_handle);
			lcd_send_text("User already", i2c_handle);
			lcd_send_cmd(0xC0, i2c_handle);
			lcd_send_text("exists", i2c_handle);
			HAL_Delay(USER_ALREADY_EXISTS_SCREEN_TIME_MS);
			current_app_state = MENU_STATE_PAGE_1;
			return;
		}
		flash_add_new_tag(RFID_buffer + 7);
		flash_update_memory_content();
		flash_download_tag_data();
		lcd_clear(i2c_handle);
		lcd_send_text("New user added", i2c_handle);
		HAL_Delay(NEW_USER_ADDED_SCREEN_TIME_MS);
		current_app_state = MENU_STATE_PAGE_1;
	}
}

void app_delete_user_state(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != DELETE_USER_STATE)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Delete user:", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("Place card to read", i2c_handle);
		prev_app_state = DELETE_USER_STATE;

		TIM2->ARR = MAX_DELETE_USER_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
		uart_flag = 0;
	}

	if(tim_flag == 1) current_app_state = MENU_STATE_PAGE_2;

	if(uart_flag == 1)
	{
		uart_flag = 0;
		if(!flash_remove_tag(RFID_buffer + 7))
		{
			lcd_clear(i2c_handle);
			lcd_send_text("User does not", i2c_handle);
			lcd_send_cmd(0xC0, i2c_handle);
			lcd_send_text("exists", i2c_handle);
			HAL_Delay(USER_DOESNT_EXISTS_SCREEN_TIME_MS);
			current_app_state = MENU_STATE_PAGE_2;
			return;
		}
		flash_update_memory_content();
		flash_download_tag_data();
		lcd_clear(i2c_handle);
		lcd_send_text("User removed", i2c_handle);
		HAL_Delay(USER_REMOVED_SCREEN_TIME_MS);
		current_app_state = MENU_STATE_PAGE_2;
	}
}

void app_change_password_state(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != CHANGE_PASSWORD_STATE)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("New password:", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		password_buffer_index = 0;
		prev_app_state = CHANGE_PASSWORD_STATE;

		TIM2->ARR = MAX_CHANGE_PASSWORD_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;

		memcpy(temp_password_buffor, flash_correct_password_buffor, 4);
	}

	if(tim_flag == 1){
		current_app_state = DEFAULT_STATE;
		memcpy(flash_correct_password_buffor, temp_password_buffor, 4);
	}

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '#' || pressed_key_char == '*') return;
		lcd_send_data('*', i2c_handle);
		flash_correct_password_buffor[password_buffer_index++] = pressed_key_char;
		if(password_buffer_index > 3)
		{
			flash_update_memory_content();
			flash_download_tag_data();
			lcd_clear(i2c_handle);
			lcd_send_text("Password changed", i2c_handle);
			HAL_Delay(PASSWORD_CHANGED_SCREEN_TIMR_MS);
			current_app_state = MENU_STATE_PAGE_3;
		}
	}

}

void app_clear_database_state(I2C_HandleTypeDef *i2c_handle){
	if(prev_app_state != CLEAR_DATABASE_STATE)
	{
		lcd_clear(i2c_handle);
		lcd_send_text("Are you sure?", i2c_handle);
		lcd_send_cmd(0xC0, i2c_handle);
		lcd_send_text("0 - yes", i2c_handle);
		password_buffer_index = 0;
		prev_app_state = CLEAR_DATABASE_STATE;

		TIM2->ARR = MAX_CLEAR_DATABASE_IDLE_TIME;
		__HAL_TIM_SET_COUNTER(&tim2, 1);
		TIM2->CR1 = 1;
		tim_flag = 0;
	}

	if(tim_flag == 1) current_app_state = MENU_STATE_PAGE_4;

	int8_t pressed_key_index = keypad_scan_pressed_index_filtered();
	if(pressed_key_index != NO_BUTTON_PRESSED)
	{
		char pressed_key_char = keypad_index_to_char(pressed_key_index);
		if(pressed_key_char == '0'){
			flash_clear_database_wo_password();
			flash_download_tag_data();
			lcd_clear(i2c_handle);
			lcd_send_text("Database is", i2c_handle);
			lcd_send_cmd(0xC0, i2c_handle);
			lcd_send_text("empty", i2c_handle);
			HAL_Delay(USER_ALREADY_EXISTS_SCREEN_TIME_MS);
			current_app_state = MENU_STATE_PAGE_4;
			return;
		}
		else{
			current_app_state = MENU_STATE_PAGE_4;
		}
	}
}

uint8_t app_is_password_correct(){
	for(int i = 0; i < 4; i++){
		if(password_buffer[i] != flash_correct_password_buffor[i]) return 0;
	}
	return 1;
}

void app_main_loop(I2C_HandleTypeDef *i2c_handle){
	switch(current_app_state)
	{
	case DEFAULT_STATE:
		app_default_state(i2c_handle);
		break;
	case WRITE_PASSWORD_STATE:
		app_write_password_state(i2c_handle);
		break;
	case MENU_STATE_PAGE_1:
		app_menu_state_page_1(i2c_handle);
		break;
	case MENU_STATE_PAGE_2:
		app_menu_state_page_2(i2c_handle);
		break;
	case MENU_STATE_PAGE_3:
		app_menu_state_page_3(i2c_handle);
		break;
	case MENU_STATE_PAGE_4:
		app_menu_state_page_4(i2c_handle);
		break;
	case CHANGE_PASSWORD_STATE:
		app_change_password_state(i2c_handle);
		break;
	case ADD_USER_STATE:
		app_add_user_state(i2c_handle);
		break;
	case DELETE_USER_STATE:
		app_delete_user_state(i2c_handle);
		break;
	case CLEAR_DATABASE_STATE:
		app_clear_database_state(i2c_handle);
		break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	TIM2->CR1 = 0;
	tim_flag = 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  uart_flag = 1;
  HAL_UART_Receive_IT(huart, RFID_buffer, RFID_FRAME_SIZE);
}












