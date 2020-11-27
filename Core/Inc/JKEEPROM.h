/*
 * JKEEPROM.h
 *
 *  Created on: Oct 26, 2020
 *      Author: KlokockaJir
 */

#ifndef INC_JKEEPROM_H_
#define INC_JKEEPROM_H_

#include "main.h"

#define EEPROM_MAXITEMS 10

class JKEEPROM
{
	public:
		JKEEPROM();

		struct EEPROM_ITEM {
			uint8_t itemIndex;
			char* text;
			uint32_t* variable;
		};

		EEPROM_ITEM EEPROM_Items[EEPROM_MAXITEMS];

		void addItem(char* txt, uint8_t eepromPosition, uint32_t* variable);
		uint32_t GetItemValue(uint8_t eepromPosition);
		char* GetItemText(uint8_t eepromPosition);
		void UpdateItemVariableFromEEPROM(uint8_t eepromPosition);
		void SaveItemVariableToEEPROM(uint8_t eepromPosition);

		uint8_t itemsCount;
	private:

		uint8_t currentItemIndex;
		uint16_t startAddress;
};



#endif /* INC_JKEEPROM_H_ */
