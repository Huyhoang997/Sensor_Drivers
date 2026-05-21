/*
 * gyro.h
 *
 *  Created on: Dec 28, 2025
 *      Author: ACER
 */

#ifndef GYRO_H_
#define GYRO_H_

#include <stdint.h>

/* ===================== ADDR_REG ======================= */
#define GYRO_ADDR             (0x69 << 1)
#define WHO_AM_I              0x0F

/* ===================== CTRl_REG ======================= */
#define CTRL_REG1             0x20
  #define REG1_NORMAL_MODE    0xBF
#define CTRL_REG2             0x21
  #define REG2_NORMAL_MODE    0x09
#define CTRL_REG3             0x22
  #define REG3_DEFAULT_MODE   0x00
#define CTRL_REG4             0x23
  #define REG4_DEFAULT_MODE   0x00
#define CTRL_REG5             0x24
  #define REG5_DEFAULT_MODE   0x00
#define DATACAPTURE_REG       0x25
#define OUT_TEMP              0x26
#define STATUS_REG            0x27    /* Read_only_reg */
#define FIFO_CTRL_REG         0x2E

/* ===================== READ_DATA_REG ==================== */
#define OUT_X_L               0x28
#define OUT_X_H               0x29
#define OUT_Y_L               0x2A
#define OUT_Y_H               0x2B
#define OUT_Z_L               0x2C
#define OUT_Z_H               0x2D

typedef enum {
  GYRO_OK,
  GYRO_TIMEOUT,
  GYRO_ERR,
  GYRO_I2C_ERR,
  GYRO_WRONG_ID,
  GYRO_READ_FAIL
} GYRO_Status_t;

typedef struct {
  int16_t received_x_axis;
  int16_t received_y_axis;
  int16_t received_z_axis;
} GYRO_Data_t;

/*=================== App_Function ==================== */
GYRO_Status_t GYRO_Init(void);

GYRO_Status_t GYRO_Read_X_Axis(GYRO_Data_t *ptr);

GYRO_Status_t GYRO_Read_Y_Axis(GYRO_Data_t *ptr);

GYRO_Status_t GYRO_Read_Z_Axis(GYRO_Data_t *ptr);

GYRO_Status_t GYRO_Read_All_Axis(GYRO_Data_t *ptr);

#endif /* GYRO_H_ */
