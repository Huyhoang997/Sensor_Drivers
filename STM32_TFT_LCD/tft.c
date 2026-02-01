/*
 * tft.c
 *
 *  Created on: Jan 9, 2026
 *      Author: ACER
 */
#include "tft.h"

/* ====================== CHIP_SELECT ====================== */
static inline void CS_LOW(void) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

void CS_HIGH(void) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

/* ====================== COMMAND_PIN ====================== */
static inline void CMD_MODE(void) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}

static inline void DATA_MODE(void) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}

/* ====================== HARDWARE_RESET ====================== */
static inline void HW_ResetOn(void) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

static inline void HW_ResetOff(void) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

static TFT_Status_Typedef TFT_SendCmd(uint8_t cmd) {

	HAL_StatusTypeDef status;
	CMD_MODE();
	status = HAL_SPI_Transmit(&hspi1, &cmd, 1, 1000);
	if(status != HAL_OK) {
		return TFT_SPI_ERR;
	}
	return TFT_OK;
}


static TFT_Status_Typedef TFT_SendData(uint8_t data) {

	HAL_StatusTypeDef status;
	DATA_MODE();
	status = HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
	if(status != HAL_OK) {
		return TFT_SPI_ERR;
	}
	return TFT_OK;
}


TFT_Status_Typedef TFT_SetPos(uint8_t row_start, uint8_t row_end, uint8_t col_start, uint8_t col_end) {
	TFT_Status_Typedef status;
	CS_LOW();

	status = TFT_SendCmd(ROW_ADDR_SET);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(0x00);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(row_start);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(0x00);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(row_end);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}


	status = TFT_SendCmd(COL_ADDR_SET);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}
	status = TFT_SendData(0x00);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}
	status = TFT_SendData(col_start);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(0x00);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(col_end);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(WRITE_DATA);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	//CS_HIGH();
	return TFT_OK;
}


TFT_Status_Typedef TFT_DrawImage(const uint16_t *data, uint16_t size_of_image) {
	if(data == NULL) {
		return TFT_ERR;
	}

	HAL_StatusTypeDef status;

	DATA_MODE();

	status = HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)data, size_of_image);

	if(status != HAL_OK) {
		return TFT_SPI_ERR;
	}

	return TFT_OK;
}


//__weak void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
//{
///    if (hspi->Instance == SPI1)
//    {
 //       CS_HIGH();
//
 //   }
//}

TFT_Status_Typedef TFT_Init(void) {

	TFT_Status_Typedef status;
	HW_ResetOn();
	HAL_Delay(50);
	HW_ResetOff();
	HAL_Delay(200);

	CS_LOW();
	status = TFT_SendCmd(SW_RESET);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}
	HAL_Delay(200);

	status = TFT_SendCmd(SLEEP_OUT);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	HAL_Delay(120);

	status = TFT_SendCmd(PARTIAL_OFF);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(INVERSION_OFF);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(GAMMA_SET);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(GAMMA_CURVE_4);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(TEARING_OFF);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}


	status = TFT_SendCmd(IDLE_OFF);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}


	status = TFT_SendCmd(FRAME_CTRL_1);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}


	status = TFT_SendData(FRAME_MODE_1);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(FRAME_MODE_2);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(FRAME_MODE_3);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(VCOM_CTRL);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(VCOM_PARA_1);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}


	status = TFT_SendCmd(POWER_CTRL_NORMAL);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(POWER_CTRL_PARA_1);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(POWER_CTRL_PARA_2);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(MEMORY_DATA_ACCESS);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(VERTICAL_FLIP);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}


	status = TFT_SendCmd(PIXEL_FORMAT);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendData(PIXEL_16BIT);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	status = TFT_SendCmd(DISPLAY_ON);
	if(status != TFT_OK) {
		return TFT_SPI_ERR;
	}

	HAL_Delay(200);
	CS_HIGH();

	return TFT_OK;
}
