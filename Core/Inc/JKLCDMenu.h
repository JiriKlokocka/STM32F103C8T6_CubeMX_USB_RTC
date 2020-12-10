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


#define LCDMENU_X_POS 				20
#define LCDMENU_Y_POS 				30
#define LCDMENU_FONTCHAR_WIDTH 		11	//pixel width of one font character
#define LCDMENU_FONTCHAR_HEIGHT 	18	//pixel height of one font character
#define LCDMENU_ROW_HEIGHT 			(LCDMENU_FONTCHAR_HEIGHT + 2)  //pixel height of one row
#define LCDMENU_MAX_DISP_ROWS		5   //Max displayed rows
#define LCDMENU_HEIGHT 				(LCDMENU_MAX_DISP_ROWS * LCDMENU_ROW_HEIGHT) //pixel height of whole menu
#define LCDMENU_WIDTH 				20 * LCDMENU_FONTCHAR_WIDTH  //Menu pixel width is:   * LCDMENU_FONTCHAR_WIDTH
#define LCDMENU_MAX_ITEM_CHARS 	LCDMENU_WIDTH / LCDMENU_FONTCHAR_WIDTH
#define LCDMENU_ACTIVE_LEADSTRING 	">"
#define LCDMENU_INACTIVE_LEADSTRING ""
#define LCDMENU_EDIT_LEADSTRING 	""
#define LCDMENU_SPARE_SPACES 		"                          " //wil be shorten automaticaly
#define LCDMENU_SCROLLARROW_UP 		"^"
#define LCDMENU_SCROLLARROW_DOWN	"v"

#define LCDMENU_PERPETUAL_SCROLL	0 //when reaches end -> skips to first


#define LCDMENU_MAXITEMS			35
#define LCDMENU_MAXITEMSPERLEVEL	20

#define LCDMENU_TEXT_OFFSET_X		2
#define LCDMENU_TEXT_OFFSET_Y		3

#define LCDMENU_USE_RECTANGLES		0
#define LCDMENU_RECTANGLE_HEIGHT 	(uint16_t)(LCDMENU_FONTCHAR_HEIGHT + LCDMENU_TEXT_OFFSET_Y + 1)

#if(LCDMENU_USE_RECTANGLES)
void LCDMENU_RectangleAround(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);
void LCDMENU_RectangleAround_Active(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);
#endif

/*  Functions definitions example

void LCDMENU_ClearArea(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height)  {
	ILI9341_Draw_Filled_Rectangle_Coord(posx,posy,posx+width,posy+height,BLACK);
}

void LCDMENU_WriteString(char* buffer, uint16_t posx, uint16_t posy) {
	ILI9341_WriteString(posx, posy, (char*)buffer, Font_11x18, ORANGE, BLACK);
}

void LCDMENU_WriteStringActive(char* buffer, uint16_t posx, uint16_t posy) {
	ILI9341_WriteString(posx, posy, (char*)buffer, Font_11x18, WHITE, BLACK);
}

void LCDMENU_RectangleAround(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height) {
	ILI9341_Draw_Hollow_Rectangle_Coord(posx, posy, posx + width, posy + height, BLACK);
}
void LCDMENU_RectangleAround_Active(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height) {
	ILI9341_Draw_Hollow_Rectangle_Coord(posx, posy, posx + width, posy + height, ORANGE);
}

*/

typedef void (*MenuCallback)(int); //(int)


class JKLCDMenu
{
	public:
		JKLCDMenu();
		void CheckButtons(uint8_t btnNumber);
		void Init();
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
			uint8_t showValInLine;
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

