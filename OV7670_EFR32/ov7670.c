/*
 * ov7670.c
 *
 *  Created on: May 19, 2026
 *      Author: ACER
 */
#include "ov7670.h"

uint8_t image_buf[TOTAL_FRAME];

static inline void OV7670_Rst_Mode(uint8_t pin_state)
{
  GPIO_PinModeSet(OV7670_RST_PORT, OV7670_RST_PIN, gpioModePushPull, pin_state);
}


void OV7670_I2C_SendCmd(OV7670_Handler_t *Instance, uint8_t reg_addr, uint8_t cmd)
{
  I2C_TransferSeq_TypeDef i2c_cfg;
  i2c_cfg.addr = OV7670_I2C_ADDR << 1;
  i2c_cfg.flags  = I2C_FLAG_WRITE_WRITE;
  i2c_cfg.buf[0].data = &reg_addr;
  i2c_cfg.buf[0].len = 1;
  i2c_cfg.buf[1].data = &cmd;
  i2c_cfg.buf[1].len = 1;
  I2CSPM_Transfer(Instance->i2c_handler, &i2c_cfg);
}




void OV7670_I2C_ReadReg(OV7670_Handler_t *Instance, uint8_t reg_addr, uint8_t *value, uint16_t len)
{
  I2C_TransferSeq_TypeDef i2c_cfg;
  i2c_cfg.addr = OV7670_I2C_ADDR << 1;
  i2c_cfg.flags  = I2C_FLAG_WRITE;
  i2c_cfg.buf[0].data = &reg_addr;
  i2c_cfg.buf[0].len = 1;
  I2CSPM_Transfer(Instance->i2c_handler, &i2c_cfg);

  i2c_cfg.flags  = I2C_FLAG_READ;
  i2c_cfg.buf[0].data = value;
  i2c_cfg.buf[0].len = len;
  I2CSPM_Transfer(Instance->i2c_handler, &i2c_cfg);
}


static inline uint8_t read_data_from_gpio(void)
{
  uint32_t portc = GPIO_PortInGet(gpioPortC);
  uint32_t portd = GPIO_PortInGet(gpioPortD);

  uint8_t raw_data = 0;

  raw_data |= ((portc >> 8) & 1U) << 0;  // D0 = PC8
  raw_data |= ((portc >> 0) & 1U) << 1;  // D1 = PC0
  raw_data |= ((portc >> 1) & 1U) << 2;  // D2 = PC1
  raw_data |= ((portc >> 2) & 1U) << 3;  // D3 = PC2
  raw_data |= ((portc >> 3) & 1U) << 4;  // D4 = PC3
  raw_data |= ((portc >> 9) & 1U) << 5;  // D5 = PC9
  raw_data |= ((portd >> 3) & 1U) << 6;  // D6 = PD3
  raw_data |= ((portd >> 2) & 1U) << 7;  // D7 = PD2

  return raw_data;
}

void OV7670_ReadData_OnPCLK(void)
{
  uint32_t index = 0;
  /* Wait current frame end */
  while (GPIO_PinInGet(OV7670_VS_PORT, OV7670_VS_PIN) == 0);

  /* Wait next frame start */
  while (GPIO_PinInGet(OV7670_VS_PORT, OV7670_VS_PIN) == 1);

  while ((GPIO_PinInGet(OV7670_VS_PORT, OV7670_VS_PIN) == 0) && (index < TOTAL_FRAME))
  {
    if (GPIO_PinInGet(OV7670_HS_PORT, OV7670_HS_PIN) == 1)
    {
      /* wait PCLK rising */
      while (GPIO_PinInGet(OV7670_PCK_PORT, OV7670_PCK_PIN) == 0);

      /* wait PCLK falling */
      while (GPIO_PinInGet(OV7670_PCK_PORT, OV7670_PCK_PIN) == 1);

      image_buf[index++] = read_data_from_gpio();

    }
  }
  //printf("\n");
  //printf("index %lu\n", index);

}


void OV7670_Init(OV7670_Handler_t *Instance)
{
  GPIO_PinModeSet(OV7670_D0_PORT, OV7670_D0_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D1_PORT, OV7670_D1_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D2_PORT, OV7670_D2_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D3_PORT, OV7670_D3_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D4_PORT, OV7670_D4_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D5_PORT, OV7670_D5_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D6_PORT, OV7670_D6_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_D7_PORT, OV7670_D7_PIN, gpioModeInput, 0);

  GPIO_PinModeSet(OV7670_VS_PORT,  OV7670_VS_PIN,  gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_HS_PORT,  OV7670_HS_PIN,  gpioModeInput, 0);
  GPIO_PinModeSet(OV7670_PCK_PORT, OV7670_PCK_PIN, gpioModeInput, 0);

  /* Enable XCLK Clock for the camera */
  sl_pwm_set_duty_cycle(&sl_pwm_hihi, 45);
  sl_pwm_start(&sl_pwm_hihi);
  sl_sleeptimer_delay_millisecond(300);

  // Hardware reset
  OV7670_Rst_Mode(PIN_LOW);
  sl_sleeptimer_delay_millisecond(100);
  OV7670_Rst_Mode(PIN_HIGH);
  sl_sleeptimer_delay_millisecond(300);

  // Software reset
  OV7670_I2C_SendCmd(Instance, OV7670_SW_RST_REG, OV7670_RST_ALL_VAL);
  sl_sleeptimer_delay_millisecond(100);

  uint8_t id_result[2];
  OV7670_I2C_ReadReg(Instance, OV7670_MIDH_REG, &id_result[0], 1);
  OV7670_I2C_ReadReg(Instance, OV7670_MIDL_REG, &id_result[1], 1);

  // =========================================================
  // 0x11 = CLKRC
  // 0x07 = internal clock XCLK / 8
  // =========================================================
  OV7670_I2C_SendCmd(Instance, OV7670_CLKRC_REG, 0x07);
  sl_sleeptimer_delay_millisecond(10);

  // =========================================================
  // QQVGA YUV base
  // 0x12 = COM7
  // 0x10 = QVGA base + YUV
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x12, 0x10);

  // =========================================================
  // Window
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x17, 0x17); // HSTART
  OV7670_I2C_SendCmd(Instance, 0x18, 0x06); // HSTOP
  OV7670_I2C_SendCmd(Instance, 0x32, 0xB6); // HREF

  OV7670_I2C_SendCmd(Instance, 0x19, 0x02); // VSTART
  OV7670_I2C_SendCmd(Instance, 0x1A, 0x7A); // VSTOP
  OV7670_I2C_SendCmd(Instance, 0x03, 0x0A); // VREF

  // =========================================================
  // Scaling / PCLK divider
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x0C, 0x04); // COM3: DCW enable
  OV7670_I2C_SendCmd(Instance, 0x3E, 0x1A); // COM14: DCW + manual + PCLK /4
  OV7670_I2C_SendCmd(Instance, 0x70, 0x3A); // SCALING_XSC
  OV7670_I2C_SendCmd(Instance, 0x71, 0x35); // SCALING_YSC
  OV7670_I2C_SendCmd(Instance, 0x72, 0x22); // H /4, V /4
  OV7670_I2C_SendCmd(Instance, 0x73, 0xF2); // DSP PCLK /4
  OV7670_I2C_SendCmd(Instance, 0xA2, 0x02); // PCLK delay

  // =========================================================
  // YUV422 output config
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x8C, 0x00); // RGB444 off
  OV7670_I2C_SendCmd(Instance, 0x04, 0x00); // COM1
  OV7670_I2C_SendCmd(Instance, 0x40, 0xC0); // COM15 full range

  // =========================================================
  //
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x13, 0xE0);

  // =========================================================
  // YUV matrix theo lib
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x4F, 0x40); // MTX1
  OV7670_I2C_SendCmd(Instance, 0x50, 0x34); // MTX2
  OV7670_I2C_SendCmd(Instance, 0x51, 0x0C); // MTX3
  OV7670_I2C_SendCmd(Instance, 0x52, 0x17); // MTX4
  OV7670_I2C_SendCmd(Instance, 0x53, 0x29); // MTX5
  OV7670_I2C_SendCmd(Instance, 0x54, 0x40); // MTX6
  OV7670_I2C_SendCmd(Instance, 0x58, 0x1E); // MTXS

  // =========================================================
  // Gamma curve
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x7A, 0x20);
  OV7670_I2C_SendCmd(Instance, 0x7B, 0x1C);
  OV7670_I2C_SendCmd(Instance, 0x7C, 0x28);
  OV7670_I2C_SendCmd(Instance, 0x7D, 0x3C);
  OV7670_I2C_SendCmd(Instance, 0x7E, 0x55);
  OV7670_I2C_SendCmd(Instance, 0x7F, 0x68);
  OV7670_I2C_SendCmd(Instance, 0x80, 0x76);
  OV7670_I2C_SendCmd(Instance, 0x81, 0x80);
  OV7670_I2C_SendCmd(Instance, 0x82, 0x88);
  OV7670_I2C_SendCmd(Instance, 0x83, 0x8F);
  OV7670_I2C_SendCmd(Instance, 0x84, 0x96);
  OV7670_I2C_SendCmd(Instance, 0x85, 0xA3);
  OV7670_I2C_SendCmd(Instance, 0x86, 0xAF);
  OV7670_I2C_SendCmd(Instance, 0x87, 0xC4);
  OV7670_I2C_SendCmd(Instance, 0x88, 0xD7);
  OV7670_I2C_SendCmd(Instance, 0x89, 0xE8);

  // =========================================================
  // AEC / AGC tuning
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x00, 0x00); // GAIN
  OV7670_I2C_SendCmd(Instance, 0x10, 0x00); // AECH
  OV7670_I2C_SendCmd(Instance, 0x0D, 0x00); // COM4
  OV7670_I2C_SendCmd(Instance, 0x14, 0x28); // COM9 gain ceiling

  OV7670_I2C_SendCmd(Instance, 0xA5, 0x05);
  OV7670_I2C_SendCmd(Instance, 0xAB, 0x07);
  OV7670_I2C_SendCmd(Instance, 0x24, 0x75); // AEW
  OV7670_I2C_SendCmd(Instance, 0x25, 0x63); // AEB
  OV7670_I2C_SendCmd(Instance, 0x26, 0xA5); // VPT

  OV7670_I2C_SendCmd(Instance, 0x9F, 0x78);
  OV7670_I2C_SendCmd(Instance, 0xA0, 0x68);
  OV7670_I2C_SendCmd(Instance, 0xA1, 0x03);
  OV7670_I2C_SendCmd(Instance, 0xA6, 0xDF);
  OV7670_I2C_SendCmd(Instance, 0xA7, 0xDF);
  OV7670_I2C_SendCmd(Instance, 0xA8, 0xF0);
  OV7670_I2C_SendCmd(Instance, 0xA9, 0x90);
  OV7670_I2C_SendCmd(Instance, 0xAA, 0x94);

  // =========================================================
  // AWB control
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x43, 0x14);
  OV7670_I2C_SendCmd(Instance, 0x44, 0xF0);
  OV7670_I2C_SendCmd(Instance, 0x45, 0x34);
  OV7670_I2C_SendCmd(Instance, 0x46, 0x58);
  OV7670_I2C_SendCmd(Instance, 0x47, 0x28);
  OV7670_I2C_SendCmd(Instance, 0x48, 0x3A);

  OV7670_I2C_SendCmd(Instance, 0x59, 0x88);
  OV7670_I2C_SendCmd(Instance, 0x5A, 0x88);
  OV7670_I2C_SendCmd(Instance, 0x5B, 0x44);
  OV7670_I2C_SendCmd(Instance, 0x5C, 0x67);
  OV7670_I2C_SendCmd(Instance, 0x5D, 0x49);
  OV7670_I2C_SendCmd(Instance, 0x5E, 0x0E);

  OV7670_I2C_SendCmd(Instance, 0x6C, 0x0A);
  OV7670_I2C_SendCmd(Instance, 0x6D, 0x55);
  OV7670_I2C_SendCmd(Instance, 0x6E, 0x11);
  OV7670_I2C_SendCmd(Instance, 0x6F, 0x9F);

  // =========================================================
  // Manual color gain tuning
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x6A, 0x59); // GREEN
  OV7670_I2C_SendCmd(Instance, 0x01, 0x46); // BLUE
  OV7670_I2C_SendCmd(Instance, 0x02, 0x45); // RED

  // =========================================================
  // Saturation / contrast
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x56, 0x60); // CONTRAST
  OV7670_I2C_SendCmd(Instance, 0xC9, 0x26); // SATCTR
  // =========================================================
  // COM16 enhancement
  // 0x38 theo lib: YUV enhancement + denoise + AWB gain
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x41, 0x38);

  // Edge off
  OV7670_I2C_SendCmd(Instance, 0x3F, 0x00);

  // =========================================================
  // COM13
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x3D, 0xC0);

  // =========================================================
  // Clocl Configure

  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x96, 0x00);
  OV7670_I2C_SendCmd(Instance, 0x97, 0x30);
  OV7670_I2C_SendCmd(Instance, 0x98, 0x20);
  OV7670_I2C_SendCmd(Instance, 0x99, 0x30);
  OV7670_I2C_SendCmd(Instance, 0x9A, 0x84);
  OV7670_I2C_SendCmd(Instance, 0x9B, 0x29);
  OV7670_I2C_SendCmd(Instance, 0x9C, 0x03);
  OV7670_I2C_SendCmd(Instance, 0x9D, 0x4C);
  OV7670_I2C_SendCmd(Instance, 0x9E, 0x3F);
  OV7670_I2C_SendCmd(Instance, 0x78, 0x04);

  OV7670_I2C_SendCmd(Instance, 0x79, 0x01);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0xF0);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x0F);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x00);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x10);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x7E);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x0A);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x80);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x0B);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x01);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x0C);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x0F);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x0D);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x20);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x09);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x80);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x02);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0xC0);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x03);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x40);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x05);
  OV7670_I2C_SendCmd(Instance, 0xC8, 0x30);
  OV7670_I2C_SendCmd(Instance, 0x79, 0x26);

  // =========================================================
  // auto exposure / auto gain / auto white balance
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x13, 0x8F);

  // =========================================================
  // Color bar
  // =========================================================
  OV7670_I2C_SendCmd(Instance, 0x42, 0x00);
  OV7670_I2C_SendCmd(Instance, 0x12, 0x10);
  OV7670_I2C_SendCmd(Instance, 0xB0, 0x84); // No clue what is this but its the most important for colors

  sl_sleeptimer_delay_millisecond(500);
}
