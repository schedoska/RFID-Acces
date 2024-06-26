#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32l4xx_hal.h"

#define SLAVE_ADDRESS_LCD 0x4E

void lcd_send_cmd (char cmd, I2C_HandleTypeDef *i2c_handle);
void lcd_send_data (char data, I2C_HandleTypeDef *i2c_handle);
void lcd_init(I2C_HandleTypeDef *i2c_handle);
void lcd_clear(I2C_HandleTypeDef *i2c_handle);
void lcd_send_text(char *text, I2C_HandleTypeDef *i2c_handle);
void lcd_send_text_lenght(char *text, uint8_t lenght, I2C_HandleTypeDef *i2c_handle);

#endif /* INC_LCD_H_ */
