#include <sl_i2cspm_instances.h>
#include <stdio.h>
#include <stdint.h>
#include <sl_sleeptimer.h>
#include <rtc.h>

uint8_t reg_sec_auto = 0x00;

static uint8_t Bcd_Convert(uint8_t index) {
  return ((index / 10) << 4) | (index % 10);
}

static uint8_t Demical_Convert(uint8_t index) {
  return ((index >> 4) * 10) + (index & 0x0F);
}

void Rtc_SetTime(uint8_t sec, uint8_t min, uint8_t hour) {
  uint8_t tx[4];
  tx[0] = reg_sec_auto;
  tx[1] = Bcd_Convert(sec);
  tx[2] = Bcd_Convert(min);
  tx[3] = Bcd_Convert(hour);

  I2C_TransferSeq_TypeDef i2c_cmd;
  i2c_cmd.addr = RTC_ADDRESS;
  i2c_cmd.flags = I2C_FLAG_WRITE;
  i2c_cmd.buf[0].data = tx;
  i2c_cmd.buf[0].len = 4;

  //Check_status
  I2C_TransferReturn_TypeDef status;
  status = I2CSPM_Transfer(sl_i2cspm_hehe, &i2c_cmd);
  if(status != i2cTransferDone) {
      printf("I2C_co_loi!\n");
  }
}

void Rtc_ReadTime(void) {
  uint8_t data_read[3];
  I2C_TransferSeq_TypeDef i2c_data;
  i2c_data.addr = RTC_ADDRESS;
  i2c_data.flags = I2C_FLAG_WRITE_READ;
  i2c_data.buf[0].data = &reg_sec_auto;
  i2c_data.buf[0].len = 1;     //gio_phut_giay

  i2c_data.buf[1].data = data_read;
  i2c_data.buf[1].len = 3;

  //Check_status
  I2C_TransferReturn_TypeDef status;
  status = I2CSPM_Transfer(sl_i2cspm_hehe, &i2c_data);
  if(status == i2cTransferDone) {
  printf("giay: %d phut: %d gio: %d\n", Demical_Convert(data_read[0]),
                                        Demical_Convert(data_read[1]),
                                        Demical_Convert(data_read[2]));
  }
}
