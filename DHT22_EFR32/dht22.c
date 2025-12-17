/*
 * dht22.c
 *
 *  Created on: Dec 13, 2025
 *      Author: ACER
 */
#include <stdio.h>
#include <stdint.h>
#include <sl_udelay.h>
#include <em_gpio.h>
#include <sl_sleeptimer.h>
#include "dht22.h"

#define TIME_OUT              200
#define DHT22_RESPONSE_TIME   40

uint16_t temp_data, humid_data;

static inline void DHT22_CmdLow() {
  GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 0);
}

static inline void DHT22_CmdHigh() {
  GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 1);
}

static inline void DHT22_ModeData() {
  GPIO_PinModeSet(gpioPortC, 1, gpioModeInputPull, 1);
}

static inline uint8_t DHT22_GetDataPin() {
  return GPIO_PinInGet(gpioPortC, 1);
}

void DHT22_SendCmd(void) {
  DHT22_CmdLow();
  sl_sleeptimer_delay_millisecond(1);

  DHT22_CmdHigh();
  sl_udelay_wait(30);

  DHT22_ModeData();
}

DHT22_Status_t DHT22_WaitResponse() {
  while (!(DHT22_GetDataPin())) {
      sl_udelay_wait(70);
      if(DHT22_GetDataPin()) {
          sl_udelay_wait(72);
          return DHT22_OK;
      }
  }
  return DHT22_ERR;
}

uint8_t DHT22_Read_1Bit() {
  uint16_t timeout = 0;
  while (!DHT22_GetDataPin()) {
    if (++timeout > 100) {
      return 0;  // Timeout
    }
  }
  sl_udelay_wait(35);
  if (DHT22_GetDataPin()) {
    timeout = 0;
    while (DHT22_GetDataPin()) {
      sl_udelay_wait(1);
      if (++timeout > 100) {
        break;
      }
    }
    return 1;
  }
  else {
    return 0;
  }
}

uint8_t DHT22_ReadByte() {
  uint8_t byte = 0;
  for(uint8_t i = 0; i < 8; i++) {
      byte = byte << 1;
      byte |= DHT22_Read_1Bit();
  }
  return byte;
}

DHT22_Status_t DHT22_ReadData() {
  uint8_t humid_low, humid_high;
  uint8_t temp_low, temp_high;
  uint8_t checksum_byte;
  DHT22_Status_t status;
  uint32_t calculate_checksum;

  DHT22_Data_t result;

  DHT22_SendCmd();
  status = DHT22_WaitResponse();

  humid_high = DHT22_ReadByte();
  humid_low = DHT22_ReadByte();
  temp_high = DHT22_ReadByte();
  temp_low = DHT22_ReadByte();
  checksum_byte = DHT22_ReadByte();
  calculate_checksum = humid_high + humid_low + temp_high + temp_low;

  result.humid = (humid_high << 8) | (humid_low);
  result.temp = (temp_high << 8) | (temp_low);
  temp_data = (result.temp)/10;
  humid_data = (result.humid)/10;

  if(status != DHT22_OK) {
      return DHT22_NO_RESPONSE;
  }

  if(calculate_checksum != checksum_byte) {
      printf("checksum_err\n");
      return DHT22_CHECKSUM_ERR;
  }
  //printf("humid: %08x\n", result.humid);
  //printf("temp: %08x\n", result.temp);
  return DHT22_OK;
}

void DHT22_PrintHumid(void) {
  printf("humid_data: %d", humid_data);
  printf("%%\n");
}

void DHT22_PrintTemp(void)  {
  printf("temp_data: %d C\n", temp_data);
}





