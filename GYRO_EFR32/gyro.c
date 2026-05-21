/*
 * gyro.c
 *
 *  Created on: Dec 28, 2025
 *      Author: ACER
 */
#include "gyro.h"
#include "sl_i2cspm_instances.h"
#include <stdio.h>


static const uint8_t reg1_config[2] = {CTRL_REG1, REG1_NORMAL_MODE};
static const uint8_t reg2_config[2] = {CTRL_REG2, REG2_NORMAL_MODE};
static const uint8_t reg3_config[2] = {CTRL_REG3, REG3_DEFAULT_MODE};
static const uint8_t reg4_config[2] = {CTRL_REG4, REG4_DEFAULT_MODE};
static const uint8_t reg5_config[2] = {CTRL_REG5, REG5_DEFAULT_MODE};

static GYRO_Status_t GYRO_SendCmd(const uint8_t *cmd) {
  I2C_TransferSeq_TypeDef i2c_tranfer_cmd;
  I2C_TransferReturn_TypeDef status;
  i2c_tranfer_cmd.addr = GYRO_ADDR;
  i2c_tranfer_cmd.flags = I2C_FLAG_WRITE;
  i2c_tranfer_cmd.buf[0].data = (uint8_t *)cmd;
  i2c_tranfer_cmd.buf[0].len = 2;
  status = I2CSPM_Transfer(sl_i2cspm_hehe, &i2c_tranfer_cmd);

  if(status != i2cTransferDone) {
      return GYRO_I2C_ERR;
  }
  return GYRO_OK;
}

static GYRO_Status_t GYRO_Received_Half_Byte(uint8_t cmd, uint8_t *data) {
  if(data == NULL) {
      return GYRO_READ_FAIL;
  }

  I2C_TransferSeq_TypeDef i2c_tranfer_data;
  I2C_TransferReturn_TypeDef status;

  i2c_tranfer_data.addr = GYRO_ADDR;
  i2c_tranfer_data.flags = I2C_FLAG_WRITE_READ;
  i2c_tranfer_data.buf[0].data = &cmd;
  i2c_tranfer_data.buf[0].len = 1;
  i2c_tranfer_data.buf[1].data = data;
  i2c_tranfer_data.buf[1].len = 1;

  status = I2CSPM_Transfer(sl_i2cspm_hehe, &i2c_tranfer_data);

  if(status != i2cTransferDone) {
      return GYRO_I2C_ERR;
  }
  return GYRO_OK;
}

static GYRO_Status_t GYRO_CheckId(uint8_t id) {
  uint8_t id_result = 0;
  GYRO_Status_t status;
  status = GYRO_Received_Half_Byte(id, &id_result);

  if(status != GYRO_OK) {
      return status;
  }

  if(id_result != 0xD3) {
      return GYRO_WRONG_ID;
  }
  return GYRO_OK;
}


GYRO_Status_t  GYRO_Read_X_Axis(GYRO_Data_t *ptr) {
  uint8_t x_result_low = 0;
  uint8_t x_result_high = 0;

  GYRO_Status_t status;

  status = GYRO_Received_Half_Byte(OUT_X_H, &x_result_high);
  if(status != GYRO_OK) {
      return GYRO_READ_FAIL;
  }

  status = GYRO_Received_Half_Byte(OUT_X_L, &x_result_low);
  if(status != GYRO_OK) {
      return GYRO_READ_FAIL;
  }

  ptr->received_x_axis = (x_result_high << 8) | (x_result_low);
  return GYRO_OK;
}


GYRO_Status_t  GYRO_Read_Y_Axis(GYRO_Data_t *ptr) {
  uint8_t y_result_low = 0;
  uint8_t y_result_high = 0;

  GYRO_Status_t status;

  status = GYRO_Received_Half_Byte(OUT_Y_H, &y_result_high);
  if(status != GYRO_OK) {
      return GYRO_READ_FAIL;
  }

  status = GYRO_Received_Half_Byte(OUT_Y_L, &y_result_low);
  if(status != GYRO_OK) {
      return GYRO_READ_FAIL;
  }

  ptr->received_y_axis = (y_result_high << 8) | (y_result_low);
  return GYRO_OK;
}


GYRO_Status_t GYRO_Read_Z_Axis(GYRO_Data_t *ptr) {
  uint8_t z_result_low = 0;
  uint8_t z_result_high = 0;

  GYRO_Status_t status;

  status = GYRO_Received_Half_Byte(OUT_Z_H, &z_result_high);
  if(status != GYRO_OK) {
      return GYRO_READ_FAIL;
  }

  status = GYRO_Received_Half_Byte(OUT_Z_L, &z_result_low);
  if(status != GYRO_OK) {
      return GYRO_READ_FAIL;
  }

  ptr->received_z_axis = (z_result_high << 8) | (z_result_low);
  return GYRO_OK;
}


GYRO_Status_t GYRO_Read_All_Axis(GYRO_Data_t *ptr) {
  if(ptr == NULL) {
    return GYRO_READ_FAIL;
  }

  uint8_t buffer[6];
  uint8_t start_reg = OUT_X_L | 0x80;  // Set bit 7 để auto-increment

  I2C_TransferSeq_TypeDef i2c_transfer_data;
  i2c_transfer_data.addr = GYRO_ADDR;
  i2c_transfer_data.flags = I2C_FLAG_WRITE_READ;
  i2c_transfer_data.buf[0].data = &start_reg;
  i2c_transfer_data.buf[0].len = 1;
  i2c_transfer_data.buf[1].data = buffer;
  i2c_transfer_data.buf[1].len = 6;

  I2C_TransferReturn_TypeDef status = I2CSPM_Transfer(sl_i2cspm_hehe, &i2c_transfer_data);
  if(status != i2cTransferDone) {
    return GYRO_I2C_ERR;
  }

  //low + high bytes
  ptr->received_x_axis = (buffer[1] << 8) | buffer[0];
  ptr->received_y_axis = (buffer[3] << 8) | buffer[2];
  ptr->received_z_axis = (buffer[5] << 8) | buffer[4];

  return GYRO_OK;
}


GYRO_Status_t GYRO_Init(void) {
  uint8_t status = GYRO_CheckId(WHO_AM_I);
  if(status != GYRO_OK) {
      return GYRO_WRONG_ID;
  }

  GYRO_SendCmd(reg1_config);
  GYRO_SendCmd(reg2_config);
  GYRO_SendCmd(reg3_config);
  GYRO_SendCmd(reg4_config);
  GYRO_SendCmd(reg5_config);

  return GYRO_OK;
}



