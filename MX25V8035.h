/*
 * MX25V8035.h
 *
 * Created on: Feb 03, 2025
 * Author: Nathan Netzel
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Nathan Netzel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the conditions of the MIT License.
 */

#ifndef INC_MX25V8035_H_
#define INC_MX25V8035_H_

#include "stm32h5xx_hal.h"

// --- Commands ---
#define MX25_REMS			0x90
#define MX25_RDSR			0x05
#define MX25_WREN			0x06
#define MX25_WRDI			0x04
#define MX25_CHIP_ERASE		0x60
#define MX25_PAGE_PROGRAM	0x02
#define MX25_READ_BYTES		0x03

// --- Status Register Bits ---
#define MX25_STATUS_WIP			0x01
#define MX25_STATUS_WEL			0x02

// --- Device ID ---
#define MX25_Device_ID			0x14
#define MX25_Manufacturer_ID	0xC2

// --- SPI Timeout (ms) ---
#define MX25_Timeout	10

// --- Error Status Enum ---
typedef enum {
    MX25_NO_ERROR            = 0x00U, /**< No error */
    MX25_ERROR               = 0x01U, /**< Generic error */
    MX25_HAL_ERROR           = 0x02U, /**< HAL communication error */
    MX25_INV_REG_DATA_READ   = 0x03U, /**< Invalid data read from register */
    MX25_ID_VERIFY_ERROR     = 0x04U  /**< Device ID verification failed */
} MX25_Error_TypeDef;

// --- Hardware Initialization Structure ---
typedef struct {
    SPI_HandleTypeDef *SPIhandler; /**< Pointer to SPI handler */
    GPIO_TypeDef *CS_GPIOport;     /**< GPIO port for chip select */
    uint16_t CS_GPIOpin;           /**< GPIO pin for chip select */
    uint8_t SPI_Timeout;           /**< SPI timeout in milliseconds */
} MX25_HW_InitTypeDef;

// --- Internal State (optional) ---
typedef enum {
    MX25_STATE_BUSY,
    MX25_STATE_IDLE
} MX25StateTypeDef;

// --- Function Prototypes ---

/**
 * @brief  Read the MX25 ID register and verify its value
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_ID_Verify(MX25_HW_InitTypeDef *);

/**
 * @brief  Read MX25 status register
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @param  MX25_Status Pointer to the variable where status will be stored
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Status(MX25_HW_InitTypeDef *, uint8_t *);

/**
 * @brief  Enable write operations on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Write_Enable(MX25_HW_InitTypeDef *);

/**
 * @brief  Disable write operations on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Write_Disable(MX25_HW_InitTypeDef *);

/**
 * @brief  Perform a full chip erase operation on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Chip_Erase(MX25_HW_InitTypeDef *);

/**
 * @brief  Program a page of up to 256 bytes on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @param  MX25_Adress Pointer to target address (will be incremented)
 * @param  MX25_Data Pointer to data buffer
 * @param  Data_Size Size in bytes (must be <= 256)
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Page_Program(MX25_HW_InitTypeDef *, uint32_t *, uint8_t *, uint32_t);

/**
 * @brief  Read data from memory starting at specified address
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @param  MX25_Adress Address to read from
 * @param  MX25_Data Pointer to destination buffer
 * @param  MX25_Number_Bytes Number of bytes to read
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Read_Data(MX25_HW_InitTypeDef *, uint32_t , uint8_t *, uint32_t);

/**
 * @brief  Enable the chip select (CS) pin for SPI communication
 * @param  CS_GPIOport GPIO port of the CS pin
 * @param  CS_GPIOpin GPIO pin number of the CS pin
 */
void enableCS_MX25(GPIO_TypeDef *, uint16_t);

/**
 * @brief  Disable the chip select (CS) pin for SPI communication
 * @param  CS_GPIOport GPIO port of the CS pin
 * @param  CS_GPIOpin GPIO pin number of the CS pin
 */
void disableCS_MX25(GPIO_TypeDef *, uint16_t);

#endif /* INC_MX25V8035_H_ */

