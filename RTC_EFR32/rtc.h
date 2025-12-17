/*
 * rtc.h
 *
 *  Created on: Nov 20, 2025
 *      Author: ACER
 */
#include <stdint.h>
#ifndef RTC_H_
#define RTC_H_

#define RTC_ADDRESS     (0x68 << 1)

void Rtc_SetTime(uint8_t sec, uint8_t min, uint8_t hour);
void Rtc_ReadTime(void);

#endif /* RTC_H_ */
