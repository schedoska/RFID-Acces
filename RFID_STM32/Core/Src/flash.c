#include "flash.h"
#include <string.h>

uint8_t flash_tag_buffor[256][6];
uint8_t flash_tag_count = 0;
uint8_t flash_tag_proxy_count = 0;
char flash_correct_password_buffor[4];

void flash_init()
{
	uint8_t data;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	data = 0x50;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	data = 0x01;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = 0x00;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(50);
}

void flash_wren()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	uint8_t data = 0x06;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void flash_write_byte(uint8_t data_t, uint16_t addres)
{
	flash_wren();

	uint8_t data;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	data = 0x02;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = MAIN_SECTION_ADDRES;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = (addres & 0xff00) >> 8;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = addres & 0x00ff;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	HAL_SPI_Transmit(&spi1, &data_t, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	while(flash_read_status_register() & 1) HAL_Delay(1);
}

uint8_t flash_read_byte(uint16_t addres)
{
	uint8_t data;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	data = 0x03;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = MAIN_SECTION_ADDRES;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = (addres & 0xff00) >> 8;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = addres & 0x00ff;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);

	HAL_SPI_Receive(&spi1, &data, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	return data;
}

uint8_t flash_read_status_register()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	uint8_t data = 0x05;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	HAL_SPI_Receive(&spi1, &data, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	return data;
}

void flash_write_byte_array(uint8_t *data_t, uint8_t size, uint16_t addres)
{
	for(int i = 0; i < size; i++)
	{
		flash_write_byte(data_t[i], addres++);
	}
}

void flash_read_byte_array(uint8_t *dest, uint8_t size, uint16_t addres)
{
	uint8_t data;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	data = 0x03;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = MAIN_SECTION_ADDRES;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = (addres & 0xff00) >> 8;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = addres & 0x00ff;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);

	for(int i = 0; i < size; i++)
	{
		HAL_SPI_Receive(&spi1, &(dest[i]), 1, 500);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void flash_download_tag_data()
{
	flash_correct_password_buffor[0] = flash_read_byte(TAG_PASSWORD_ADDRES);
	flash_correct_password_buffor[1] = flash_read_byte(TAG_PASSWORD_ADDRES + 1);
	flash_correct_password_buffor[2] = flash_read_byte(TAG_PASSWORD_ADDRES + 2);
	flash_correct_password_buffor[3] = flash_read_byte(TAG_PASSWORD_ADDRES + 3);

	flash_tag_count = flash_read_byte(TAG_COUNT_ADDRES);
	flash_tag_proxy_count = flash_tag_count;
	uint16_t tag_addres = TAG_FIRST_ADDRES;
	for(int i = 0; i < flash_tag_count; i++)
	{
		flash_read_byte_array(flash_tag_buffor[i], 6, tag_addres);
		tag_addres += 6;
	}
}

uint8_t find_tag(uint8_t *tag)
{
	for(int i = 0; i < flash_tag_count; i++)
	{
		if(flash_compare_tags(flash_tag_buffor[i], tag, 6)) return 1;
	}
	return 0;
}

uint8_t flash_compare_tags(uint8_t *tag1, uint8_t *tag2, uint8_t size)
{
	for(int i = 0; i < size; i++)
	{
		if(tag1[i] != tag2[i]) return 0;
	}
	return 1;
}

void flash_add_new_tag(uint8_t *tag)
{
	strcpy(flash_tag_buffor[flash_tag_count], tag);
	++flash_tag_count;
	flash_tag_proxy_count = flash_tag_count;
}

uint8_t flash_remove_tag(uint8_t *tag)
{
	for(int i = 0; i < flash_tag_count; i++)
	{
		if(flash_compare_tags(tag, flash_tag_buffor[i], 6))
		{
			for(int q = 0; q < 6; q++) flash_tag_buffor[i][q] = 0;
			--flash_tag_count;
			return 1;
		}
	}
	return 0;
}

void flash_update_memory_content()
{
	flash_erase_block(MAIN_SECTION_ADDRES);

	flash_write_byte(flash_correct_password_buffor[0], TAG_PASSWORD_ADDRES);
	flash_write_byte(flash_correct_password_buffor[1], TAG_PASSWORD_ADDRES + 1);
	flash_write_byte(flash_correct_password_buffor[2], TAG_PASSWORD_ADDRES + 2);
	flash_write_byte(flash_correct_password_buffor[3], TAG_PASSWORD_ADDRES + 3);

	flash_write_byte(flash_tag_count, TAG_COUNT_ADDRES);
	uint16_t tag_addres = TAG_FIRST_ADDRES;
	for(int i = 0; i < flash_tag_proxy_count; i++)
	{
		if(is_tag_removed(flash_tag_buffor[i])) continue;
		flash_write_byte_array(flash_tag_buffor[i], 6, tag_addres);
		tag_addres += 6;
	}
}

void flash_erase_block(uint8_t *addres)
{
	flash_wren();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	uint8_t data = 0x20;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = addres;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = 0x00;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	data = 0x00;
	HAL_SPI_Transmit(&spi1, &data, 1, 500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	while(flash_read_status_register() & 1) HAL_Delay(1);
}

uint8_t is_tag_removed(uint8_t *tag)
{
	for(int i = 0; i < 6; i++)
	{
		if(tag[i] != 0) return 0;
	}
	return 1;
}

void flash_clear_database_wo_password()
{
	flash_erase_block(MAIN_SECTION_ADDRES);
	flash_write_byte(flash_correct_password_buffor[0], TAG_PASSWORD_ADDRES);
	flash_write_byte(flash_correct_password_buffor[1], TAG_PASSWORD_ADDRES + 1);
	flash_write_byte(flash_correct_password_buffor[2], TAG_PASSWORD_ADDRES + 2);
	flash_write_byte(flash_correct_password_buffor[3], TAG_PASSWORD_ADDRES + 3);
}




