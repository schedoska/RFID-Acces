#ifndef INC_APP_H_
#define INC_APP_H_

#include "lcd.h"
#include "keypad.h"
#include "flash.h"

typedef enum {DEFAULT_STATE,
			  WRITE_PASSWORD_STATE,
			  WRONG_PASSWORD_STATE,
			  CORRECT_PASSWORD_STATE,
			  MENU_STATE_PAGE_1,
			  MENU_STATE_PAGE_2,
			  MENU_STATE_PAGE_3,
			  MENU_STATE_PAGE_4,
			  ADD_USER_STATE,
			  DELETE_USER_STATE,
			  CHANGE_PASSWORD_STATE,
			  CLEAR_DATABASE_STATE}
APP_STATE;

extern APP_STATE current_app_state;
extern APP_STATE prev_app_state;

extern char password_buffer[4];
extern uint8_t password_buffer_index;
extern char correct_password[4];

extern uint8_t menu_page_index;

#define RFID_FRAME_SIZE 14
extern uint8_t RFID_buffer[RFID_FRAME_SIZE];
extern UART_HandleTypeDef uart1;

extern TIM_HandleTypeDef tim2;
#define MAX_PASSWORD_IDLE_TIME 50000
#define MAX_MENU_PAGE_IDLE_TIME 50000
#define MAX_CHANGE_PASSWORD_IDLE_TIME 70000
#define MAX_ADD_USER_IDLE_TIME 70000
#define MAX_DELETE_USER_IDLE_TIME 70000
#define MAX_CLEAR_DATABASE_IDLE_TIME 50000

#define WRONG_PASSWORD_SCREEN_TIME_MS 1500
#define PASSWORD_CHANGED_SCREEN_TIMR_MS 1500
#define DOORS_OPEN_TIME_MS 4000
#define ACCESS_DENIED_SCREEN_TIME_MS 1500
#define USER_ALREADY_EXISTS_SCREEN_TIME_MS 1500
#define NEW_USER_ADDED_SCREEN_TIME_MS 2000
#define USER_DOESNT_EXISTS_SCREEN_TIME_MS 2000
#define USER_REMOVED_SCREEN_TIME_MS 2000

void app_default_state(I2C_HandleTypeDef *i2c_handle);
void app_write_password_state(I2C_HandleTypeDef *i2c_handle);

void app_menu_state_page_1(I2C_HandleTypeDef *i2c_handle);
void app_menu_state_page_2(I2C_HandleTypeDef *i2c_handle);
void app_menu_state_page_3(I2C_HandleTypeDef *i2c_handle);
void app_menu_state_page_4(I2C_HandleTypeDef *i2c_handle);

void app_add_user_state(I2C_HandleTypeDef *i2c_handle);
void app_delete_user_state(I2C_HandleTypeDef *i2c_handle);
void app_change_password_state(I2C_HandleTypeDef *i2c_handle);
void app_clear_database_state(I2C_HandleTypeDef *i2c_handle);

uint8_t app_is_password_correct();

void app_main_loop(I2C_HandleTypeDef *i2c_handle);

#endif /* INC_APP_H_ */
