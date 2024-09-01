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
 * @file      driver_sts4x.h
 * @brief     driver sts4x header file
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

#ifndef DRIVER_STS4X_H
#define DRIVER_STS4X_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sts4x_driver sts4x driver function
 * @brief    sts4x driver modules
 * @{
 */

/**
 * @addtogroup sts4x_base_driver
 * @{
 */

/**
 * @brief sts4x address enumeration definition
 */
typedef enum
{
    STS4X_ADDRESS_0 = (0x44 << 1),        /**< address0 */
    STS4X_ADDRESS_1 = (0x45 << 1),        /**< address1 */
    STS4X_ADDRESS_2 = (0x46 << 1),        /**< address2 */
} sts4x_address_t;

/**
 * @brief sts4x mode enumeration definition
 */
typedef enum
{
    STS4X_MODE_HIGH_PRECISION   = 0xFD,        /**< measure with high precision */
    STS4X_MODE_MEDIUM_PRECISION = 0xF6,        /**< measure with medium precision */
    STS4X_MODE_LOWEST_PRECISION = 0xE0,        /**< measure with lowest precision */
} sts4x_mode_t;

/**
 * @brief sts4x handle structure definition
 */
typedef struct sts4x_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t iic_addr;                                                          /**< iic device address */
    uint8_t inited;                                                            /**< inited flag */
} sts4x_handle_t;

/**
 * @brief sts4x information structure definition
 */
typedef struct sts4x_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} sts4x_info_t;

/**
 * @}
 */

/**
 * @defgroup sts4x_link_driver sts4x link driver function
 * @brief    sts4x link driver modules
 * @ingroup  sts4x_driver
 * @{
 */

/**
 * @brief     initialize sts4x_handle_t structure
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] STRUCTURE is sts4x_handle_t
 * @note      none
 */
#define DRIVER_STS4X_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_STS4X_LINK_IIC_INIT(HANDLE, FUC)              (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_STS4X_LINK_IIC_DEINIT(HANDLE, FUC)            (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] FUC points to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_STS4X_LINK_IIC_READ_COMMAND(HANDLE, FUC)      (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] FUC points to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_STS4X_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)     (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_STS4X_LINK_DELAY_MS(HANDLE, FUC)              (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an sts4x handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_STS4X_LINK_DEBUG_PRINT(HANDLE, FUC)           (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup sts4x_base_driver sts4x base driver function
 * @brief    sts4x base driver modules
 * @ingroup  sts4x_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an sts4x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sts4x_info(sts4x_info_t *info);

/**
 * @brief     set the iic address
 * @param[in] *handle points to an sts4x handle structure
 * @param[in] addr is the chip iic address
 * @return    status code
 *            - 0 success
 *            - 1 set addr failed
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t sts4x_set_addr(sts4x_handle_t *handle, sts4x_address_t addr);

/**
 * @brief      get the iic address
 * @param[in]  *handle points to an sts4x handle structure
 * @param[out] *addr points to a chip iic address buffer
 * @return      status code
 *              - 0 success
 *              - 1 get addr failed
 *              - 2 handle is NULL
 * @note        none
 */
uint8_t sts4x_get_addr(sts4x_handle_t *handle, sts4x_address_t *addr);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an sts4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t sts4x_init(sts4x_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an sts4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t sts4x_deinit(sts4x_handle_t *handle);

/**
 * @brief      read temperature and humidity
 * @param[in]  *handle points to an sts4x handle structure
 * @param[in]  mode is the read mode
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t sts4x_read(sts4x_handle_t *handle, sts4x_mode_t mode, uint16_t *temperature_raw, float *temperature_s);

/**
 * @brief     soft reset the chip
 * @param[in] *handle points to an sts4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts4x_soft_reset(sts4x_handle_t *handle);

/**
 * @brief      get serial number
 * @param[in]  *handle points to an sts4x handle structure
 * @param[out] *num points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t sts4x_get_serial_number(sts4x_handle_t *handle, uint8_t num[4]);

/**
 * @}
 */

/**
 * @defgroup sts4x_extend_driver sts4x extend driver function
 * @brief    sts4x extend driver modules
 * @ingroup  sts4x_driver
 * @{
 */

/**
 * @brief      write and read bytes
 * @param[in]  *handle points to an sts4x handle structure
 * @param[in]  cmd is the send command
 * @param[in]  delay_ms is the delay in ms
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t sts4x_write_read(sts4x_handle_t *handle, uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
