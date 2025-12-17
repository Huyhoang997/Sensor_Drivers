/*
 * dht22.h
 *
 *  Created on: Dec 13, 2025
 *      Author: ACER
 */
#include <stdint.h>

#ifndef DHT22_H_
#define DHT22_H_

extern uint16_t temp_data, humid_data;

typedef struct {
  uint16_t humid;
  uint16_t temp;
} DHT22_Data_t;

typedef enum {
  DHT22_OK,
  DHT22_TIMEOUT,
  DHT22_CHECKSUM_ERR,
  DHT22_NO_RESPONSE,
  DHT22_ERR
} DHT22_Status_t;

DHT22_Status_t DHT22_ReadData();
void DHT22_PrintHumid(void);
void DHT22_PrintTemp(void);

#endif /* DHT22_H_ */
