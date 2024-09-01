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
 * @file      driver_sts4x_basic.c
 * @brief     driver sts4x basic source file
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

#include "driver_sts4x_basic.h"

static sts4x_handle_t gs_handle;        /**< sts4x handle */

/**
 * @brief     basic example init
 * @param[in] addr is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t sts4x_basic_init(sts4x_address_t addr)
{
    uint8_t res;
    
    /* link functions */
    DRIVER_STS4X_LINK_INIT(&gs_handle, sts4x_handle_t);
    DRIVER_STS4X_LINK_IIC_INIT(&gs_handle, sts4x_interface_iic_init);
    DRIVER_STS4X_LINK_IIC_DEINIT(&gs_handle, sts4x_interface_iic_deinit);
    DRIVER_STS4X_LINK_IIC_READ_COMMAND(&gs_handle, sts4x_interface_iic_read_cmd);
    DRIVER_STS4X_LINK_IIC_WRITE_COMMAND(&gs_handle, sts4x_interface_iic_write_cmd);
    DRIVER_STS4X_LINK_DELAY_MS(&gs_handle, sts4x_interface_delay_ms);
    DRIVER_STS4X_LINK_DEBUG_PRINT(&gs_handle, sts4x_interface_debug_print);

    /* set addr */
    res = sts4x_set_addr(&gs_handle, addr);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: set addr pin failed.\n");
        
        return 1;
    }
    
    /* sts4x init */
    res = sts4x_init(&gs_handle);
    if (res != 0)
    {
        sts4x_interface_debug_print("sts4x: init failed.\n");
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *temperature points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sts4x_basic_read(float *temperature)
{
    uint16_t temperature_raw;
   
    /* read data */
    if (sts4x_read(&gs_handle, STS4X_BASIC_DEFAULT_MODE,
                  (uint16_t *)&temperature_raw, temperature) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sts4x_basic_deinit(void)
{
    /* close sts4x */
    if (sts4x_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get serial number
 * @param[out] *num points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t sts4x_basic_get_serial_number(uint8_t num[4])
{
    /* get serial number */
    if (sts4x_get_serial_number(&gs_handle, num) != 0)
    {
        return 1;
    }
    
    return 0;
}