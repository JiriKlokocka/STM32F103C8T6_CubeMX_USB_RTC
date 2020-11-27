/*
 * JK_USART_Commands.h
 *
 *  Created on: Oct 28, 2020
 *      Author: KlokockaJir
 */

#ifndef INC_JK_USART_COMMANDS_H_
#define INC_JK_USART_COMMANDS_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define ARRSIZE(a) (sizeof(a) / sizeof(*a))

#define USART_RXBUFFERSIZE 128
#define USART_CMDBUFFERSIZE 24

#ifndef STRSWITCH_CASE_INIT
#define STRSWITCH_CASE_INIT
    #define STRSWITCH(X)	for (char* __switch_p__ = X, __switch_next__=1 ; __switch_p__ ; __switch_p__=0, __switch_next__=1) { {
    #define STRCASE(X)			} if (!__switch_next__ || !(__switch_next__ = strcmp(__switch_p__, X))) {
    #define STRDEFAULT		  	} {
    #define STREND			}}
#endif


uint8_t USART_Extract_Commands(uint8_t inputChar, uint8_t* commandBuffer,uint8_t commandBuffLen);
void USART_Process_Commands(char* cmdBuff, uint8_t len);
uint8_t USART_Command_Simple(char* varNameBuff, char* cmdBuff);
uint8_t USART_Command_Value(char* varNameBuff, char* cmdBuff , int* value);
void USART_GetCommandName(char* cmdBuff , char* varNameBuff);
int USART_GetCommandValue(char* cmdBuff);
int JK_strpos(char *hay, char *needle, int offset);
uint8_t JK_isdigit(char c);
int JK_atoi(char* str);
void logJK(char* str, uint16_t clr);

#ifdef __cplusplus
}
#endif


#endif /* INC_JK_USART_COMMANDS_H_ */
