/*
 * JKEEPROM.cpp
 *
 *  Created on: Oct 26, 2020
 *      Author: KlokockaJir
 */

#include "JKEEPROM.h"
#include "eeprom.h"
#include <stdio.h>
#include <string.h>


JKEEPROM::JKEEPROM()
{
	itemsCount = 0;
	currentItemIndex = 0;
	startAddress = 0;
}

//txt - text description af the item
//eepromPosition - position of variable in eeprom
//value - pointer to variable
void JKEEPROM::addItem(char* txt, uint8_t eepromPosition, uint32_t* variable)
{
	uint8_t pos = eepromPosition;
	EEPROM_ITEM &itm = EEPROM_Items[pos];
	itm.itemIndex = eepromPosition;
	itm.text = txt;
	itm.variable = variable;

	uint32_t tempValue = 0;
	EE_Read(startAddress + (uint32_t)itm.itemIndex, &tempValue);

	if((int32_t)tempValue == -1) {
		//=>if it is first use of EEPROM position - saves variable value to eeprom
		EE_Write(startAddress + (uint32_t)itm.itemIndex, *variable);
	} else {
		//=>else load variable value from EEPROM position
		EE_Read(startAddress + (uint32_t)itm.itemIndex, variable);
	}
	itemsCount++;
}

void JKEEPROM::UpdateItemVariableFromEEPROM(uint8_t eepromPosition) {
	EE_Read(startAddress + (uint32_t)EEPROM_Items[eepromPosition].itemIndex, EEPROM_Items[eepromPosition].variable);
}

void JKEEPROM::SaveItemVariableToEEPROM(uint8_t eepromPosition) {
	EE_Write(startAddress + (uint32_t)EEPROM_Items[eepromPosition].itemIndex, *EEPROM_Items[eepromPosition].variable);
}

//returns value of the items' variable at eepromPosition
uint32_t JKEEPROM::GetItemValue(uint8_t eepromPosition)
{
	uint32_t tempValue = 0;
	EE_Read(startAddress + eepromPosition, &tempValue);
	return tempValue;
}

//returns text of the items' variable at eepromPosition
char* JKEEPROM::GetItemText(uint8_t eepromPosition)
{
	return EEPROM_Items[eepromPosition].text;
}

