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
 * @file      driver_sts4x.c
 * @brief     driver sts4x source file
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

#include "driver_sts4x.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion STS4X"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.08f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                     /**< chip max supply voltage */
#define MAX_CURRENT               0.5f                     /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                   /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip command definition
 */
#define STS4X_COMMAND_SOFT_RESET            0x94        /**< soft reset command */
#define STS4X_COMMAND_READ_SERIAL_NUMBER    0x89        /**< read serial number command */

/**
 * @brief      write and read bytes
 * @param[in]  *handle points to an sts4x handle structure
 * @param[in]  cmd is the send command
 * @param[in]  delay is the delay in ms
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
static uint8_t a_sts4x_write_read(sts4x_handle_t *handle, uint8_t cmd, uint16_t delay, uint8_t *data, uint16_t len)
{
    if (handle->iic_write_cmd(handle->iic_addr, &cmd, 1) != 0)             /* write command */
    {
        return 1;                                                          /* return error */
    }
    if (delay != 0)                                                        /* if not 0 */
    {
        handle->delay_ms(delay);                                           /* delay */
    }
    if (len != 0)                                                          /* check length */
    {
        if (handle->iic_read_cmd(handle->iic_addr, data, len) != 0)        /* read data */
        {
            return 1;                                                      /* return error */
        }
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     calculate the crc
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_sts4x_crc(uint8_t *data, uint16_t len)
{
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;
    uint16_t i;
    uint16_t j;
  
    for (j = len; j != 0; --j)                                              /* length-- */
    {
        crc ^= *data++;                                                     /* xor */
        for (i = 8; i != 0; --i)                                            /* 8 times */
        {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc<<1);        /* calculate crc */
        }
    }
  
    return crc;                                                             /* return crc */
}

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
uint8_t sts4x_init(sts4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("sts4x: iic_init is null.\n");                   /* iic_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("sts4x: iic_deinit is null.\n");                 /* iic_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                        /* check iic_read_cmd */
    {
        handle->debug_print("sts4x: iic_read_cmd is null.\n");               /* iic_read_cmd is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                       /* check iic_write_cmd */
    {
        handle->debug_print("sts4x: iic_write_cmd is null.\n");              /* iic_write_cmd is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("sts4x: delay_ms is null.\n");                   /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("sts4x: iic init failed.\n");                    /* iic init failed */
       
        return 1;                                                            /* return error */
    }
    res = a_sts4x_write_read(handle, STS4X_COMMAND_SOFT_RESET, 
                             10, NULL, 0);                                   /* soft reset */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("sts4x: write command failed.\n");               /* write command failed */
        (void)handle->iic_deinit();                                          /* close iic */
        
        return 4;                                                            /* return error */
    }
    handle->inited = 1;                                                      /* flag finish initialization */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t sts4x_deinit(sts4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    res = a_sts4x_write_read(handle, STS4X_COMMAND_SOFT_RESET, 
                             10, NULL, 0);                            /* soft reset */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts4x: write command failed.\n");        /* write command failed */
        
        return 4;                                                     /* return error */
    }
    if (handle->iic_deinit() != 0)                                    /* iic deinit */
    {
        handle->debug_print("sts4x: iic deinit failed.\n");           /* iic deinit failed */
       
        return 1;                                                     /* return error */
    }
    handle->inited = 0;                                               /* flag close */
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t sts4x_set_addr(sts4x_handle_t *handle, sts4x_address_t addr)
{
    if (handle == NULL)                      /* check handle */
    {
        return 2;                            /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr;        /* set address */
    
    return 0;                                /* success return 0 */
}

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
uint8_t sts4x_get_addr(sts4x_handle_t *handle, sts4x_address_t *addr)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    
    *addr = (sts4x_address_t)(handle->iic_addr);        /* get address */
    
    return 0;                                           /* success return 0 */
}

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
uint8_t sts4x_read(sts4x_handle_t *handle, sts4x_mode_t mode, uint16_t *temperature_raw, float *temperature_s)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    switch (mode)                                                                  /* select mode */
    {
        case STS4X_MODE_HIGH_PRECISION :                                           /* measure with high precision */
        {
            res = a_sts4x_write_read(handle, STS4X_MODE_HIGH_PRECISION, 
                                     10, buf, 3);                                  /* read data */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("sts4x: write command failed.\n");             /* write command failed */
                
                return 1;                                                          /* return error */
            }
            
            break;                                                                 /* break */
        }
        case STS4X_MODE_MEDIUM_PRECISION :                                         /* measure with medium precision */
        {
            res = a_sts4x_write_read(handle, STS4X_MODE_MEDIUM_PRECISION, 
                                     5, buf, 3);                                   /* read data */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("sts4x: write command failed.\n");             /* write command failed */
                
                return 1;                                                          /* return error */
            }
            
            break;                                                                 /* break */
        }
        case STS4X_MODE_LOWEST_PRECISION :                                         /* measure with the lowest precision */
        {
            res = a_sts4x_write_read(handle, STS4X_MODE_LOWEST_PRECISION, 
                                     2, buf, 3);                                   /* read data */
            if (res != 0)                                                          /* check result */
            {
                handle->debug_print("sts4x: write command failed.\n");             /* write command failed */
                
                return 1;                                                          /* return error */
            }
            
            break;                                                                 /* break */
        }
        default :                                                                  /* invalid */
        {
            memset(buf, 0, sizeof(uint8_t) * 3);                                   /* clear buffer */
            
            break;                                                                 /* break */
        }
    }
    if (a_sts4x_crc(buf + 0, 2) != buf[2])                                         /* check crc */
    {
        handle->debug_print("sts4x: crc is error.\n");                             /* crc is error */
        
        return 4;                                                                  /* return error */
    }
    *temperature_raw = (uint16_t)((((uint16_t)buf[0]) << 8) | buf[1]);             /* get raw temperature */
    *temperature_s = (float)(*temperature_raw) / 65535.0f * 175.0f - 45.0f;        /* convert raw temperature */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t sts4x_get_serial_number(sts4x_handle_t *handle, uint8_t num[4])
{
    uint8_t res;
    uint8_t buf[6];
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_sts4x_write_read(handle, STS4X_COMMAND_READ_SERIAL_NUMBER, 
                             10, buf, 6);                                     /* read serial number */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("sts4x: write command failed.\n");                /* write command failed */
        
        return 1;                                                             /* return error */
    }
    
    if (a_sts4x_crc(buf + 0, 2) != buf[2])                                    /* check crc */
    {
        handle->debug_print("sts4x: crc is error.\n");                        /* crc is error */
        
        return 4;                                                             /* return error */
    }
    if (a_sts4x_crc(buf + 3, 2) != buf[5])                                    /* check crc */
    {
        handle->debug_print("sts4x: crc is error.\n");                        /* crc is error */
        
        return 4;                                                             /* return error */
    }
    num[0] = buf[0];                                                          /* set number 0 */
    num[1] = buf[1];                                                          /* set number 1 */
    num[2] = buf[3];                                                          /* set number 2 */
    num[3] = buf[4];                                                          /* set number 3 */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t sts4x_soft_reset(sts4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    res = a_sts4x_write_read(handle, STS4X_COMMAND_SOFT_RESET, 
                             10, NULL, 0);                            /* soft reset */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts4x: write command failed.\n");        /* write command failed */
        
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t sts4x_write_read(sts4x_handle_t *handle, uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    return a_sts4x_write_read(handle, cmd, delay_ms, data, len);       /* write and read */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an sts4x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sts4x_info(sts4x_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(sts4x_info_t));                          /* initialize sts4x info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
