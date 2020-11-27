#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdbool.h>
//include the Correct HAL version here according to the Cortex core.
//#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

//################################################################################################################
bool	EE_FormatPage(uint32_t PageAddress);
bool 	EE_Read(uint16_t VirtualAddress, uint32_t* Data);
bool	EE_Reads(uint16_t StartVirtualAddress,uint16_t HowMuchToRead,uint32_t* Data);
bool    EE_ReadBytes(uint16_t StartVirtualAddress,uint16_t HowManyBytesToRead,void* Data);
bool 	EE_Write(uint16_t VirtualAddress, uint32_t Data);
bool 	EE_Writes(uint16_t StartVirtualAddress,uint16_t HowMuchToWrite,uint32_t* Data);
bool 	EE_WriteBytes(uint16_t StartVirtualAddress,uint16_t HowManyBytesToWrite,void* Data);
//################################################################################################################

#ifdef __cplusplus
}
#endif

#endif
