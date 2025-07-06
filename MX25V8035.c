/*
 * MX25V8035.c
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

#include <MX25V8035.h>

/**
 * @brief  Read the MX25 ID register and verify its value
 * @note   Compares returned ID with expected manufacturer and device IDs
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_ID_Verify(MX25_HW_InitTypeDef *MX25_Handler){

	uint8_t SPITransmitData_MX25[4];
	uint8_t MX25_REMS_Reg_Return[2];

	SPITransmitData_MX25[0] = MX25_REMS;
	SPITransmitData_MX25[3] = 0x00;

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, SPITransmitData_MX25, 4, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	if(HAL_SPI_Receive(MX25_Handler->SPIhandler, MX25_REMS_Reg_Return, 2, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);

	if ( (MX25_REMS_Reg_Return[0] == MX25_Manufacturer_ID) && (MX25_REMS_Reg_Return[1] == MX25_Device_ID) )
			return MX25_NO_ERROR;
		else
			return MX25_ID_VERIFY_ERROR;
}

/**
 * @brief  Read MX25 status register
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @param  MX25_Status Pointer to the variable where status will be stored
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Status(MX25_HW_InitTypeDef *MX25_Handler, uint8_t *MX25_Status)
{
	uint8_t SPITransmitData_MX25 = MX25_RDSR;

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, &SPITransmitData_MX25, 1, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	if(HAL_SPI_Receive(MX25_Handler->SPIhandler, MX25_Status, 1, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}

	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	return MX25_NO_ERROR;
}

/**
 * @brief  Enable write operations on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Write_Enable(MX25_HW_InitTypeDef *MX25_Handler)
{
	uint8_t MX25_Status_Reg_Return = 0;
	uint8_t SPITransmitData_MX25 = MX25_WREN;

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, &SPITransmitData_MX25, 1, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);

	MX25_Status(MX25_Handler, &MX25_Status_Reg_Return);
	if((MX25_Status_Reg_Return & MX25_STATUS_WEL)!= MX25_STATUS_WEL)
		return MX25_INV_REG_DATA_READ;
	else
		return MX25_NO_ERROR;
}

/**
 * @brief  Disable write operations on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Write_Disable(MX25_HW_InitTypeDef *MX25_Handler)
{
	uint8_t MX25_Status_Reg_Return = 0;
	uint8_t SPITransmitData_MX25 = MX25_WRDI;

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, &SPITransmitData_MX25, 1, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);

	MX25_Status(MX25_Handler, &MX25_Status_Reg_Return);
	if((MX25_Status_Reg_Return & MX25_STATUS_WEL)!= 0)
		return MX25_INV_REG_DATA_READ;
	else
		return MX25_NO_ERROR;
}


/**
 * @brief  Perform a full chip erase operation on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Chip_Erase(MX25_HW_InitTypeDef *MX25_Handler)
{

	if(MX25_Write_Enable(MX25_Handler) != MX25_NO_ERROR)
		return MX25_HAL_ERROR;

	uint8_t SPITransmitData_MX25 = MX25_CHIP_ERASE;

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, &SPITransmitData_MX25, 1, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);

	return MX25_NO_ERROR;
}


/**
 * @brief  Program a page of up to 256 bytes on MX25
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @param  MX25_Adress Pointer to target address (will be incremented)
 * @param  MX25_Data Pointer to data buffer
 * @param  Data_Size Size in bytes (must be <= 256)
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Page_Program(MX25_HW_InitTypeDef *MX25_Handler, uint32_t *MX25_Adress, uint8_t *MX25_Data, uint32_t Data_Size)
{

	if(Data_Size > 256) return MX25_HAL_ERROR; 

	if(Data_Size == 256 && (((*MX25_Adress)& 0xFF) != 0x00)) return MX25_HAL_ERROR; 

	if(MX25_Write_Enable(MX25_Handler) != MX25_NO_ERROR)
		return MX25_HAL_ERROR;

	uint8_t SPITransmitData_MX25[4];
	SPITransmitData_MX25[0] = MX25_PAGE_PROGRAM;
	SPITransmitData_MX25[1] = (((*MX25_Adress) >> 16) & 0xFF);
	SPITransmitData_MX25[2] = (((*MX25_Adress) >> 8) & 0xFF);
	SPITransmitData_MX25[3] = ((*MX25_Adress)& 0xFF);

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, SPITransmitData_MX25, 4, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, MX25_Data, Data_Size, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);

	*MX25_Adress += Data_Size;

	return MX25_NO_ERROR;
}

/**
 * @brief  Read data from memory starting at specified address
 * @param  MX25_Handler Pointer to the MX25_HW_InitTypeDef structure
 * @param  MX25_Adress Address to read from
 * @param  MX25_Data Pointer to destination buffer
 * @param  MX25_Number_Bytes Number of bytes to read
 * @retval MX25_Error_TypeDef
 */
MX25_Error_TypeDef MX25_Read_Data(MX25_HW_InitTypeDef *MX25_Handler, uint32_t MX25_Adress, uint8_t *MX25_Data, uint32_t MX25_Number_Bytes)
{

	if(MX25_Write_Enable(MX25_Handler) != MX25_NO_ERROR)
		return MX25_HAL_ERROR;

	uint8_t SPITransmitData_MX25[4];
	SPITransmitData_MX25[0] = MX25_READ_BYTES;
	SPITransmitData_MX25[1] = (((MX25_Adress) >> 16) & 0xFF);
	SPITransmitData_MX25[2] = (((MX25_Adress) >> 8) & 0xFF);
	SPITransmitData_MX25[3] = ((MX25_Adress)& 0xFF);

	enableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
	if(HAL_SPI_Transmit(MX25_Handler->SPIhandler, SPITransmitData_MX25, 4, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	if(HAL_SPI_Receive(MX25_Handler->SPIhandler, MX25_Data, MX25_Number_Bytes, MX25_Handler->SPI_Timeout) != HAL_OK){
		disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);
		return MX25_HAL_ERROR;
	}
	disableCS_MX25(MX25_Handler->CS_GPIOport, MX25_Handler->CS_GPIOpin);

	return MX25_NO_ERROR;
}

/**
 * @brief  Enables chip select (active low)
 * @param  CS_GPIOport GPIO port of chip select pin
 * @param  CS_GPIOpin GPIO pin number
 * @retval None
 */
void enableCS_MX25(GPIO_TypeDef *CS_GPIOport, uint16_t CS_GPIOpin){
  HAL_GPIO_WritePin(CS_GPIOport, CS_GPIOpin, GPIO_PIN_RESET);
}

/**
 * @brief  Disables chip select (active low)
 * @param  CS_GPIOport GPIO port of chip select pin
 * @param  CS_GPIOpin GPIO pin number
 * @retval None
 */
void disableCS_MX25(GPIO_TypeDef *CS_GPIOport, uint16_t CS_GPIOpin){
  HAL_GPIO_WritePin(CS_GPIOport, CS_GPIOpin, GPIO_PIN_SET);
}

