#include "bmp280.h"


static BMP280_Status_Typedef BMP280_SendCmd(BMP280_Handler_t *Instance, uint8_t *cmd, uint16_t len, uint16_t Timeout)
{
    if(Instance->hi2c == NULL || cmd == NULL)
    {
        return BMP280_ERR; 
    }

    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(Instance->hi2c, Instance->Address, cmd, len, Timeout);
    if(status != HAL_OK)
    {
        return BMP280_I2C_ERR;
    }
    
    return BMP280_OK;
}

static BMP280_Status_Typedef BMP280_ReceivedData(BMP280_Handler_t *Instance, uint8_t *data, uint16_t len, uint16_t Timeout)
{
    if(Instance->hi2c == NULL || data == NULL)
    {
        return BMP280_ERR; 
    }
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Receive(Instance->hi2c, Instance->Address, data, len, Timeout);
    if(status != HAL_OK)
    {
        return BMP280_I2C_ERR;
    }

    return BMP280_OK;

}


static BMP280_Status_Typedef BMP280_ReadRegister(BMP280_Handler_t *Instance,uint8_t reg_addr, uint8_t *data, uint16_t len, uint16_t Timeout)
{
    if(Instance->hi2c == NULL || data == NULL)
    {
        return BMP280_ERR; 
    }

    BMP280_Status_Typedef status;
    status = BMP280_SendCmd(Instance, &reg_addr, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }

    status = BMP280_ReceivedData(Instance, data, len, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }

    return BMP280_OK;
}

static BMP280_Status_Typedef BMP280_CheckID(BMP280_Handler_t *Instance, uint16_t Timeout)
{
    if(Instance->hi2c == NULL)
    {
        return BMP280_ERR; 
    }
    BMP280_Status_Typedef status;
    uint8_t id_result;
    status = BMP280_ReadRegister(Instance, BMP280_ID_REG, &id_result, 1, Timeout);
    if(status != BMP280_OK )
    {
        return status;
    }

    if(id_result != 0x60)
    {
        return BMP280_INVALID_ID;
    }

    return BMP280_OK;
}

BMP280_Status_Typedef BMP280_WaitMeasuring(BMP280_Handler_t *Instance, uint16_t Timeout)
{
    BMP280_Status_Typedef status; 
    uint8_t measuring_flag = 1;
    uint32_t tickstart = HAL_GetTick();
    while((measuring_flag & (1U << 3)))
    {
        status = BMP280_ReadRegister(Instance, BMP280_STATUS_REG, &measuring_flag, 1, Timeout);
        if(status != BMP280_OK)
        {
            return status;
        }

        if((HAL_GetTick() - tickstart) > Timeout)
        {
            return BMP280_TIMEOUT;
        }
    }

    return BMP280_OK;
}

static BMP280_Status_Typedef BMP280_ReadOffSet(BMP280_Handler_t *Instance, BMP280_Calib_Config_t *ptr, uint16_t Timeout)
{
    if(Instance == NULL)
    {
        return BMP280_ERR;
    }
    BMP280_Status_Typedef status;
    uint8_t calib_arr[24];
    status = BMP280_ReadRegister(Instance, BMP280_DIG_T1_REG, calib_arr, sizeof(calib_arr), Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }
    ptr->dig_T1 = calib_arr[0] | (calib_arr[1] << 8);
    ptr->dig_T2 = calib_arr[2] | (calib_arr[3] << 8);
    ptr->dig_T3 = calib_arr[4] | (calib_arr[5] << 8);
    ptr->dig_P1 = calib_arr[6] | (calib_arr[7] << 8);
    ptr->dig_P2 = calib_arr[8] | (calib_arr[9] << 8);
    ptr->dig_P3 = calib_arr[10] | (calib_arr[11] << 8);
    ptr->dig_P4 = calib_arr[12] | (calib_arr[13] << 8);
    ptr->dig_P5 = calib_arr[14] | (calib_arr[15] << 8);
    ptr->dig_P6 = calib_arr[16] | (calib_arr[17] << 8);
    ptr->dig_P7 = calib_arr[18] | (calib_arr[19] << 8);
    ptr->dig_P8 = calib_arr[20] | (calib_arr[21] << 8);
    ptr->dig_P9 = calib_arr[22] | (calib_arr[23] << 8);

    return BMP280_OK;
}


static BMP280_Status_Typedef BMP280_ReadRawData(BMP280_Handler_t * Instance, BMP280_Calib_Config_t *ptr, uint16_t Timeout)
{
   if(Instance == NULL)
    {
        return BMP280_ERR;
    }
    BMP280_Status_Typedef status;
    uint8_t press_msb = 0, press_lsb = 0, press_xlsb = 0;
    uint8_t temp_msb = 0, temp_lsb = 0, temp_xlsb = 0;

    status = BMP280_ReadRegister(Instance, BMP280_PRESS_MSB, &press_msb, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }

    status = BMP280_ReadRegister(Instance, BMP280_PRESS_LSB, &press_lsb, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }

    status = BMP280_ReadRegister(Instance, BMP280_PRESS_XLSB, &press_xlsb, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }

    status = BMP280_ReadRegister(Instance, BMP280_TEMP_MSB, &temp_msb, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }  
    
    status = BMP280_ReadRegister(Instance, BMP280_TEMP_LSB, &temp_lsb, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }

    status = BMP280_ReadRegister(Instance, BMP280_TEMP_XLSB, &temp_xlsb, 1, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }
    ptr->rawPressData = ((int32_t)press_msb << 12) | ((int32_t)press_lsb << 4) | ((int32_t)press_xlsb >> 4);
    ptr->rawTempData = ((int32_t)temp_msb << 12) | ((int32_t)temp_lsb << 4) | ((int32_t)temp_xlsb >> 4); 
    return BMP280_OK;
}


BMP280_Status_Typedef BMP280_ReadTemp(BMP280_Handler_t *Instance, int32_t *temp_result, uint16_t Timeout)
{
    if(Instance == NULL || temp_result == NULL)
    {
        return BMP280_ERR;
    }

    BMP280_Calib_Config_t calib_result;
    BMP280_Status_Typedef status; 
    status = BMP280_WaitMeasuring(Instance, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }
    BMP280_ReadOffSet(Instance, &calib_result, Timeout);
    BMP280_ReadRawData(Instance, &calib_result, Timeout);

    int32_t var1, var2, t_fine;
    var1 = ((((calib_result.rawTempData >> 3) - ((int32_t)calib_result.dig_T1 << 1))) * ((int32_t)calib_result.dig_T2)) >> 11;
    var2 = (((((calib_result.rawTempData >> 4) - ((int32_t)calib_result.dig_T1)) * ((calib_result.rawTempData >> 4) - ((int32_t)calib_result.dig_T1))) >> 12) * ((int32_t)calib_result.dig_T3)) >> 14;
    t_fine = var1 + var2;
    *temp_result = (t_fine * 5 + 128) >> 8;

    return BMP280_OK;
}

BMP280_Status_Typedef BMP280_ReadPressure(BMP280_Handler_t *Instance, uint32_t *press_result, uint16_t Timeout)
{
    if(Instance == NULL || press_result == NULL)
    {
        return BMP280_ERR;
    }
    BMP280_Calib_Config_t calib_result;
    BMP280_Status_Typedef status; 

    status = BMP280_WaitMeasuring(Instance, Timeout);
    if(status != BMP280_OK)
    {
        return status;
    }
    BMP280_ReadOffSet(Instance, &calib_result, Timeout);
    BMP280_ReadRawData(Instance, &calib_result, Timeout);

    int32_t var1_t, var2_t, t_fine;
    var1_t = ((((calib_result.rawTempData >> 3) - ((int32_t)calib_result.dig_T1 << 1))) * ((int32_t)calib_result.dig_T2)) >> 11;
    var2_t = (((((calib_result.rawTempData >> 4) - ((int32_t)calib_result.dig_T1)) * ((calib_result.rawTempData >> 4) - ((int32_t)calib_result.dig_T1))) >> 12) * ((int32_t)calib_result.dig_T3)) >> 14;
    t_fine = var1_t + var2_t;

    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_result.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_result.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_result.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_result.dig_P3) >> 8) + ((var1 * (int64_t)calib_result.dig_P2) << 12);
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t)calib_result.dig_P1) >> 33;
    if(var1 == 0)
    {
        return BMP280_READ_PRESSURE_ERR;
    }
    p = 1048576 - calib_result.rawPressData;
    p = ((((int64_t)p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_result.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_result.dig_P8) * (p)) >> 19;
    p = ((p + var1 + var2) >> 8) + ((int64_t)calib_result.dig_P7 << 4);
    *press_result = (uint32_t)p / 256;
    return BMP280_OK;
}

BMP280_Status_Typedef BMP280_Init(BMP280_Handler_t *Instance, uint16_t Timeout)
{
    BMP280_Status_Typedef init_status;
    init_status = BMP280_CheckID(Instance, Timeout);
    if(init_status != BMP280_OK)
    {
        return init_status;
    }
    /* User configure power mode */
    uint8_t ctrl_cmd[2] = {BMP280_CTRL_MEAS, (Instance->osrs_T << 5) | (Instance->osrs_P << 2) | (Instance->Mode)};
    
    init_status = BMP280_SendCmd(Instance, ctrl_cmd, 2, Timeout);
    if(init_status != BMP280_OK)
    {
        return init_status;
    }
    /* User configure normal standby time */
    uint8_t config_cmd[2] = {BMP280_CONFIG_REG, (Instance->t_standby << 5) | (Instance->FilterSetting << 2)};
    if(Instance->Mode == BMP280_NORMAL_MODE)
    {
        BMP280_SendCmd(Instance, config_cmd, 2, Timeout);
    }


    return BMP280_OK;
}
