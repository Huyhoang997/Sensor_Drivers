/*
 * oled.c
 *
 *  Created on: Dec 11, 2025
 *      Author: ACER
 */
#include <stdio.h>
#include <sl_i2cspm_instances.h>
#include <em_cmu.h>
#include <sl_sleeptimer.h>
#include <oled.h>

static void OLED_SendCmd(uint8_t cmd) {
  uint8_t buff_cmd[2] = {OLED_CMD_MODE, cmd};
  I2C_TransferSeq_TypeDef i2c_transfer_seg;
  I2C_TransferReturn_TypeDef status;
  i2c_transfer_seg.addr = OLED_ADDRESS;
  i2c_transfer_seg.flags = I2C_FLAG_WRITE;
  i2c_transfer_seg.buf[0].data = buff_cmd;
  i2c_transfer_seg.buf[0].len = 2;

  status = I2CSPM_Transfer(I2C0, &i2c_transfer_seg);
  if(status != i2cTransferDone) {
      printf("I2C_ERR\n");
  }
}

static void OLED_SendData(uint8_t data) {
  uint8_t buff_data[2] = {OLED_DATA_MODE, data};
  I2C_TransferReturn_TypeDef status;
  I2C_TransferSeq_TypeDef i2c_transfer_seg;
  i2c_transfer_seg.addr = OLED_ADDRESS;
  i2c_transfer_seg.flags = I2C_FLAG_WRITE;
  i2c_transfer_seg.buf[0].data = buff_data;
  i2c_transfer_seg.buf[0].len = 2;

  status = I2CSPM_Transfer(I2C0, &i2c_transfer_seg);
  if(status != i2cTransferDone) {
      printf("I2C_ERR\n");
  }
}

void OLED_SetPos(uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end) {
  OLED_SendCmd(SET_COL_ADDR);
  OLED_SendCmd(col_start);
  OLED_SendCmd(col_end);

  OLED_SendCmd(SET_PAGE_ADDR);
  OLED_SendCmd(page_start);
  OLED_SendCmd(page_end);
}

void OLED_DrawImage(const uint8_t *ptr) {
  for(int i = 0; i < OLED_WIDTH * OLED_PAGE; i++) {
      OLED_SendData(*(ptr + i));
  }
}


void OLED_Init(void) {
  sl_sleeptimer_delay_millisecond(200);
  OLED_SendCmd(DISPLAY_OFF);

  OLED_SendCmd(SET_DISPLAY_CLOCK);
  OLED_SendCmd(RATIO_OSC_MAX);

  OLED_SendCmd(SET_PRE_CHARGE);
  OLED_SendCmd(PRE_CHARGE_DEFAULT);

  OLED_SendCmd(SET_VCOM_DESELECT);
  OLED_SendCmd(VCOM_DESELECT_DEFAULT);

  OLED_SendCmd(SET_MEMORY_ADDR_MODE);
  OLED_SendCmd(SET_HORIZONTAL_MODE);

  OLED_SendCmd(SET_DISPLAY_START_LINE);
  OLED_SendCmd(SET_SEG_REMAP_NORMAL);

  OLED_SendCmd(SET_MULTI_RATIO);
  OLED_SendCmd(MULTI_RATIO_DEFAULT);

  OLED_SendCmd(SET_COM_SCAN_DIRECTION);
  OLED_SendCmd(SET_HORIZONTAL_FLIP);
  OLED_SendCmd(SET_COM_PIN_CONFIG);
  OLED_SendCmd(SEQUENTIAL_COM_PIN);

  OLED_SendCmd(SET_CONTRAST_CONTROL);
  OLED_SendCmd(CONTRAST_MAX);

  OLED_SendCmd(SET_HORIZONTAL_FLIP);

  OLED_SendCmd(DISPLAY_SET_NORMAL);
  OLED_SendCmd(ENTIRE_DISPLAY_OFF);
  //OLED_SendCmd(DISPLAY_SET_INVERSE);
  OLED_SendCmd(DISPLAY_ON);
}
