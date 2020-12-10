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


/*
 * Adds item to JKEEPROM object and:
 * if EEPROM has NOT been used - stores variable value to EEPROM
 * if EEPROM has been used - reads variable value from EEPROM
 *
 * txt - text description af the item
 * eepromPosition - position of variable in eeprom
 * value - pointer to variable
 */
void JKEEPROM::addItem(char* txt, uint8_t eepromPosition, uint32_t* variable)
{
	uint8_t pos = eepromPosition;
	EEPROM_ITEM &itm = EEPROM_Items[pos];
	itm.itemIndex = eepromPosition;
	itm.text = txt;
	itm.variable = variable;

	uint32_t tempValue = 0;
	EE_Read(startAddress + (uint32_t)itm.itemIndex, &tempValue);

	if(tempValue == 0xFFFFFFFF) {
		//=>if it is first use of EEPROM position - saves variable value to eeprom
		EE_Write(startAddress + (uint32_t)itm.itemIndex, *variable);
	} else {
		//=>else load variable value from EEPROM position
		EE_Read(startAddress + (uint32_t)itm.itemIndex, variable);
	}
	itemsCount++;
}


void JKEEPROM::addItemInternal(char* txt, uint8_t eepromPosition, uint32_t* variable)
{
	uint8_t pos = eepromPosition;
	EEPROM_ITEM &itm = EEPROM_Items[pos];
	itm.itemIndex = eepromPosition;
	itm.text = txt;
	itm.variable = variable;

	uint32_t tempValue = 0;
	EE_Read(startAddress + (uint32_t)itm.itemIndex, &tempValue);

	if(tempValue == 0xFFFFFFFF) {
		//=>if it is first use of EEPROM position - saves variable value to eeprom
		EE_Write(startAddress + (uint32_t)itm.itemIndex, *variable);
	} else {
		//=>else load variable value from EEPROM position
		EE_Read(startAddress + (uint32_t)itm.itemIndex, variable);
	}
	itemsCount++;
}


/* Reads variable value from EEPROM at eepromPosition
 *
 */
void JKEEPROM::UpdateItemVariableFromEEPROM(uint8_t eepromPosition) {
	EE_Read(startAddress + (uint32_t)EEPROM_Items[eepromPosition].itemIndex, EEPROM_Items[eepromPosition].variable);
}

/* stores variable value to EEPROM at eepromPosition
 *
 */
void JKEEPROM::SaveItemVariableToEEPROM(uint8_t eepromPosition) {
	EE_Write(startAddress + (uint32_t)EEPROM_Items[eepromPosition].itemIndex, *EEPROM_Items[eepromPosition].variable);
}

/* Reads all variables values from EEPROM
 *
 */
void JKEEPROM::UpdateAllItemsVariablesFromEEPROM() {
	for(uint8_t i = 0; i < itemsCount; i++) {
		EE_Read(startAddress + (uint32_t)EEPROM_Items[i].itemIndex, EEPROM_Items[i].variable);
	}
}

/* Stores all variables values to EEPROM
 *
 */
void JKEEPROM::SaveAllItemsVariablesToEEPROM() {
	for(uint8_t i = 0; i < itemsCount; i++) {
		EE_Write(startAddress + (uint32_t)EEPROM_Items[i].itemIndex, *EEPROM_Items[i].variable);
	}
}


uint8_t JKEEPROM::GetItemsCount() {
	return itemsCount;
}

/* Returns directly value at eepromPosition without updating value
 *
 */
uint32_t JKEEPROM::ReadItemValueFromEEPROM(uint8_t eepromPosition)
{
	uint32_t tempValue = 0;
	EE_Read(startAddress + eepromPosition, &tempValue);
	return tempValue;
}

/* Stores directly value at eepromPosition without updating variable
 *
 */
void JKEEPROM::SaveItemValueToEEPROM(uint8_t eepromPosition, uint32_t value)
{
	EE_Write(startAddress + (uint32_t)EEPROM_Items[eepromPosition].itemIndex, value);
}



//returns text of the items' variable at eepromPosition
char* JKEEPROM::GetItemText(uint8_t eepromPosition)
{
	return EEPROM_Items[eepromPosition].text;
}

