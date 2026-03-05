#ifndef BMP280_H_
#define BMP280_H_

#include<stdint.h>
#include "main.h"
#include "stdbool.h"

/* Define macro for BMP280 status */
typedef enum 
{
    BMP280_OK,
    BMP280_ERR,
    BMP280_I2C_ERR,
    BMP280_INVALID_ID,
    BMP280_TIMEOUT,
    BMP280_READ_PRESSURE_ERR
} BMP280_Status_Typedef;

typedef struct 
{
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    int32_t rawTempData;
    int32_t rawPressData;
} BMP280_Calib_Config_t;


/* BMP280 power mode definition structure */
typedef enum
{
    BMP280_SLEEP_MODE = 0U,
    BMP280_FORCE_MODE,
    BMP280_NORMAL_MODE = 3U
} BMP280_Power_Mode;

/* BMP280 temperature oversampling definition structure */
typedef enum
{
    BMP280_SKIP_OVERS_T = 0U,
    BMP280_OVERS_X1_T,
    BMP280_OVERS_X2_T,
    BMP280_OVERS_X4_T,
    BMP280_OVERS_X8_T,
    BMP280_OVERS_X16_T
} BMP280_TempOversampling;

/* BMP280 pressure oversampling definition structure */
typedef enum
{
    BMP280_SKIP_OVERS_P = 0U,
    BMP280_OVERS_X1_P,
    BMP280_OVERS_X2_P,
    BMP280_OVERS_X4_P,
    BMP280_OVERS_X8_P,
    BMP280_OVERS_X16_P 
} BMP280_PressOversampling;


typedef enum
{
    STANDBY_HALF_MS = 0U,
    STANDBY_62_HALF_MS,
    STANDBY_125_MS,
    STANDBY_250_MS,
    STANDBY_500_MS,
    STANDBY_1000_MS,
    STANDBY_2000_MS,
    STANDBY_4000_MS
} BMP280_NormalMode_Standby;
typedef enum
{
    BMP280_FILTER_OFF       = 0U,
    BMP280_FILTER_COEF_2,
    BMP280_FILTER_COEF_4,
    BMP280_FILTER_COEF_8,
    BMP280_FILTER_COEF_16,
} BMP280_ConfigFilter;

/* BMP280 initial configure structure */
typedef struct
{
    I2C_HandleTypeDef *hi2c;
    uint8_t Address;
    BMP280_Power_Mode Mode;
    BMP280_TempOversampling osrs_T;
    BMP280_PressOversampling osrs_P;
    BMP280_ConfigFilter FilterSetting;
    BMP280_NormalMode_Standby t_standby;
} BMP280_Handler_t;

/* Device address and ID */
#define BMP280_ADDR            (0xEC)
#define BMP280_ID_REG           0xD0

#define BMP280_CTRL_MEAS        0xF4 
#define BMP280_CONFIG_REG       0xF5
#define BMP280_STATUS_REG       0xF3

#define BMP280_DIG_T1_REG       0x88
#define BMP280_DIG_T2_REG       0x8A
#define BMP280_DIG_T3_REG       0x8C

#define BMP280_DIG_P1_REG       0x8E
#define BMP280_DIG_P2_REG       0x90
#define BMP280_DIG_P3_REG       0x92
#define BMP280_DIG_P4_REG       0x94
#define BMP280_DIG_P5_REG       0x96
#define BMP280_DIG_P6_REG       0x98
#define BMP280_DIG_P7_REG       0x9A
#define BMP280_DIG_P8_REG       0x9C
#define BMP280_DIG_P9_REG       0x9E

#define BMP280_PRESS_MSB        0xF7
#define BMP280_PRESS_LSB        0xF8
#define BMP280_PRESS_XLSB       0xF9

#define BMP280_TEMP_MSB         0xFA
#define BMP280_TEMP_LSB         0xFB
#define BMP280_TEMP_XLSB        0xFC


BMP280_Status_Typedef BMP280_Init(BMP280_Handler_t *Instance, uint16_t Timeout);
BMP280_Status_Typedef BMP280_ReadTemp(BMP280_Handler_t *Instance, int32_t *temp_result, uint16_t Timeout);
BMP280_Status_Typedef BMP280_ReadPressure(BMP280_Handler_t *Instance, uint32_t *press_result, uint16_t Timeout);

#endif
