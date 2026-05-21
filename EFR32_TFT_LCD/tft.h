/*
 * tft.h
 *
 *  Created on: Apr 26, 2026
 *      Author: ACER
 */

#ifndef TFT_H_
#define TFT_H_

#include <sl_sleeptimer.h>
#include <sl_spidrv_instances.h>
#include "em_gpio.h"
#include "em_ldma.h"

typedef enum
{
    TFT_OK = 0U,
    TFT_ERR,
    TFT_SPI_ERR,
    TFT_TIMEOUT
} TFT_Status_Typedef;

typedef enum
{
  PIN_LOW = 0U,
  PIN_HIGH
} TFT_Mode_PinState;

typedef struct
{
  GPIO_Port_TypeDef port;
  uint8_t pin;
} TFT_CS_Config_t;

typedef struct
{
  GPIO_Port_TypeDef port;
  uint8_t pin;
} TFT_RST_Config_t;

typedef struct
{
  GPIO_Port_TypeDef port;
  uint8_t pin;
} TFT_DC_Config_t;

typedef struct
{
  TFT_CS_Config_t cs_cfg;
  TFT_RST_Config_t rst_cfg;
  TFT_DC_Config_t dc_cfg;
  SPIDRV_Handle_t handle;

} TFT_Handler_t;

#define NULL        ((void *)0)
#define DC_CMD_MODE         0U
#define DC_DATA_MODE        1U

#define CHUNK_SIZE  2048
#define FULL_DISPLAY     (TFT_WIDTH *  TFT_HEIGHT)
#define TOTAL_FRAME      (FULL_DISPLAY * 2)

#define SW_RESET              0x01U
  #define SLEEP_IN            0x10U
  #define SLEEP_OUT           0x11U

#define PARTIAL_ON            0x12U
#define PARTIAL_OFF           0x13U
#define PARTIAL_ADDR_SET      0x30U


#define INVERSION_ON          0x21U
#define INVERSION_OFF         0x20U

#define DISPLAY_ON            0x29U
#define DISPLAY_OFF           0x28U

#define GAMMA_SET             0x26U
#define GAMMA_CURVE_1         0x01U
#define GAMMA_CURVE_2         0x02U
#define GAMMA_CURVE_3         0x04U
#define GAMMA_CURVE_4         0x08U

#define COL_ADDR_SET          0x2AU
#define ROW_ADDR_SET          0x2BU

#define WRITE_DATA            0x2CU

#define TEARING_OFF           0x34U
#define TEARING_ON            0x35U

#define MEMORY_DATA_ACCESS    0x36U
  #define MADCTL_MY           0x48U  // Vertical Flip (Lật dọc)
  #define MADCTL_MX           0x40U  // Horizontal Flip (Lật ngang)
  #define MADCTL_MV           0x20U  // Row/Column Exchange (Xoay 90 độ)
  #define MADCTL_BGR          0x08U  // BGR Color Mode

#define IDLE_ON               0x39U
#define IDLE_OFF              0x38U

#define PIXEL_FORMAT          0x3AU
 #define PIXEL_16BIT          0x05U
 #define PIXEL_8BIT           0x03U
 #define PIXEL_18BIT          0x06U

#define FRAME_CTRL_1          0xB1U
 #define FRAME_MODE_1         0x01U
 #define FRAME_MODE_2         0x2CU
 #define FRAME_MODE_3         0x2DU

#define VCOM_CTRL             0xC5U
 #define VCOM_PARA_1          0x0EU
 #define VCOM_PARA_2          0x3EU

#define POWER_CTRL_NORMAL     0xC3U
 #define POWER_CTRL_PARA_1    0x8AU
 #define POWER_CTRL_PARA_2    0x2AU

#define TFT_WIDTH             128
#define TFT_HEIGHT            160

#define TFT_ROW_START_ADDR    1
#define TFT_ROW_END_ADDR      160
#define TFT_COL_START_ADDR    2
#define TFT_CAL_END_ADDR      129


TFT_Status_Typedef TFT_SendCmd(TFT_Handler_t *Instance, uint8_t cmd, uint8_t length);
TFT_Status_Typedef TFT_FillColor(TFT_Handler_t *Instance, uint16_t color);
TFT_Status_Typedef TFT_Init(TFT_Handler_t *Instance);
TFT_Status_Typedef TFT_DeInit(TFT_Handler_t *Instance);
TFT_Status_Typedef TFT_ClearScreen(TFT_Handler_t *Instance);
TFT_Status_Typedef TFT_SetPos(TFT_Handler_t *Instance, uint8_t row_start, uint8_t row_end, uint8_t col_start, uint8_t col_end);
TFT_Status_Typedef TFT_DrawImage(TFT_Handler_t *Instance,const uint16_t *image, uint32_t length);


#endif /* TFT_H_ */
