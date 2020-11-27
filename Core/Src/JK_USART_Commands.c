/*
 * JK_USART_Commands.c
 *
 *  Created on: Oct 28, 2020
 *      Author: KlokockaJir
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "JK_USART_Commands.h"
#include "ILI9341_STM32_Driver.h"

extern int pwm1_value_A;
extern int pwm2_value_A;
extern int pwm3_value_A;

extern int pwm1_value_B;
extern int pwm2_value_B;
extern int pwm3_value_B;




uint8_t USART_Extract_Commands(uint8_t inputChar, uint8_t* commandBuffer,uint8_t commandBuffLen) {

	//uint8_t i = 0;
	static bool varFound = 0;
	static uint8_t j = 0;

		// 2) start filling the buffer and search for ENDING_CHAR
		if(varFound == 1) {
			uint8_t cmdLength = 0;

			if(j < (commandBuffLen - 1)) {
				if(inputChar == '$') { //osetreni aaaa$aaaa#aaaa$aaaa
					j = 0;
					varFound = 0;
				} else {
					if(inputChar == '#') {
						commandBuffer[j] = '\r';
						commandBuffer[j+1] = '\0';
						cmdLength = j; //length for return
						j = 0;
						//USART_Process_Commands(buffStorePair, strlen(buffStorePair));
						HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
						varFound = 0;
						return cmdLength;
					} else {
						commandBuffer[j] = inputChar;
						j++;
					}
				}
			} else {
				varFound = 0;
				j=0;
			}
		}

		// 1) If BEGIN_CHAR is found then in the next iteration starts filling the commandBuffer[]
		if(inputChar == '$') { //pokud nasel $ => v dalsi iteraci se bude nacitat buffStorePair
			varFound = 1;
		}
	return 0;
}

void USART_Process_Commands(char* cmdBuff, uint8_t len){

    STRSWITCH (cmdBuff)
        STRCASE ("GREENBLACK")
			ILI9341_Log_SetColors(GREEN, BLACK);
            break;
        STRCASE ("BLACKGREEN")
            ILI9341_Log_SetColors(BLACK, GREEN);
            break;
        STRCASE ("WHITEBLACK")
        	ILI9341_Log_SetColors(WHITE, BLACK);
            break;
        STRCASE ("BLACKWHITE")
            ILI9341_Log_SetColors(BLACK, WHITE);
            break;
        STRDEFAULT
			char varNameBuff[USART_CMDBUFFERSIZE];
			USART_GetCommandName(cmdBuff , varNameBuff);

			STRSWITCH(varNameBuff)
				STRCASE("R_A")
					//pwm1_setvalue(USART_GetCommandValue(cmdBuff));
					pwm1_value_A = USART_GetCommandValue(cmdBuff);
					break;
				STRCASE("G_A")
					pwm2_value_A = USART_GetCommandValue(cmdBuff);
					break;
				STRCASE("B_A")
					pwm3_value_A = USART_GetCommandValue(cmdBuff);
					break;

				STRCASE("R_B")
					pwm1_value_B = USART_GetCommandValue(cmdBuff);
					break;
				STRCASE("G_B")
					pwm2_value_B = USART_GetCommandValue(cmdBuff);
					break;
				STRCASE("B_B")
					pwm3_value_B = USART_GetCommandValue(cmdBuff);
					break;
				STRCASE("R_C")
					pwm1_value_A = USART_GetCommandValue(cmdBuff);
					pwm1_value_B = pwm1_value_A - 10;
					break;


				STRDEFAULT
					break;
		STREND
    	    break;
    STREND

	/*

	 if(USART_Command_Simple("WHITEBLACK", buff)) {
		 ILI9341_Log_SetColors(WHITE, BLACK);
	 }

	 USART_Command_Value("PWMRED", buff , &pwm1_value);

	 int pwm = 0;
	 if(USART_Command_Value("PWMRD", buff , &pwm)){
		 user_pwm1_setvalue_log(pwm);
	 }

	 //uint8_t ILI9341_Log_fontNr;
	 //USART_Command_Value("FONT", buff , &ILI9341_Log_FONTNR);
	 //ILI9341_Log_SetFontNr(ILI9341_Log_fontNr);

	 int dateVal = 0;
	 if(USART_Command_Value("SET_DAY", buff , &dateVal)) {
		 SetMyDay(dateVal);
	 }
	 if(USART_Command_Value("SET_MONTH", buff , &dateVal)) {
		 SetMyMonth(dateVal);
	 }
	 if(USART_Command_Value("SET_YEAR", buff , &dateVal)) {
		 SetMyYear(dateVal);
	 }

	 if(USART_Command_Value("SET_HOURS", buff , &dateVal)) {
		 SetMyHours(dateVal);
	 }
	 if(USART_Command_Value("SET_MINUTES", buff , &dateVal)) {
		 SetMyMinutes(dateVal);
	 }
	 if(USART_Command_Value("SET_SECONDS", buff , &dateVal)) {
		 SetMySeconds(dateVal);
	 }
	 */

	 /*
	 ILI9341_Log_SetFColor(WHITE);
	 char buff2[50];
	 sprintf((char*)buff2, "%s\n", buff);
	 ILI9341_LogToLCD(buff2);
	 ILI9341_Log_SetFColor(GREEN);
	 */


}




uint8_t USART_Command_Simple(char* varNameBuff, char* cmdBuff){

	if(JK_strpos(cmdBuff, varNameBuff, 0) == 0 && strlen(varNameBuff) == strlen(cmdBuff)){
		return 1;
	} else {
		return 0;
	}
}

/*
 * Stor mumeric value from cmdBuff into *value where variable name is varNameBuff
 * USART_Command_Value("VARIABLENAME", cmdBuff, &val)
 */
uint8_t USART_Command_Value(char* varNameBuff, char* cmdBuff , int* value){

	uint8_t varLen = strlen(varNameBuff);

	//If cmdBuff is longer and cmdBuff contains varNameBuff then its not just a simple command
	if(strlen(cmdBuff) > varLen && JK_strpos(cmdBuff, varNameBuff, 0) == 0) { //pokud ne, jedna se o prikaz

		uint8_t j = 0;
		uint8_t negativeSign = 0;
		uint8_t varFound = 0;


	    //Is there negative sighn roght after variable name?
		if (cmdBuff[varLen] == '-')
	    {
	    	negativeSign = 1;
	    }

		//Is there a digit after variable name (and negative sign)
		if(JK_isdigit(cmdBuff[varLen + negativeSign])) {  //pokud nasleduje cislo, trasujme ho

			//Fill tmpVarNameBuff with data from cmdBuff for comparation
			char tmpVarNameBuff[varLen+1];
			for(j = 0; j < varLen; j++){
				tmpVarNameBuff[j] = cmdBuff[j];
			}
			tmpVarNameBuff[varLen] = '\0'; //Add \0 to the end

			//Compare buffers
			if( strcmp(tmpVarNameBuff, varNameBuff) == 0 ){
				varFound = 1;
			}

			//Count number of digits
			uint8_t k = 0;
			while(k < strlen(cmdBuff) && JK_isdigit(cmdBuff[k + varLen + negativeSign])){
				k++;
			}

			//Check if there are diggits
			if(varFound && k > 0 ){
				 char tempValBuff[k + 1 + negativeSign];

				 if(negativeSign) {
					 tempValBuff[0] = '-';
				 }

				 //Fill the buffer with digits (and sign)
				 for(uint8_t i=0; i<k; i++) { //naplnit buffer s ciframa
					 tempValBuff[i+negativeSign] = cmdBuff[i + varLen + negativeSign];
				 }
				 tempValBuff[k + negativeSign] = '\0';

				 //Get number from string
				 *value = JK_atoi(tempValBuff);

				 /*
				 ILI9341_Log_SetFColor(CYAN);
				 char buff2[50];
				 sprintf((char*)buff2, "%s %s %i\n",varNameBuff, tempValBuff, *value);
				 ILI9341_LogToLCD(buff2);
				 ILI9341_Log_SetFColor(GREEN);
				 */

				 /*
				 char buff2[512];
				 sprintf((char*)buff2,"|cmdBuff:%s| |varLen:%i| |j:%i| |varNameBuff:%s| |tmpVarNameBuff:%s| |varFound:%i| |k:%i| |tempValBuff:%s| |value:%i|\r\n", cmdBuff, varLen, j, varNameBuff, tmpVarNameBuff, varFound, k, tempValBuff, atoi(tempValBuff));  // `strstr` returns NULL if search string not found
			     CDC_Transmit_FS((uint8_t*)buff2, strlen((char*)buff2));
			     */
			}
			return 1;
		} else {
			return 0;
		}

	} else {
		return 0;
	}

}

/*
 * Extract all non-numeric character from cmdBuff into varNameBuff
 */
void USART_GetCommandName(char* cmdBuff , char* varNameBuff){

	uint8_t varLen = strlen(cmdBuff);
	uint8_t i = 0;
	while(i < varLen && !JK_isdigit(cmdBuff[i]) && cmdBuff[i] != '-') {
		varNameBuff[i] = cmdBuff[i];
		i++;
	}
	varNameBuff[i] = '\0';
}

/*
 * Extract numeric value from cmdBuff
 */
int USART_GetCommandValue(char* cmdBuff){

	uint8_t varLen = strlen(cmdBuff);
	uint8_t k = 0, i = 0;
	char tempValBuff[10];
	while(i < varLen) {
		if((JK_isdigit(cmdBuff[i]) || cmdBuff[i] == '-') && k < 10){
			tempValBuff[k] = cmdBuff[i];
			k++;
		}
		i++;
	}
	tempValBuff[k] = '\0';

	return JK_atoi(tempValBuff);
}

int JK_strpos(char *hay, char *needle, int offset)
{
   char haystack[strlen(hay)];
   strncpy(haystack, hay+offset, strlen(hay)-offset);
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack+offset;
   return -1;
}

uint8_t JK_isdigit(char c) {
	if(c > 47 && c < 58){
		return 1;
	} else {
		return 0;
	}
}

int JK_atoi(char* str)
{
    if (*str == '\0')
        return 0;

    // Initialize result
    int res = 0;

    // Initialize sign as positive
    int sign = 1;

    // Initialize index of first digit
    int i = 0;

    // If number is negative,
    // then update sign
    if (str[0] == '-')
    {
        sign = -1;

        // Also update index of first digit
        i++;
    }

    // Iterate through all digits of
    // input string and update result
    for (; str[i] != '\0'; ++i)
    {

        // You may add some lines
        // to write error message
        if (JK_isdigit(str[i]) == false)
            return 0;

        // to error stream
        res = res * 10 + str[i] - '0';
    }

    // Return result with sign
    return sign * res;
}


void logJK(char* str, uint16_t clr) {
	 ILI9341_Log_SetFColor(clr);
	 char buff2[50];
	 sprintf((char*)buff2, "%s\n", str);
	 ILI9341_LogToLCD(str);
	 ILI9341_Log_SetFColor(GREEN);
}
