# MX25V8035 STM32 HAL Driver

C driver library for the MX25V8035 serial NOR flash memory device.  
Designed for STM32 microcontrollers using the STM32 HAL library and SPI interface.

---

## **Features**

- Verify manufacturer and device ID  
- Read status register  
- Enable and disable write operations  
- Erase entire chip  
- Program up to 256 bytes per page  
- Read data from memory  
- SPI communication with manual chip select control  
- Basic error handling  

---

## **Requirements**

- STM32 MCU with STM32 HAL library  
- SPI hardware interface connected to MX25V8035 flash  
- Basic experience with embedded C and STM32CubeIDE or similar  

---

**Note:**  
This driver includes `#include "stm32h5xx_hal.h"` in the header files and is configured for the STM32H5 series.  
If you intend to reuse the code for other STM32 families, please adjust the HAL include and any hardware-specific settings accordingly.

---

## **Quick Start**

1. **Add driver files to your project**

Include `MX25V8035.c` and `MX25V8035.h` in your STM32 project source folder.

2. **Configure your hardware**

Set up SPI peripheral and GPIO pins for SPI and Chip Select (CS) according to your board schematic.

3. **Initialize driver handle structure**

```c
MX25_HW_InitTypeDef MX25_Handle = {
    .SPIhandler   = &hspi2,            // Your SPI handle configured in your project
    .CS_GPIOport  = GPIOB,             // GPIO port of CS pin
    .CS_GPIOpin   = GPIO_PIN_12,       // GPIO pin of CS
    .SPI_Timeout  = 100                // SPI timeout in milliseconds
};
```

4. **Verify memory connection**

```c
if (MX25_ID_Verify(&MX25_Handle) != MX25_NO_ERROR) {
    // Handle error: memory not detected
}
```

5. **Write data to memory**

```c
uint8_t data_to_write[256] = { /* your data */ };
uint32_t addr = 0x000000;

MX25_Page_Program(&MX25_Handle, &addr, data_to_write, 256);
```

6. **Read back data**

```c
uint8_t data_read[256];
MX25_Read_Data(&MX25_Handle, 0x000000, data_read, 256);
```

---

## **API Overview**

- `MX25_ID_Verify()` — Verify manufacturer and device ID  
- `MX25_Status()` — Read status register  
- `MX25_Write_Enable()` — Enable write operations  
- `MX25_Write_Disable()` — Disable write operations  
- `MX25_Chip_Erase()` — Erase the entire flash chip  
- `MX25_Page_Program()` — Write data to flash (max 256 bytes)  
- `MX25_Read_Data()` — Read data from flash memory  
- `enableCS_MX25()` / `disableCS_MX25()` — Manually control chip select  

---

## **License**

This project is licensed under the MIT License. See the LICENSE file for details.

---

## **Author**

Nathan Netzel  
Electrical Engineering Student — Londrina State University
