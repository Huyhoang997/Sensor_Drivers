/*
 * touch_screen.h
 *
 *  Created on: May 10, 2026
 *      Author: ACER
 */

#ifndef TOUCH_SCREEN_H_
#define TOUCH_SCREEN_H_

#include "sl_i2cspm_instances.h"
#include "em_gpio.h"
#include "stdio.h"
#include "gpiointerrupt.h"
#include <sl_sleeptimer.h>

typedef enum
{
    FT6336U_OK,
    FT6336U_ERR,
    FT6336U_I2C_ERR,
    FT6336U_NO_TOUCH
} FT6336U_Status_Typedef;


typedef enum
{
  FT6336U_PIN_LOW = 0U,
  FT6336U_PIN_HIGH
} FT6336U_Mode_PinState;

typedef enum {
    TOUCH_RELEASED = 0,
    TOUCH_PRESSED = 1
} TouchState_t;

typedef struct
{
  GPIO_Port_TypeDef port;
  uint8_t pin;
} FT6336U_RST_Config_t;


typedef struct 
{
    sl_i2cspm_t *i2c_handle;
    FT6336U_RST_Config_t touch_rst_cfg;
} FT6336U_Handle_t;

/* Define INT pin for FT6336U */
#define FT6336U_INT_PORT            gpioPortD
#define FT6336U_INT_PIN             8
/* Slave address */
#define FT6336U_I2C_ADDR            0x38U

/* Registers map */
#define FT6336U_DEVICE_MODE_REG     0x00U  
 #define FT6336U_WORKING_MODE       0x00U
 #define FT6336U_FACTORY_MODE       0x04U
 
#define FT6336U_GESTURE_ID_REG      0x01U

#define FT6336U_TOUCH_DATA_REG      0x02U

#define FT6336U_TOUCH_X_MSB_REG     0x03U
#define FT6336U_TOUCH_X_LSB_REG     0x04U

#define FT6336U_TOUCH_Y_MSB_REG     0x05U
#define FT6336U_TOUCH_Y_LSB_REG     0x06U

#define FT6366U_TOUCH_PRESSURE_VAL_REG  0x07U
#define FT6336U_TOUCH_AREA_VAL_REG  0x08U

#define FT6336U_G_MODE_REG          0xA4U
 #define FT6336U_POLLING_MODE       0x00U
 #define FT6336U_TRIGGER_MODE       0x01U

#define FT6336U_CTRL_TRG                0x86U
 #define FT6336U_KEEP_ACTIVE_MODE       0x00U
 #define FT6336U_SWITCH_ACTIVE_MONITOR  0x01U

FT6336U_Status_Typedef FT6336U_Init(FT6336U_Handle_t *Instance);
#endif /* TOUCH_SCREEN_H_ */
