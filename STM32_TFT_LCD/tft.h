 /* tft.h
 *
 *  Created on: Jan 9, 2026
 *      Author: ACER
 */
#ifndef TFT_H_
#define TFT_H_

#include "main.h"

#define SW_RESET						0x01U
#define SLEEP_IN						0x10U
#define SLEEP_OUT						0x11U

#define PARTIAL_ON						0x12U
#define PARTIAL_OFF						0x13U
#define PARTIAL_ADDR_SET				0x30U


#define INVERSION_ON					0x21U
#define INVERSION_OFF					0x20U

#define DISPLAY_ON						0x29U
#define DISPLAY_OFF						0x28U

#define GAMMA_SET						0x26U
 #define GAMMA_CURVE_1					0x01U
 #define GAMMA_CURVE_2					0x02U
 #define GAMMA_CURVE_3					0x04U
 #define GAMMA_CURVE_4					0x08U

#define COL_ADDR_SET					0x2AU
#define ROW_ADDR_SET					0x2BU

#define WRITE_DATA						0x2CU

#define TEARING_OFF						0x34U
#define TEARING_ON						0x35U

#define MEMORY_DATA_ACCESS				0x36U
 #define VERTICAL_FLIP					0xC8U
 #define HORIZONTAL_FLIP				0x08U

#define IDLE_ON							0x39U
#define IDLE_OFF						0x38U

#define PIXEL_FORMAT					0x3AU
 #define PIXEL_16BIT					0x05U
 #define PIXEL_8BIT						0x03U
 #define PIXEL_18BIT					0x06U

#define FRAME_CTRL_1					0xB1U
 #define FRAME_MODE_1					0x01U
 #define FRAME_MODE_2					0x2CU
 #define FRAME_MODE_3					0x2DU

#define VCOM_CTRL						0xC5U
 #define VCOM_PARA_1					0x0EU
 #define VCOM_PARA_2					0x3EU

#define POWER_CTRL_NORMAL				0xC3U
 #define POWER_CTRL_PARA_1				0x8AU
 #define POWER_CTRL_PARA_2				0x2AU
 #
#define TFT_WIDTH						128
#define TFT_HEIGHT						128

typedef enum {
	TFT_OK,
	TFT_ERR,
	TFT_SPI_ERR,
	TFT_TIMEOUT
} TFT_Status_Typedef;

TFT_Status_Typedef TFT_Init(void);
TFT_Status_Typedef TFT_SetPos(uint8_t row_start, uint8_t row_end, uint8_t col_start, uint8_t col_end);
TFT_Status_Typedef TFT_DrawImage(const uint16_t *data, uint16_t size_of_image);
void CS_HIGH(void);

#endif /* TFT_H_ */
