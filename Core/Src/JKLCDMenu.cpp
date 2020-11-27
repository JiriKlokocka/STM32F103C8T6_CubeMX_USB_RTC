/*
 * JKLCDMenu.cpp
 *
 *  Created on: Oct 19, 2020
 *      Author: KlokockaJir
 */

#include "JKLCDMenu.h"
#include <stdio.h>
#include <string.h>


JKLCDMenu::JKLCDMenu()
{
	itemCount = 0;
	currentItemIndex = 0;
	currentLevelID = 0;
	btnPressed = BTN_NONE;
	btnEdit = BTN_NONE;
}


void JKLCDMenu::CheckButtons(uint8_t btnNumber) {

	//ENTER
	if(btnNumber == USER_BUTTON_ENTER) {
		if(btnPressed == BTN_EDIT) {
			btnPressed = BTN_QUIT_EDIT;
		} else {
			btnPressed = BTN_ENTER;
		}
	}

	//UP
	if(btnNumber == USER_BUTTON_UP) {
		if(btnPressed == BTN_NONE) {
			btnPressed = BTN_UP;
		}else if(btnPressed == BTN_EDIT){
			btnEdit = BTN_UP;
		}
	}

	//DOWN
	if(btnNumber == USER_BUTTON_DOWN) {
		if(btnPressed == BTN_NONE) {
			btnPressed = BTN_DOWN;
		}else if(btnPressed == BTN_EDIT){
			btnEdit = BTN_DOWN;
		}
	}

}

void JKLCDMenu::DrawMenu()
{
	char buff[50];
	uint16_t pixelWidth = 0;
	currentItemsCount = 0;
	uint8_t scrollShift = 0;

	FillCurrentItemsTable(currentLevelID); //sets also currentItemsCount


	if(currentItemsTableIndex > (LCDMENU_MAX_DISP_ROWS -1) ) {
		scrollShift = currentItemsTableIndex - LCDMENU_MAX_DISP_ROWS+1;
	}
	uint8_t j = 0;
	for( uint8_t i = 0 ; i<LCDMENU_MAX_DISP_ROWS && i < currentItemsCount ; i++) {
		//higlighted active menu row buffer fill
		if (MenuItems[currentItemsTable[i + scrollShift]].itemIndex == currentItemIndex) {
			sprintf((char*)buff, "%s%s%s", LCDMENU_ACTIVE_LEADSTRING, MenuItems[currentItemsTable[i + scrollShift]].text, LCDMENU_SPARE_SPACES);
		//inactive active menu row buffer fill
		} else {
			sprintf((char*)buff, "%s%s%s", LCDMENU_INACTIVE_LEADSTRING, MenuItems[currentItemsTable[i + scrollShift]].text, LCDMENU_SPARE_SPACES);
		}

					//current item index:
					//MenuItems[currentItemsTable[i + scrollShift]].itemIndex

		//truncate string to fit the menu width
		pixelWidth = strlen(buff) * LCDMENU_FONTCHAR_WIDTH;
		if((pixelWidth / LCDMENU_FONTCHAR_WIDTH) > LCDMENU_MAX_CHARS_WIDTH){
			//if(currentItemsCount > LCDMENU_MAX_DISP_ROWS){
			//	buff[LCDMENU_MAX_CHARS_WIDTH - 1] = '\0';
			//} else {
				buff[LCDMENU_MAX_CHARS_WIDTH] = '\0';
			//}
		}

		//higlighted active menu row
		if (MenuItems[currentItemsTable[i + scrollShift]].itemIndex == currentItemIndex) {
			LCDMENU_WriteStringActive((char*)buff, LCDMENU_X_POS, LCDMENU_Y_POS + (i * LCDMENU_ROW_HEIGHT));
		//inactive menu row
		} else {
			LCDMENU_WriteString((char*)buff, LCDMENU_X_POS, LCDMENU_Y_POS + (i * LCDMENU_ROW_HEIGHT));
		}
		j = i;

	}

	//Arrow up and down scrolling
	if(currentItemsCount > LCDMENU_MAX_DISP_ROWS){
		if(scrollShift > 0) {
			LCDMENU_WriteStringActive((char*)LCDMENU_SCROLLARROW_UP, LCDMENU_WIDTH - LCDMENU_FONTCHAR_WIDTH, LCDMENU_Y_POS);
		}
		if((j + scrollShift) != (currentItemsCount-1)) {
			LCDMENU_WriteStringActive((char*)LCDMENU_SCROLLARROW_DOWN, LCDMENU_WIDTH - LCDMENU_FONTCHAR_WIDTH, LCDMENU_Y_POS + ((LCDMENU_MAX_DISP_ROWS-1) * LCDMENU_ROW_HEIGHT));
		}
	}

}

void JKLCDMenu::ProcessMenu()
{

	//char buff[20];

	if(btnPressed != BTN_NONE) {

		if(btnPressed == BTN_UP) {
			if(currentItemsTableIndex < currentItemsCount - 1) {
				currentItemsTableIndex++;
			} else {
				if(LCDMENU_PERPETUAL_SCROLL) currentItemsTableIndex = 0;
			}
			currentItemIndex = currentItemsTable[currentItemsTableIndex];
			DrawMenu();
			btnPressed = BTN_NONE;


		} else if(btnPressed == BTN_DOWN) {
			if(currentItemsTableIndex > 0) {
				currentItemsTableIndex--;
			} else {
				if(LCDMENU_PERPETUAL_SCROLL) currentItemsTableIndex = currentItemsCount - 1;
			}
			currentItemIndex = currentItemsTable[currentItemsTableIndex];
			DrawMenu();
			btnPressed = BTN_NONE;

		} else if(btnPressed == BTN_ENTER) {

			if(MenuItems[currentItemIndex].itemType == ITEMTYPE_NORMAL) {
				DrawMenuEditHeader();
				btnPressed = BTN_EDIT;
				btnEdit = BTN_EDIT; //first run - to be printed
			} else if(MenuItems[currentItemIndex].itemType == ITEMTYPE_CALLBACK_ONLY) {
				DrawMenuEditHeader();
				btnPressed = BTN_EDIT;
				btnEdit = BTN_EDIT; //first run - to be printed
			} else if (MenuItems[currentItemIndex].itemType == ITEMTYPE_GOTOSUBMENU) {

				currentLevelID = MenuItems[currentItemIndex].childLevelID;

				currentItemIndex = FillCurrentItemsTable(currentLevelID); //to first item be highlighted

				currentItemsTableIndex = 0;

				LCDMENU_ClearArea(LCDMENU_X_POS, LCDMENU_Y_POS, LCDMENU_WIDTH, LCDMENU_HEIGHT);
				DrawMenu();
				btnPressed = BTN_NONE;
			} else if (MenuItems[currentItemIndex].itemType == ITEMTYPE_EXITSUBMENU) {
				currentLevelID = MenuItems[currentItemIndex].parentLevelID;

				currentItemIndex = FillCurrentItemsTable(currentLevelID); //to first item be highlighted

				currentItemsTableIndex = 0;

				LCDMENU_ClearArea(LCDMENU_X_POS, LCDMENU_Y_POS, LCDMENU_WIDTH, LCDMENU_HEIGHT);
				DrawMenu();
				btnPressed = BTN_NONE;
			}

		} else if(btnPressed == BTN_EDIT) {

			if(MenuItems[currentItemIndex].itemType == ITEMTYPE_NORMAL){
				EditValue();
			}else if(MenuItems[currentItemIndex].itemType == ITEMTYPE_CALLBACK_ONLY) {
				EditCallbackOnly();
			}

		} else if(btnPressed == BTN_QUIT_EDIT) {
			DrawMenu();
			btnPressed = BTN_NONE;
		}

		//sprintf((char*)buff, "%i %i %i %i %i",currentLevelID, currentItemsTableIndex, currentItemsTable[currentItemsTableIndex], currentItemsCount, currentItemsTable[0]);
		//LCDMENU_WriteString((char*)buff, 0, 140);

	} else {


	}
}

void JKLCDMenu::DrawMenuEditHeader()
{
	char buff[50];
	uint16_t pixelWidth = 0;
	LCDMENU_ClearArea(LCDMENU_X_POS, LCDMENU_Y_POS, LCDMENU_WIDTH, LCDMENU_HEIGHT);
	sprintf((char*)buff, "%s%s:%s",LCDMENU_EDIT_LEADSTRING, MenuItems[currentItemIndex].text, LCDMENU_SPARE_SPACES);

	pixelWidth = strlen(buff) * LCDMENU_FONTCHAR_WIDTH;
	if((pixelWidth / LCDMENU_FONTCHAR_WIDTH) > LCDMENU_MAX_CHARS_WIDTH){
		buff[LCDMENU_MAX_CHARS_WIDTH] = '\0';
	}

	LCDMENU_WriteStringActive((char*)buff, LCDMENU_X_POS, LCDMENU_Y_POS);
}

void JKLCDMenu::EditValue()
{
	if(btnEdit != BTN_NONE) {

		if(btnEdit == BTN_UP) {
			if(MenuItems[currentItemIndex].call != NULL) {
				//Callback with +1 param
				(*(MenuItems[currentItemIndex].call))(1);
			} else {
				//Direct change value ++
				*MenuItems[currentItemIndex].value = *MenuItems[currentItemIndex].value + 1;
			}

		} else if(btnEdit == BTN_DOWN) {
			if(MenuItems[currentItemIndex].call != NULL) {
				//Callback with -1 param
				(*(MenuItems[currentItemIndex].call))(-1);
			} else {
				//Direct change value ++
				*MenuItems[currentItemIndex].value = *MenuItems[currentItemIndex].value - 1;
			}
		} else {
			//Callback with 0 param
			if(MenuItems[currentItemIndex].call != NULL) {
				(*(MenuItems[currentItemIndex].call))(0);
			}
		}

		//Display value
		char buff[20];
		sprintf((char*)buff, " %ld  ", *MenuItems[currentItemIndex].value);
		LCDMENU_WriteString((char*)buff, LCDMENU_X_POS, LCDMENU_Y_POS + (2*LCDMENU_ROW_HEIGHT));

		btnEdit = BTN_NONE;

		//MENU_ITEM &itm = MenuItems[currentIndex];
		//MenuCallback call = itm.call;
		//(*(call))();
	}
}

void JKLCDMenu::EditCallbackOnly()
{
	if(btnEdit != BTN_NONE && MenuItems[currentItemIndex].call == NULL) {
		//Display error No Callback
		btnEdit = BTN_NONE;
		LCDMENU_WriteString((char*)"Err:NoCallbackFn", LCDMENU_X_POS, LCDMENU_Y_POS + (2*LCDMENU_ROW_HEIGHT));
		return;
	}

	if(btnEdit != BTN_NONE) {

		if(btnEdit == BTN_UP) {
				//Callback with +1 param
				(*(MenuItems[currentItemIndex].call))(1);

		} else if(btnEdit == BTN_DOWN) {
				//Callback with -1 param
				(*(MenuItems[currentItemIndex].call))(-1);
		} else {
			//Callback with 0 param
				(*(MenuItems[currentItemIndex].call))(0);
		}

		btnEdit = BTN_NONE;
	}
}

void JKLCDMenu::addItem(uint8_t parentLevelID, uint8_t levelID, uint8_t childLevelID, char* txt, uint8_t itemType, int32_t* value, MenuCallback func )
{
	uint8_t pos = itemCount;  // position of newly added task is based on priority
	MENU_ITEM &itm = MenuItems[pos];
	itm.itemIndex = itemCount;
	itm.itemType = itemType;
	itm.text = txt;
	itm.parentLevelID = parentLevelID;
	itm.levelID = levelID;
	itm.childLevelID = childLevelID;
	itm.value = value;
	itm.call = func;
	itemCount++;
}


void JKLCDMenu::addItem(uint8_t parentLevelID, uint8_t levelID, uint8_t childLevelID, char* txt, uint8_t itemType, int32_t* value)
{
	uint8_t pos = itemCount;  // position of newly added task is based on priority
	MENU_ITEM &itm = MenuItems[pos];
	itm.itemIndex = itemCount;
	itm.itemType = itemType;
	itm.text = txt;
	itm.parentLevelID = parentLevelID;
	itm.levelID = levelID;
	itm.childLevelID = childLevelID;
	itm.value = value;
	itm.call = NULL;
	itemCount++;
}

void JKLCDMenu::addItem(uint8_t parentLevelID, uint8_t levelID, uint8_t childLevelID, char* txt, uint8_t itemType)
{
	uint8_t pos = itemCount;  // position of newly added task is based on priority
	MENU_ITEM &itm = MenuItems[pos];
	itm.itemIndex = itemCount;
	itm.itemType = itemType;
	itm.text = txt;
	itm.parentLevelID = parentLevelID;
	itm.levelID = levelID;
	itm.childLevelID = childLevelID;
	itm.value = NULL;
	itm.call = NULL;
	itemCount++;
}


uint8_t JKLCDMenu::FillCurrentItemsTable(uint8_t currenLevelId) {
	//char buff[20];
	currentItemsCount = 0;
	for(uint8_t i =0;i<itemCount;i++) {

		if(MenuItems[i].levelID == currentLevelID) {
			//fill curent menu level index table
			currentItemsTable[currentItemsCount] = MenuItems[i].itemIndex;
			currentItemsCount++;
		}
	}
	//sprintf((char*)buff, "%i%i",currentLevelID,currentItemsTable[0] );
	//LCDMENU_WriteString((char*)buff, 0, 160);

	return currentItemsTable[0];
}


//run
//MenuCallback call = itm.call;
//(*(call))();
