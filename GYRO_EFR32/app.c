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
#include <stdio.h>
#include <stdint.h>
#include <gyro.h>
#include <sl_sleeptimer.h>
#include <lcd.h>

GYRO_Data_t x_result, y_result, z_result, all_result;
char buf[40];
GYRO_Status_t check;
void app_init(void)
{
  GYRO_Status_t status;
  status = GYRO_Init();
  if(status != GYRO_OK) {
      printf("%d\n", status);
  }
  lcd_Init();
  lcd_cursor(0, 0);
  lcd_printf("x_axis: ");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  check = GYRO_Read_All_Axis(&all_result);

  lcd_cursor(0, 0);
  lcd_printf("x: ");

  snprintf(buf,sizeof(buf), "%d", all_result.received_x_axis);
  lcd_cursor(0, 3);
  lcd_printf("       ");
  lcd_cursor(0, 3);
  lcd_printf(buf);

  snprintf(buf,sizeof(buf), "%d", all_result.received_y_axis);
  lcd_cursor(0, 8);
  lcd_printf("y: ");
  lcd_cursor(0, 11);
  lcd_printf("      ");
  lcd_cursor(0, 11);
  lcd_printf(buf);

  snprintf(buf,sizeof(buf), "%d", all_result.received_z_axis);
  lcd_cursor(1, 0);
  lcd_printf("z: ");
  lcd_cursor(1, 3);
  lcd_printf("      ");
  lcd_cursor(1, 3);
  lcd_printf(buf);
  if(check == GYRO_OK) {
  printf(" x_axis: %d\n y_axis: %d\n z_axis: %d\n\n\n", all_result.received_x_axis,
                                                        all_result.received_y_axis,
                                                        all_result.received_z_axis);
  sl_sleeptimer_delay_millisecond(200);
  }
  else {
      printf("check: %d\n",check);
  }
}
