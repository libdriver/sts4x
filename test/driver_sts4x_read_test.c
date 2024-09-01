/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sts4x_read_test.c
 * @brief     driver sts4x read test source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-09-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/09/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_sts4x_read_test.h"

static sts4x_handle_t gs_handle;        /**< sts4x handle */

/**
 * @brief     read test
 * @param[in] addr is the iic device address
 * @param[in] times is test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sts4x_read_test(sts4x_address_t addr, uint32_t times)
{
    uint8_t res;
    uint8_t num[4];
    uint32_t i;
    sts4x_info_t info;
    
    /* link functions */
    DRIVER_STS4X_LINK_INIT(&gs_handle, sts4x_handle_t);
    DRIVER_STS4X_LINK_IIC_INIT(&gs_handle, sts4x_interface_iic_init);
    DRIVER_STS4X_LINK_IIC_DEINIT(&gs_handle, sts4x_interface_iic_deinit);
    DRIVER_STS4X_LINK_IIC_READ_COMMAND(&gs_handle, sts4x_interface_iic_read_cmd);
    DRIVER_STS4X_LINK_IIC_WRITE_COMMAND(&gs_handle, sts4x_interface_iic_write_cmd);
    DRIVER_STS4X_LINK_DELAY_MS(&gs_handle, sts4x_interface_delay_ms);
    DRIVER_STS4X_LINK_DEBUG_PRINT(&gs_handle, sts4x_interface_debug_print);
    
    /* sts4x info */
    res = sts4x_info(&info);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        sts4x_interface_debug_print("sts4x: chip is %s.\n", info.chip_name);
        sts4x_interface_debug_print("sts4x: manufacturer is %s.\n", info.manufacturer_name);
        sts4x_interface_debug_print("sts4x: interface is %s.\n", info.interface);
        sts4x_interface_debug_print("sts4x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sts4x_interface_debug_print("sts4x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sts4x_interface_debug_print("sts4x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sts4x_interface_debug_print("sts4x: max current is %0.2fmA.\n", info.max_current_ma);
        sts4x_interface_debug_print("sts4x: max temperature is %0.1fC.\n", info.temperature_max);
        sts4x_interface_debug_print("sts4x: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    sts4x_interface_debug_print("sts4x: start read test.\n");
    
    /* set address */
    res = sts4x_set_addr(&gs_handle, addr);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: set addr failed.\n");
       
        return 1;
    }
    
    /* sts4x init */
    res = sts4x_init(&gs_handle);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: init failed.\n");
       
        return 1;
    }
    
    /* get serial number */
    res = sts4x_get_serial_number(&gs_handle, num);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: get serial number failed.\n");
        (void)sts4x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    sts4x_interface_debug_print("sts4x: serial number is 0x%02X 0x%02X 0x%02X 0x%02X.\n", num[0], num[1], num[2], num[3]);
    
    /* wait 1000 ms */
    sts4x_interface_delay_ms(1000);
    
    /* output */
    sts4x_interface_debug_print("sts4x: high precision.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        /* read data */
        res = sts4x_read(&gs_handle, STS4X_MODE_HIGH_PRECISION,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts4x_interface_debug_print("sts4x: read failed.\n");
            (void)sts4x_deinit(&gs_handle);
            
            return 1;
        }
        sts4x_interface_debug_print("sts4x: temperature is %0.2fC.\n", temperature_s);
    
        /* wait 1000 ms */
        sts4x_interface_delay_ms(1000);
    }
    
    /* wait 1000 ms */
    sts4x_interface_delay_ms(1000);
    
    /* output */
    sts4x_interface_debug_print("sts4x: medium precision.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        /* read data */
        res = sts4x_read(&gs_handle, STS4X_MODE_MEDIUM_PRECISION,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts4x_interface_debug_print("sts4x: read failed.\n");
            (void)sts4x_deinit(&gs_handle);
            
            return 1;
        }
        sts4x_interface_debug_print("sts4x: temperature is %0.2fC.\n", temperature_s);
    
        /* wait 1000 ms */
        sts4x_interface_delay_ms(1000);
    }
    
    /* wait 1000 ms */
    sts4x_interface_delay_ms(1000);
    
    /* output */
    sts4x_interface_debug_print("sts4x: lowest precision.\n");
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;

        /* read data */
        res = sts4x_read(&gs_handle, STS4X_MODE_LOWEST_PRECISION,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts4x_interface_debug_print("sts4x: read failed.\n");
            (void)sts4x_deinit(&gs_handle);
            
            return 1;
        }
        sts4x_interface_debug_print("sts4x: temperature is %0.2fC.\n", temperature_s);
    
        /* wait 1000 ms */
        sts4x_interface_delay_ms(1000);
    }
    
    /* output */
    sts4x_interface_debug_print("sts4x: soft reset.\n");
    
    /* soft reset */
    res = sts4x_soft_reset(&gs_handle);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: soft reset failed.\n");
        (void)sts4x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish read test */
    sts4x_interface_debug_print("sts4x: finish read test.\n");
    (void)sts4x_deinit(&gs_handle);

    return 0;
}
