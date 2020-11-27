
/*
 *
 *
 * Menu2 v2.0
 * Martin Hubacek
 * 18.3.2013
 * http://martinhubacek.cz
 *
 *
 */

#ifndef __M2PLATFORM_H__
#define __M2PLATFORM_H__

#ifdef __cplusplus
 extern "C" {
#endif


//#include "keyboard.h"
//#include "lcd.h"
//#include "ustdlib.h"

 #include "ILI9341_STM32_Driver.h"

#define MENU_LANGUAGES 2

// If you use different languages, define them here
#define LANGUAGE_CZECH 0
#define LANGUAGE_ENGLISH 1



// Set rows/cols based on your font (for graphical displays)
#define ROW(x) ((x)*20)
#define COL(y) ((y)*14)
// For character LCDs use definitions below
//#define ROW(x) (x)
//#define COL(y) (y)

// Number of items on one screen
// Not including title
#define MENU_LINES 10

//1
//2-scroll dolu - sipka neni na 1. radku
//3ok
//4 posledni scroll dolu blbne
//5ok

// Symbol which is displayed in front of the selected item
// This symbol doesn't appear when MENU_LINES == 1
#define ARROW_SYMBOL ">"
// How many spaces is between arrow symbol and menu item
// useful to set zero on smaller displays
#define ARROW_GAP 1

// Clear display
#define displayClear()	lcdBufferClear()

#define displayClear2()	lcdBufferClear2()

// Display string
#define displayString(str, posx, posy) lcdBufferString(str, posx, posy)
// If you have separate functions for set position and print text, use define below
//#define displayString(str, posx, posy) {lcdGotoXY(posx, posy); lcdBufferString(str);}

// Display number
#define displayNumber(str, posx, posy) lcdBufferNumber(str, posx, posy)
// If you have separate functions for set position and print number, use define below
//#define displayNumber(str, posx, posy) {lcdGotoXY(posx, posy); lcdBufferNumber(str)}

// Optional function to write buffer to display - comment if not used
//#define displayDraw()		lcdBufferDraw()


#define MENU_MS_TICK HAL_GetTick()
extern volatile unsigned long msTick;

#ifdef __cplusplus
}
#endif

#endif
