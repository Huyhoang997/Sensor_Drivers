/*
 * oled.h
 *
 *  Created on: Dec 11, 2025
 *      Author: ACER
 */

#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

/* ================ OLED_ADDR ============== */
#define OLED_ADDRESS           (0x3C << 1U)
#define OLED_CMD_MODE           0x00
#define OLED_DATA_MODE          0x40
#define OLED_HEIGHT             64
#define OLED_WIDTH              128
#define OLED_PAGE               8

/* ================ OLED_CMD =============== */
#define DISPLAY_OFF             0xAE
#define DISPLAY_ON              0xAF
#define ENTIRE_DISPLAY_ON       0xA5
#define ENTIRE_DISPLAY_OFF      0xA4
#define DISPLAY_SET_NORMAL      0xA6
#define DISPLAY_SET_INVERSE     0xA7
#define SET_CONTRAST_CONTROL    0x81
  #define CONTRAST_DEFAULT      0x7F
  #define CONTRAST_MAX          0xFF
  #define CONTRAST_HALF         0x1F
  #define CONSTRAST_MIN         0x00

/* =========== SET_MEMORY_ADDR_MODE =========== */
#define SET_MEMORY_ADDR_MODE    0x20
  #define SET_HORIZONTAL_MODE   0x00
  #define SET_VERTICAL_MODE     0x01
  #define SET_PAGE_MODE         0x02
#define SET_COL_ADDR            0x21
#define SET_PAGE_ADDR           0x22

/* ============== HARDWARE_CONFIG ============== */
#define SET_DISPLAY_START_LINE  0x40
#define SET_SEG_REMAP_NORMAL    0xA0
  #define SET_VERTICAL_FLIP     0xA1
#define SET_MULTI_RATIO         0xA8
  #define MULTI_RATIO_DEFAULT   0x3F
#define SET_COM_SCAN_DIRECTION  0xC0
  #define SET_HORIZONTAL_FLIP   0xC8
#define SET_COM_PIN_CONFIG      0xDA
  #define SEQUENTIAL_COM_PIN    0x12

/* =============== TIMING & DRIVING ============= */
#define SET_DISPLAY_CLOCK       0xD5
  #define RATIO_OSC_DEFAULT     0x80
  #define RATIO_OSC_MAX         0xF0
#define SET_PRE_CHARGE          0xD9
  #define PRE_CHARGE_DEFAULT    0x22
  #define PRE_CHARGE_FAST       0xF0
#define SET_VCOM_DESELECT       0xDB
  #define VCOM_DESELECT_DEFAULT 0x20
#define COMMAND_FOR_NOP         0xE3

void OLED_Init(void);
void OLED_SetPos(uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end);
void OLED_DrawImage(const uint8_t *ptr);

#endif /* OLED_H_ */
