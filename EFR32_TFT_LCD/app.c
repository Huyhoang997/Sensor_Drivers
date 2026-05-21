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
#include <em_iadc.h>


void SysTick_Handler(void)
{
  lv_tick_inc(1);
}

#if 1
void update_adc_label(uint32_t value) {
    char buf[10];
    // Chuyển số ADC thành chuỗi
    sprintf(buf, "%lu", value);

    // Cập nhật lên màn hình qua object mà EEZ đã tạo
    // Lưu ý: Kiểm tra chính xác tên biến trong objects struct của bạn
    lv_label_set_text(objects.ui_label_adc_value, buf);
}
#endif


uint32_t simulate_adc_value(void) {
    static uint32_t val = 0;
    static int8_t direction = 1; // 1 là tăng, -1 là giảm

    // Mỗi lần gọi, giá trị tăng/giảm một khoảng ngẫu nhiên hoặc cố định
    val += (direction * 50);

    // Đảo chiều khi chạm ngưỡng để tạo hiệu ứng sóng (Triangle wave)
    if (val >= 4000) {
        val = 4000;
        direction = -1;
    } else if (val <= 100) {
        val = 100;
        direction = 1;
    }

    return val;
}


void Adc_Init() {
  //clock config
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_IADC0, true);

  //iadc_config
  GPIO-> BBUSALLOC |= GPIO_BBUSALLOC_BEVEN0_ADC0;
  GPIO-> BBUSALLOC |= GPIO_BBUSALLOC_BODD1_ADC0;
  IADC_Init_t adc_init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t all_config = IADC_ALLCONFIGS_DEFAULT;
  const IADC_InitSingle_t init_single = IADC_INITSINGLE_DEFAULT;
  IADC_SingleInput_t  single_input = IADC_SINGLEINPUT_DEFAULT;
  single_input.posInput = iadcPosInputPortDPin9;
  all_config.configs->analogGain = iadcCfgAnalogGain0P5x;

  //iadc init
  IADC_reset(IADC0);
  IADC_init(IADC0, &adc_init, &all_config);
  IADC_initSingle(IADC0, &init_single, &single_input);

}

void adc_read() {
    IADC_command(IADC0, iadcCmdStartSingle);

    // Đợi cho đến khi có dữ liệu hợp lệ (Data Valid)
    while(!(IADC0->STATUS & IADC_STATUS_SINGLEQEN));

    IADC_Result_t result = IADC_readSingleResult(IADC0);
    update_adc_label(result.data);
}

void app_init(void)
{
  SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

  lv_init();
  lv_port_disp_init();
  ui_init();
  Adc_Init();
  //lv_demo_benchmark();
  //lv_example_anim_2();
  //lv_label_set_text(objects.ui_label_adc_value, "888");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler();
  ui_tick();
  sl_sleeptimer_delay_millisecond(5);
#if 1
  static uint32_t last_adc_time = 0;
  uint32_t now = sl_sleeptimer_get_tick_count64();

  // CHUYỂN ĐỔI 2000ms SANG TICK
  uint32_t ticks_to_wait = sl_sleeptimer_ms_to_tick(1995);

  if (now - last_adc_time > ticks_to_wait) {
      uint32_t fake_adc = simulate_adc_value();
      update_adc_label(fake_adc);

      last_adc_time = now;
  }
#endif
}
