/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "oled.h"
#include <stdio.h>
#include <stdint.h>
#include "image.h"

void app_init(void)
{
  OLED_Init();
  OLED_SetPos(0, 127, 0, 7);

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  OLED_DrawImage(bitmap_hehe0);
  OLED_DrawImage(bitmap_hehe1);
  OLED_DrawImage(bitmap_hehe2);
  OLED_DrawImage(bitmap_hehe3);
  OLED_DrawImage(bitmap_hehe4);
  OLED_DrawImage(bitmap_hehe5);
  OLED_DrawImage(bitmap_hehe6);
  OLED_DrawImage(bitmap_hehe7);
  OLED_DrawImage(bitmap_hehe8);
  OLED_DrawImage(bitmap_hehe9);
  OLED_DrawImage(bitmap_hehe10);
  OLED_DrawImage(bitmap_hehe11);
  OLED_DrawImage(bitmap_hehe12);
  OLED_DrawImage(bitmap_hehe13);
  OLED_DrawImage(bitmap_hehe14);
  OLED_DrawImage(bitmap_hehe15);
}
