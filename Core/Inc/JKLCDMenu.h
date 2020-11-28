/*
 * JKLCDMenu.h
 *
 *  Created on: Oct 19, 2020
 *      Author: KlokockaJir
 */

#ifndef INC_JKLCDMENU_H_
#define INC_JKLCDMENU_H_

#include "main.h"

void LCDMENU_ClearArea(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);
void LCDMENU_WriteString(char* buffer, uint16_t posx, uint16_t posy);
void LCDMENU_WriteStringActive(char* buffer, uint16_t posx, uint16_t posy);
void LCDMENU_RectangleAround(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);
void LCDMENU_RectangleAround_Active(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);


#define USER_BUTTON_NONE	0
#define USER_BUTTON_ENTER	1
#define USER_BUTTON_UP		2
#define USER_BUTTON_DOWN	3


#define LCDMENU_X_POS 				0	//pixel width of one font character
#define LCDMENU_Y_POS 				23	//pixel width of one font character
#define LCDMENU_FONTCHAR_WIDTH 		11	//pixel width of one font character
#define LCDMENU_FONTCHAR_HEIGHT 	18	//pixel width of one font character
#define LCDMENU_ROW_HEIGHT 			(LCDMENU_FONTCHAR_HEIGHT + 0) //pixel height of one row
#define LCDMENU_MAX_DISP_ROWS		5   //Max displayed rows
#define LCDMENU_HEIGHT 				(LCDMENU_MAX_DISP_ROWS * LCDMENU_ROW_HEIGHT) //pixel height of whole menu
#define LCDMENU_WIDTH 				20 * LCDMENU_FONTCHAR_WIDTH  //Menu pixel width is  14 * LCDMENU_FONTCHAR_WIDTH
#define LCDMENU_MAX_CHARS_WIDTH 	LCDMENU_WIDTH / LCDMENU_FONTCHAR_WIDTH
#define LCDMENU_ACTIVE_LEADSTRING 	""
#define LCDMENU_INACTIVE_LEADSTRING ""
#define LCDMENU_EDIT_LEADSTRING 	""
#define LCDMENU_SPARE_SPACES 		"                " //wil be shorten automaticaly
#define LCDMENU_SCROLLARROW_UP 		"^"
#define LCDMENU_SCROLLARROW_DOWN	"v"

#define LCDMENU_PERPETUAL_SCROLL	0 //when reaches end -> skips to first

#define LCDMENU_MAXITEMS			35
#define LCDMENU_MAXITEMSPERLEVEL	20

/*LCDMENU_TEXT_COLOR;
LCDMENU_BG_COLOR;
LCDMENU_TEXT_COLOR_ACTIVE;
LCDMENU_BG_COLOR_ACTIVE;*/




typedef void (*MenuCallback)(int); //(int)


class JKLCDMenu
{
	public:
		JKLCDMenu();
		void CheckButtons(uint8_t btnNumber);
		void DrawMenu();
		void DrawMenuEditHeader();
		void ProcessMenu();
		void EditValue();
		void EditCallbackOnly();

		struct MENU_ITEM {
			uint8_t itemIndex;
			uint8_t itemType;
			char* text;
			MenuCallback call;
			int32_t* value;
			uint8_t parentLevelID;
			uint8_t levelID;
			uint8_t childLevelID;
		};

		MENU_ITEM MenuItems[LCDMENU_MAXITEMS];

		//, int32_t lowerLimit, int32_t upperLimit, int32_t multiplier

		void addItem(uint8_t parentLevelID, uint8_t levelID, uint8_t childLevelID, char* txt, uint8_t itemType, int32_t* value, MenuCallback func );
		void addItem(uint8_t parentLevelID, uint8_t levelID, uint8_t childLevelID, char* txt, uint8_t itemType, int32_t* value);
		void addItem(uint8_t parentLevelID, uint8_t levelID, uint8_t childLevelID, char* txt, uint8_t itemType);


		uint8_t btnPressed;
		uint8_t btnEdit;

	private:
		uint8_t FillCurrentItemsTable(uint8_t currenLevelId);
		int8_t currentItemIndex;
		uint8_t itemCount;
		uint8_t currentLevelID;
		uint8_t currentItemsCount;
		uint8_t currentItemsTable[LCDMENU_MAXITEMSPERLEVEL];
		uint8_t currentItemsTableIndex = 0;
};


typedef enum {
	 BTN_NONE = 0,
	 BTN_UP = 1,
	 BTN_DOWN = 2,
	 BTN_ENTER = 3,
	 BTN_EDIT = 4,
	 BTN_QUIT_EDIT = 5
}btnPressed;

typedef enum {
	ITEMTYPE_NORMAL = 0,
	ITEMTYPE_GOTOSUBMENU = 1,
	ITEMTYPE_EXITSUBMENU = 2,
	ITEMTYPE_CALLBACK_ONLY = 3


}itemTypes;


#endif /* INC_JKLCDMENU_H_ */

