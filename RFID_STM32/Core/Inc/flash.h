#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32l4xx_hal.h"

extern SPI_HandleTypeDef spi1;
#define MAIN_SECTION_ADDRES 0x01
#define TAG_PASSWORD_ADDRES 0x0001
#define TAG_COUNT_ADDRES 0x0010
#define TAG_FIRST_ADDRES 0x0020

void flash_init();
void flash_wren();
void flash_write_byte(uint8_t data_t, uint16_t addres);
uint8_t flash_read_byte(uint16_t addres);
uint8_t flash_read_status_register();
void flash_write_byte_array(uint8_t *data_t, uint8_t size, uint16_t addres);
void flash_read_byte_array(uint8_t *dest, uint8_t size, uint16_t addres);

extern uint8_t flash_tag_count;
extern uint8_t flash_tag_buffor[256][6];
extern char flash_correct_password_buffor[4];

void flash_download_tag_data();
uint8_t find_tag(uint8_t *tag);
uint8_t flash_compare_tags(uint8_t *tag1, uint8_t *tag2, uint8_t size);

void flash_add_new_tag(uint8_t *tag);
uint8_t flash_remove_tag(uint8_t *tag);
void flash_update_memory_content();
void flash_erase_block(uint8_t *addres);
void flash_clear_database_wo_password();

uint8_t is_tag_removed(uint8_t *tag);

#endif /* INC_FLASH_H_ */
