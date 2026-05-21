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
#include <sl_spidrv_instances.h>
#include <em_gpio.h>
#include "lv_port_disp.h"
//#include "lvgl/examples/anim/lv_example_anim.h"
#include "em_cmu.h"
#include "em_device.h"
//#include "lvgl/demos/benchmark/lv_demo_benchmark.h"
#include "ui/ui.h"
#include "screens.h"
#include "touch_screen.h"

void SysTick_Handler(void)
{
  lv_tick_inc(1);
}

FT6336U_Handle_t touch_screen;
FT6336U_Status_Typedef status;
uint16_t x_value, y_value;


void my_touchpad_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    uint16_t touch_x = 0;
    uint16_t touch_y = 0;

    // FT6336U_GetTouchPos trả về FT6336U_OK khi có người nhấn
    if(FT6336U_GetTouchPos(&touch_screen, &touch_x, &touch_y) == FT6336U_OK)
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = MY_DISP_HOR_RES - touch_x;
        data->point.y = MY_DISP_VER_RES - touch_y;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void lvgl_port_indev_init(void)
{
    /* 1. Tạo đối tượng Input Device */
    lv_indev_t * indev = lv_indev_create();

    /* 2. Thiết lập loại thiết bị là Pointer (Cảm ứng/Chuột) */
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

    /* 3. Gán hàm callback đọc dữ liệu */
    lv_indev_set_read_cb(indev, my_touchpad_read);
}

void app_init(void)
{
  SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

  touch_screen.i2c_handle = sl_i2cspm_touch;
  touch_screen.touch_rst_cfg.port = gpioPortD;
  touch_screen.touch_rst_cfg.pin = 9;
  status = FT6336U_Init(&touch_screen);
  if(status != FT6336U_OK)
  {
    printf("Touch screen error ID: %d\n", status);
  }

  lv_init();
  lv_port_disp_init();
  lvgl_port_indev_init();
  //ui_init();
  //Adc_Init();
  lv_demo_benchmark();

  //lv_example_anim_2();
  //lv_label_set_text(objects.ui_label_adc_value, "888");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler();
  //ui_tick();
  sl_sleeptimer_delay_millisecond(5);
}
