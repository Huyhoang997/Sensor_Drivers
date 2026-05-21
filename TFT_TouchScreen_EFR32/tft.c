/*
 * tft.c
 *
 *  Created on: Apr 26, 2026
 *      Author: ACER
 */
#include "tft.h"


static inline void TFT_CS_SetMode(TFT_Handler_t *Instance, TFT_Mode_PinState PinState)
{
  GPIO_PinModeSet(Instance->cs_cfg.port, Instance->cs_cfg.pin, gpioModePushPull, PinState);
}

static inline void TFT_DC_SetMode(TFT_Handler_t *Instance, uint8_t dc_mode)
{
  GPIO_PinModeSet(Instance->dc_cfg.port, Instance->dc_cfg.pin, gpioModePushPull, dc_mode);
}

static inline void TFT_RST_SetMode(TFT_Handler_t *Instance, TFT_Mode_PinState PinState)
{
  GPIO_PinModeSet(Instance->rst_cfg.port, Instance->rst_cfg.pin, gpioModePushPull, PinState);
}




TFT_Status_Typedef TFT_SendCmd(TFT_Handler_t *Instance, uint8_t cmd, uint8_t length)
{
  if(Instance == NULL)
  {
      return TFT_ERR;
  }
  Ecode_t status;
  TFT_CS_SetMode(Instance, PIN_LOW);

  TFT_DC_SetMode(Instance, DC_CMD_MODE);
  status = SPIDRV_MTransmitB(Instance->handle, &cmd, length);
  if(status != ECODE_OK)
  {
      return TFT_SPI_ERR;
  }

  TFT_CS_SetMode(Instance, PIN_HIGH);
  return TFT_OK;
}



TFT_Status_Typedef TFT_SendData(TFT_Handler_t *Instance, uint8_t data, uint8_t length)
{
  if(Instance == NULL)
  {
      return TFT_ERR;
  }
  Ecode_t status;
  TFT_CS_SetMode(Instance, PIN_LOW);

  TFT_DC_SetMode(Instance, DC_DATA_MODE);
  status = SPIDRV_MTransmitB(Instance->handle, &data, length);
  if(status != ECODE_OK)
  {
      return TFT_SPI_ERR;
  }

  TFT_CS_SetMode(Instance, PIN_HIGH);
  return TFT_OK;
}



TFT_Status_Typedef TFT_SetPos(TFT_Handler_t *Instance, uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end)
{
  TFT_Status_Typedef status;

  // 1. Set Row Address (Đưa khối này lên đầu giống code cũ của bạn)
  status = TFT_SendCmd(Instance, ROW_ADDR_SET, 1);
  if(status != TFT_OK) return TFT_SPI_ERR;
  TFT_SendData(Instance, (row_start >> 8) & 0xFF, 1);
  TFT_SendData(Instance, row_start & 0xFF, 1);
  TFT_SendData(Instance, (row_end >> 8) & 0xFF, 1);
  TFT_SendData(Instance, row_end & 0xFF, 1);

  // 2. Set Column Address
  status = TFT_SendCmd(Instance, COL_ADDR_SET, 1);
  if(status != TFT_OK) return TFT_SPI_ERR;
  TFT_SendData(Instance, (col_start >> 8) & 0xFF, 1);
  TFT_SendData(Instance, col_start & 0xFF, 1);
  TFT_SendData(Instance, (col_end >> 8) & 0xFF, 1);
  TFT_SendData(Instance, col_end & 0xFF, 1);

  // 3. Lệnh bắt đầu ghi dữ liệu
  status = TFT_SendCmd(Instance, WRITE_DATA, 1);

  return status;
}



TFT_Status_Typedef TFT_Init(TFT_Handler_t *Instance)
{
  if(Instance == NULL)
  {
      return TFT_ERR;
  }

  TFT_Status_Typedef checkStatus;
  /* TFT Hardware reset */
  TFT_RST_SetMode(Instance, PIN_LOW);

  sl_sleeptimer_delay_millisecond(50);

  TFT_RST_SetMode(Instance, PIN_HIGH);
  /* Wait for the TFT stable */
  sl_sleeptimer_delay_millisecond(200);

  /* TFT Init commands */
  checkStatus = TFT_SendCmd(Instance, SW_RESET, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }
  sl_sleeptimer_delay_millisecond(200);

  checkStatus = TFT_SendCmd(Instance, SLEEP_OUT, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }
  sl_sleeptimer_delay_millisecond(120);

  checkStatus = TFT_SendCmd(Instance, PARTIAL_OFF, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, INVERSION_OFF, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, GAMMA_SET, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, GAMMA_CURVE_4, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, TEARING_OFF, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, IDLE_OFF, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, FRAME_CTRL_1, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, FRAME_MODE_2, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }


  checkStatus = TFT_SendCmd(Instance, BLANKING_PORCH_CTRL, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, VFP, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, VBP, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, FIXED, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, HBP, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  //                                                                      xz          checkStatus = TFT_SendCmd(Instance, DISPLAY_FUNC_CTRL, 1);
  //checkStatus = TFT_SendData(Instance, 0x80, 1); // Parameter 1: RM=0 (System Interf), PTG=0, PT=0
  //checkStatus = TFT_SendData(Instance, 0x02, 1); // Parameter 2: GS=0, SS=0, SM=0, ISC=2
  //checkStatus = TFT_SendData(Instance, 0x3B, 1);

  checkStatus = TFT_SendCmd(Instance, VCOM_CTRL, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, VCOM_PARA_2, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, POWER_CTRL_NORMAL, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, POWER_CTRL_PARA_1, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, POWER_CTRL_PARA_2, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }


  checkStatus = TFT_SendCmd(Instance, MEMORY_DATA_ACCESS, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }


  checkStatus = TFT_SendData(Instance, MADCTL_MY, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, PIXEL_FORMAT, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendData(Instance, PIXEL_16BIT, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  checkStatus = TFT_SendCmd(Instance, DISPLAY_ON, 1);
  if(checkStatus != TFT_OK)
  {
      return TFT_ERR;
  }

  sl_sleeptimer_delay_millisecond(200);
  return TFT_OK;
}



TFT_Status_Typedef TFT_ClearScreen(TFT_Handler_t *Instance)
{
  if(Instance == NULL)
  {
      return TFT_ERR;
  }

  TFT_Status_Typedef status;

  status = TFT_SendCmd(Instance, SW_RESET, 1);
  if(status != TFT_OK) return TFT_ERR;

  sl_sleeptimer_delay_millisecond(120);

  status = TFT_Init(Instance);
  if(status != TFT_OK) return TFT_ERR;

  return TFT_OK;
}



TFT_Status_Typedef TFT_DeInit(TFT_Handler_t *Instance)
{
  if(Instance == NULL)
  {
      return TFT_ERR;
  }
  /* TFT Hardware reset */
  TFT_RST_SetMode(Instance, PIN_LOW);

  sl_sleeptimer_delay_millisecond(50);

  TFT_RST_SetMode(Instance, PIN_HIGH);
  /* Wait for the TFT stable */
  sl_sleeptimer_delay_millisecond(200);
  return TFT_OK;
}


#if 0
TFT_Status_Typedef TFT_FillColor(TFT_Handler_t *Instance, uint16_t color)
{
  uint8_t cc[TFT_WIDTH * TFT_HEIGHT] = {0};
  uint8_t buf[2] = {
      color >> 8,
      color & 0xFF
  };
  TFT_SendCmd(Instance, WRITE_DATA, 1);

  TFT_CS_SetMode(Instance, PIN_LOW);
  TFT_DC_SetMode(Instance, DC_DATA_MODE);
  for (int i = 0; i < (TFT_HEIGHT * TFT_HEIGHT); i++)
  {
      SPIDRV_MTransmitB(Instance->handle, buf, 2);
      //SPIDRV_MTransmitB(Instance->handle, &buf[1], 1);
  }
  TFT_CS_SetMode(Instance, PIN_HIGH);

  return TFT_OK;
}

#endif

#if 0
TFT_Status_Typedef TFT_FillColor(TFT_Handler_t *Instance, uint16_t color)
{
    if(Instance == NULL) return TFT_ERR;

    #define CHUNK_SIZE  2048
    static uint8_t cc[CHUNK_SIZE];

    for(int i = 0; i < CHUNK_SIZE; i += 2)
    {
        cc[i]     = color >> 8;
        cc[i + 1] = color & 0xFF;
    }

    TFT_SendCmd(Instance, WRITE_DATA, 1);
    TFT_CS_SetMode(Instance, PIN_LOW);
    TFT_DC_SetMode(Instance, DC_DATA_MODE);

    int total = TFT_HEIGHT * TFT_HEIGHT * 2;
    for(int sent = 0; sent < total; sent += CHUNK_SIZE)
    {
        int size = ((sent + CHUNK_SIZE) <= total) ? CHUNK_SIZE : (total - sent);
        SPIDRV_MTransmitB(Instance->handle, cc, size);
    }

    TFT_CS_SetMode(Instance, PIN_HIGH);
    return TFT_OK;
}
#endif

/* TFT enable DMA double buffering */
#if 1

#define CHUNK_SIZE_DMA 2048
#define TOTAL_CHUNKS   (TOTAL_FRAME / CHUNK_SIZE_DMA)

static uint8_t buff1[CHUNK_SIZE_DMA];
static uint8_t buff2[CHUNK_SIZE_DMA];

volatile uint8_t dma_busy = 0;

void spi_callback(struct SPIDRV_HandleData *handle, Ecode_t transferStatus, int itemsTransferred)
{
  (void)handle;
  (void)transferStatus;
  (void)itemsTransferred;
  dma_busy = 0;
}


TFT_Status_Typedef TFT_FillColor(TFT_Handler_t *Instance, uint16_t color)
{
    uint8_t *buffers[2] = {buff1, buff2};
    uint8_t current_buf = 0;

    TFT_SendCmd(Instance, WRITE_DATA, 1);
    TFT_CS_SetMode(Instance, PIN_LOW);
    TFT_DC_SetMode(Instance, DC_DATA_MODE);

    for(int i = 0; i < TOTAL_CHUNKS; i++)
    {

        while(dma_busy);


        for(int j = 0; j < CHUNK_SIZE_DMA; j += 2) {
            buffers[current_buf][j]     = color >> 8;
            buffers[current_buf][j + 1] = color & 0xFF;
        }
        dma_busy = 1;
        SPIDRV_MTransmit(Instance->handle, buffers[current_buf], CHUNK_SIZE_DMA, spi_callback);

        current_buf = 1 - current_buf;
    }
    while(dma_busy);
    TFT_CS_SetMode(Instance, PIN_HIGH);

    return TFT_OK;
}



#if 0
TFT_Status_Typedef TFT_DrawImage(TFT_Handler_t *Instance, const uint16_t *image, uint32_t length)
{
   uint8_t *buffers[2] = {buff1, buff2};
   uint8_t current_buf = 0;
   uint32_t image_pos = 0;

   TFT_CS_SetMode(Instance, PIN_LOW);
   TFT_DC_SetMode(Instance, DC_DATA_MODE);

   uint32_t num_chunks = length / CHUNK_SIZE_DMA;
   uint32_t remainder  = length % CHUNK_SIZE_DMA;  // phần dư

   // Gửi các chunk đầy
   for(uint32_t i = 0; i < num_chunks; i++)
   {
       while(dma_busy);
       for(int j = 0; j < CHUNK_SIZE_DMA; j += 2) {
           buffers[current_buf][j]     = *(image + image_pos) >> 8;
           buffers[current_buf][j + 1] = *(image + image_pos) & 0xFF;
           image_pos++;
       }
       dma_busy = 1;
       SPIDRV_MTransmit(Instance->handle, buffers[current_buf], CHUNK_SIZE_DMA, spi_callback);
       current_buf = 1 - current_buf;
   }

   // Gửi phần dư
   if(remainder > 0)
   {
       while(dma_busy);
       for(uint32_t j = 0; j < remainder; j += 2) {
           buffers[current_buf][j]     = *(image + image_pos) >> 8;
           buffers[current_buf][j + 1] = *(image + image_pos) & 0xFF;
           image_pos++;
       }
       dma_busy = 1;
       SPIDRV_MTransmit(Instance->handle, buffers[current_buf], remainder, spi_callback);
   }

   while(dma_busy);
   TFT_CS_SetMode(Instance, PIN_HIGH);

   return TFT_OK;
}
#endif



#if 1
TFT_Status_Typedef TFT_DrawImage(TFT_Handler_t *Instance, const uint16_t *image, uint32_t length)
{
    uint8_t *buffers[2] = {buff1, buff2};
    uint8_t current_buf = 0;
    uint32_t image_pos = 0;

    uint32_t num_chunks = length / CHUNK_SIZE_DMA;
    uint32_t remainder  = length % CHUNK_SIZE_DMA;

    TFT_CS_SetMode(Instance, PIN_LOW);      
    TFT_DC_SetMode(Instance, DC_DATA_MODE); 

    if (num_chunks > 0)
    {
        // Fill first chuck & kick DMA
        for (int j = 0; j < CHUNK_SIZE_DMA; j += 2) {
            buffers[0][j]     = *(image + image_pos) >> 8;
            buffers[0][j + 1] = *(image + image_pos) & 0xFF;
            image_pos++;
        }
        dma_busy = 1;
        SPIDRV_MTransmit(Instance->handle, buffers[0], CHUNK_SIZE_DMA, spi_callback);

        // Start double bufing after 1st chuck
        for (uint32_t i = 1; i < num_chunks; i++)
        {
            uint8_t next = 1 - current_buf;

            for (int j = 0; j < CHUNK_SIZE_DMA; j += 2) {
                buffers[next][j]     = *(image + image_pos) >> 8;
                buffers[next][j + 1] = *(image + image_pos) & 0xFF;
                image_pos++;
            }

            while (dma_busy);
            dma_busy = 1;
            SPIDRV_MTransmit(Instance->handle, buffers[next], CHUNK_SIZE_DMA, spi_callback);

            current_buf = next;
        }
    }

    if (remainder > 0)
    {
        uint8_t next = (num_chunks == 0) ? 0 : (1 - current_buf);

        for (int j = 0; j < remainder; j += 2) {
            buffers[next][j]     = *(image + image_pos) >> 8;
            buffers[next][j + 1] = *(image + image_pos) & 0xFF;
            image_pos++;
        }

        while (dma_busy);
        dma_busy = 1;
        SPIDRV_MTransmit(Instance->handle, buffers[next], remainder, spi_callback);
    }

    while (dma_busy);
    TFT_CS_SetMode(Instance, PIN_HIGH);

    return TFT_OK;
}
#endif

#endif
