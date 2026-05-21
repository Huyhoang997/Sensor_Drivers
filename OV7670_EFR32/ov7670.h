/*
 * ov7670.h
 *
 *  Created on: May 19, 2026
 *      Author: ACER
 */

#ifndef OV7670_H_
#define OV7670_H_

#include "sl_i2cspm_instances.h"
#include "app.h"
#include "sl_pwm_instances.h"
#include "em_gpio.h"
#include "sl_sleeptimer.h"
#include "stdio.h"


typedef struct
{
    sl_i2cspm_t *i2c_handler;
} OV7670_Handler_t;


#define IMAGE_HEIGHT            120
#define IMAGE_WEIGHT            156
#define TOTAL_FRAME             (IMAGE_HEIGHT * IMAGE_WEIGHT * 2)

extern uint8_t image_buf[TOTAL_FRAME];

#define PIN_LOW                 0U
#define PIN_HIGH                1U
#define OV7670_I2C_ADDR         0x21U


#define OV7670_SW_RST_REG      0x12U
 #define OV7670_NORMAL_OP      0x0U
 #define OV7670_RST_ALL_VAL    0x80U

#define OV7670_MIDH_REG       0x1CU
#define OV7670_MIDL_REG       0xA2U

#define OV7670_PID_REG        0x0A
#define OV7670_VER_REG        0x0B

/* OV7670 QQVGA-YUV macro setting */
#define OV7670_CLKRC_REG      0x11
#define OV7670_COM14_REG      0x3E
#define REG_SCALING_PCLK_DIV  0x73



#define OV7670_RST_PORT       gpioPortB
#define OV7670_RST_PIN        0

#define OV7670_VS_PORT        gpioPortD
#define OV7670_VS_PIN         4
#define OV7670_HS_PORT        gpioPortD
#define OV7670_HS_PIN         5
#define OV7670_PCK_PORT       gpioPortB
#define OV7670_PCK_PIN        1

#define OV7670_D0_PORT       gpioPortC
#define OV7670_D0_PIN        8
#define OV7670_D1_PORT       gpioPortC
#define OV7670_D1_PIN        0
#define OV7670_D2_PORT       gpioPortC
#define OV7670_D2_PIN        1
#define OV7670_D3_PORT       gpioPortC
#define OV7670_D3_PIN        2
#define OV7670_D4_PORT       gpioPortC
#define OV7670_D4_PIN        3
#define OV7670_D5_PORT       gpioPortC
#define OV7670_D5_PIN        9
#define OV7670_D6_PORT       gpioPortD
#define OV7670_D6_PIN        3
#define OV7670_D7_PORT       gpioPortD
#define OV7670_D7_PIN        2
void OV7670_Init(OV7670_Handler_t *Instance);
void OV7670_ReadData_OnPCLK(void);

#endif /* OV7670_H_ */
