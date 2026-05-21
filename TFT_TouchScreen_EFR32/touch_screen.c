/*
 * touch_screen.c
 *
 *  Created on: May 10, 2026
 *      Author: ACER
 */
#include "touch_screen.h"

volatile uint8_t touch_action = 0;

void pin_int_callback(uint8_t intNo)
{
    (void)intNo;
    if (GPIO_PinInGet(FT6336U_INT_PORT, FT6336U_INT_PIN) == 0) {
        touch_action = 1;
        //printf("Phat hien nhan!\n");
    }
}


static inline void FT6336U_RST_SetMode(FT6336U_Handle_t *Instance, FT6336U_Mode_PinState PinState)
{
  GPIO_PinModeSet(Instance->touch_rst_cfg.port, Instance->touch_rst_cfg.pin, gpioModePushPull, PinState);
}



FT6336U_Status_Typedef FT6336U_WriteReg(FT6336U_Handle_t *Instance, uint8_t reg_addr, uint8_t cmd)
{
  if(Instance == NULL)
  {
      return FT6336U_ERR;
  }
  I2C_TransferSeq_TypeDef i2c_transfer;
  I2C_TransferReturn_TypeDef status;

  i2c_transfer.addr = FT6336U_I2C_ADDR << 1;
  i2c_transfer.flags = I2C_FLAG_WRITE_WRITE;
  i2c_transfer.buf[0].data = &reg_addr;
  i2c_transfer.buf[0].len = 1;
  i2c_transfer.buf[1].data = &cmd;
  i2c_transfer.buf[1].len = 1;

  status = I2CSPM_Transfer(Instance->i2c_handle, &i2c_transfer);
  if(status != i2cTransferDone)
  {
      printf("i2c err id: %d\n", status);
    return FT6336U_I2C_ERR;
  }
  return FT6336U_OK;
}


FT6336U_Status_Typedef FT6336U_ReadReg(FT6336U_Handle_t *Instance, uint8_t reg_addr, uint8_t *value, uint16_t length)
{
  if(Instance == NULL)
  {
      return FT6336U_ERR;
  }
  I2C_TransferSeq_TypeDef i2c_transfer;
  I2C_TransferReturn_TypeDef status;

  i2c_transfer.addr = FT6336U_I2C_ADDR << 1;
  i2c_transfer.flags = I2C_FLAG_WRITE_READ;
  i2c_transfer.buf[0].data = &reg_addr;
  i2c_transfer.buf[0].len = 1;
  i2c_transfer.buf[1].data = value;
  i2c_transfer.buf[1].len = length;

  status = I2CSPM_Transfer(Instance->i2c_handle, &i2c_transfer);
  if(status != i2cTransferDone)
  {
    return FT6336U_I2C_ERR;
  }
  return FT6336U_OK;
}


FT6336U_Status_Typedef FT6336U_GetTouchPos(FT6336U_Handle_t *Instance, uint16_t *x_pos, uint16_t *y_pos)
{
  if(Instance == NULL)
  {
      return FT6336U_ERR;
  }
  uint8_t buf[6];
  static TouchState_t current_state = 0;

  switch (touch_action) {
      case 1:
        if(FT6336U_ReadReg(Instance, FT6336U_TOUCH_X_MSB_REG, buf, 6) == FT6336U_OK)
        {
          *x_pos = ((buf[0] & 0x0F) << 8) | buf[1];
          *y_pos = ((buf[2] & 0x0F) << 8) | buf[3];
          touch_action = 0;
          printf("x_pos: %d\n", *x_pos);
          printf("y_pos: %d\n", *y_pos);

          current_state = TOUCH_PRESSED;
          touch_action = 0;

          return FT6336U_OK;
        }
      break;

      case 0:
        if(current_state == TOUCH_PRESSED)
        {
        *x_pos = 0;
        *y_pos = 0;
        current_state = TOUCH_RELEASED;
        touch_action = TOUCH_RELEASED;
        }
      break;

    default:
      break;
  }

  return FT6336U_NO_TOUCH;
}
FT6336U_Status_Typedef FT6336U_Init(FT6336U_Handle_t *Instance)
{
    if(Instance == NULL)
    {
        return FT6336U_ERR;
    }
    FT6336U_Status_Typedef status;
    GPIOINT_Init();
    GPIO_PinModeSet(FT6336U_INT_PORT, FT6336U_INT_PIN, gpioModeInputPull, 1);
    GPIOINT_CallbackRegister(FT6336U_INT_PIN, pin_int_callback);
    GPIO_ExtIntConfig(FT6336U_INT_PORT, FT6336U_INT_PIN, FT6336U_INT_PIN, false, true, true);


    FT6336U_RST_SetMode(Instance, FT6336U_PIN_LOW);
    sl_sleeptimer_delay_millisecond(3);
    FT6336U_RST_SetMode(Instance, FT6336U_PIN_HIGH);
    sl_sleeptimer_delay_millisecond(300);

    status = FT6336U_WriteReg(Instance, FT6336U_DEVICE_MODE_REG, FT6336U_WORKING_MODE);
    if(status != FT6336U_OK)
    {
        return status;
    }

    status = FT6336U_WriteReg(Instance, FT6336U_CTRL_TRG, FT6336U_KEEP_ACTIVE_MODE);
    if(status != FT6336U_OK)
    {
        return status;
    }

    return FT6336U_OK;
}

