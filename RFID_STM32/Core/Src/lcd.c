#include "lcd.h"

void lcd_send_cmd (char cmd, I2C_HandleTypeDef *i2c_handle){
	 char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (i2c_handle, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data, I2C_HandleTypeDef *i2c_handle){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (i2c_handle, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init(I2C_HandleTypeDef *i2c_handle){
	/*HAL_Delay(50);
	lcd_send_cmd (0x30, i2c_handle);
	HAL_Delay(5);
	lcd_send_cmd (0x30, i2c_handle);
	HAL_Delay(3);
	lcd_send_cmd (0x30, i2c_handle);
	HAL_Delay(12);
	lcd_send_cmd (0x20, i2c_handle);
	HAL_Delay(12);

	lcd_send_cmd (0x28, i2c_handle);
	HAL_Delay(3);
	lcd_send_cmd (0x08, i2c_handle);
	HAL_Delay(3);
	lcd_send_cmd (0x01, i2c_handle);
	HAL_Delay(3);
	HAL_Delay(3);
	lcd_send_cmd (0x06, i2c_handle);
	HAL_Delay(3);
	lcd_send_cmd (0x0C, i2c_handle);*/

	HAL_Delay(50);
	lcd_send_cmd(0x02, i2c_handle);
	HAL_Delay(2);
	lcd_send_cmd(0x28, i2c_handle);
	lcd_send_cmd(0x0C, i2c_handle);
	lcd_send_cmd(0x06, i2c_handle);
	HAL_Delay(2);
	lcd_send_cmd(0x01, i2c_handle);
	lcd_send_cmd(0x80, i2c_handle);
	HAL_Delay(5);
}

void lcd_clear(I2C_HandleTypeDef *i2c_handle){
	lcd_send_cmd(0x01, i2c_handle);
	HAL_Delay(2);
}

void lcd_send_text(char *text, I2C_HandleTypeDef *i2c_handle){
	while(*text){
		lcd_send_data(*text, i2c_handle);
		text++;
	}
}

void lcd_send_text_lenght(char *text, uint8_t lenght, I2C_HandleTypeDef *i2c_handle){
	for(int i = 0; i < lenght; i++){
		lcd_send_data(text[i], i2c_handle);
	}
}






