/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "ov7670.h"
#include "sl_iostream.h"
#include "sl_iostream_handles.h"


OV7670_Handler_t i2c_cfg;

uint32_t cc;



static void UART_SendRawFrame(void)
{
  const uint8_t header[] = {
      'O', 'V', '7', '6',
      0x9C, 0x00,   // width  = 156
      0x78, 0x00,   // height = 120
      0x02          // YUV422 = 2 bytes/pixel
  };

  sl_iostream_write(sl_iostream_vcom_handle, header, sizeof(header));

  for (uint32_t i = 0; i < TOTAL_FRAME; i += 256)
  {
    uint32_t remain = TOTAL_FRAME - i;
    uint32_t chunk = remain > 256 ? 256 : remain;

    sl_iostream_write(sl_iostream_vcom_handle, &image_buf[i], chunk);
  }
}

void app_init(void)
{
  i2c_cfg.i2c_handler = sl_i2cspm_hehe;

  OV7670_Init(&i2c_cfg);

  for (int i = 0; i < 15; i++)
  {
    OV7670_ReadData_OnPCLK(); /* đọc nhưng bỏ qua */
  }
  // Capture 1 frame
  OV7670_ReadData_OnPCLK();

  // Gửi 1 ảnh qua UART
  UART_SendRawFrame();

  printf("Frame sent\r\n");
}

void app_process_action(void)
{
  // Không làm gì nữa để chỉ gửi 1 ảnh
}
