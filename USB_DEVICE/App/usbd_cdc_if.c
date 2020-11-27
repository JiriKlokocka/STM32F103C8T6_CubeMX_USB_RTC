/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
#include "ILI9341_STM32_Driver.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
#define APP_RX_DATA_SIZE  1000
#define APP_TX_DATA_SIZE  1000

#define ARRSIZE(a) (sizeof(a) / sizeof(*a))
#define BUFFSTOREPAIRSIZE 100
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

extern uint16_t ILI9341_Log_FONTNR;
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
void VCP_Etract_Commands(uint8_t* Buf, uint16_t Len);
void VCP_Process_Commands(char* str, uint8_t len);
uint8_t VCP_Command(char* varBuff, char* buff);
uint8_t VCP_Command_Value(char* varBuff, char* buff , uint16_t* value);
int JKstrpos(char *hay, char *needle, int offset);
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
	VCP_Etract_Commands(Buf, *Len);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void VCP_Etract_Commands(uint8_t* Buf, uint16_t Len) {

	//RX Buffer ma velikost 64
	//char buff[200];

	static char buffStorePair[BUFFSTOREPAIRSIZE];
	uint8_t i = 0;
	static uint8_t j = 0;
	static bool varFound = 0;

	while(i < Len) {

		if(varFound == 1) {
			if(j < (BUFFSTOREPAIRSIZE - 1)) {
				if(Buf[i] == '$') { //osetreni aaaa$aaaa#aaaa$aaaa
					j = 0;
					varFound = 0;
				} else {
					if(Buf[i] == '#') {
						buffStorePair[j] = '\0';
						j = 0;
						VCP_Process_Commands(buffStorePair, strlen(buffStorePair));
						varFound = 0;
					} else {
						buffStorePair[j] = Buf[i];
						j++;
					}
				}

			} else {
				varFound = 0;
				j=0;
			}
		}

		if(Buf[i] == '$') { //pokud nasel $ => v dalsi iteraci se bude nacitat buffStorePair
			varFound = 1;
		}
		i++;
	}



/*

	 uint8_t *s;
	 s = (uint8_t*)strstr((char*)Buf, "$cmd:");
	 if (s != NULL)
	 {
		 varPos = (s - Buf) + 5;
	}

*/


}

void VCP_Process_Commands(char* buff, uint8_t len){

	 if(VCP_Command("GREENBLACK", buff)) {
		 ILI9341_Log_SetFColor(GREEN);
		 ILI9341_Log_SetBColor(BLACK);
	 }

	 if(VCP_Command("BLACKGREEN", buff)) {
		 ILI9341_Log_SetFColor(BLACK);
		 ILI9341_Log_SetBColor(DARKGREEN);
	 }

	 if(VCP_Command("WHITEBLACK", buff)) {
		 ILI9341_Log_SetColors(WHITE, BLACK);

	 }

	 if(VCP_Command("BLACKWHITE", buff)) {
		 ILI9341_Log_SetColors(BLACK, WHITE);
	 }


	 if(VCP_Command("LIGHTOFF", buff)) {
		 //pwm_value = 0;
	 }

	 if(VCP_Command("LIGHTON", buff)) {
		 //pwm_value = 1000;
	 }

	 //VCP_Command_Value("PWM", buff , &pwm1_valueA);

	 //uint8_t ILI9341_Log_fontNr;
	 VCP_Command_Value("FONT", buff , &ILI9341_Log_FONTNR);
	 //ILI9341_Log_SetFontNr(ILI9341_Log_fontNr);

	 uint16_t dateVal = 0;
	 if(VCP_Command_Value("SET_DAY", buff , &dateVal)) {
		 SetMyDay(dateVal);
	 }
	 if(VCP_Command_Value("SET_MONTH", buff , &dateVal)) {
		 SetMyMonth(dateVal);
	 }
	 if(VCP_Command_Value("SET_YEAR", buff , &dateVal)) {
		 SetMyYear(dateVal);
	 }

	 if(VCP_Command_Value("SET_HOURS", buff , &dateVal)) {
		 SetMyHours(dateVal);
	 }
	 if(VCP_Command_Value("SET_MINUTES", buff , &dateVal)) {
		 SetMyMinutes(dateVal);
	 }
	 if(VCP_Command_Value("SET_SECONDS", buff , &dateVal)) {
		 SetMySeconds(dateVal);
	 }


	 ILI9341_Log_SetFColor(WHITE);
	 ILI9341_LogToLCD(buff);
	 ILI9341_Log_SetFColor(GREEN);

}


uint8_t VCP_Command(char* varBuff, char* buff){

	if(JKstrpos(buff, varBuff, 0) == 0 && strlen(varBuff) == strlen(buff)){
		return 1;
	} else {
		return 0;
	}


}

uint8_t VCP_Command_Value(char* varBuff, char* buff , uint16_t* value){


	uint8_t j = 0;
	uint8_t varLen = strlen(varBuff);
	uint8_t varFound = 0;
	char tmpBuff[varLen+1];

	if(varLen < strlen(buff) && JKstrpos(buff, varBuff, 0) == 0) { //pokud ne, jedna se o prikaz
		/*ILI9341_Log_SetFColor(YELLOW);
		ILI9341_LogToLCD(varBuff);
		ILI9341_Log_SetFColor(GREEN);*/

		 if(isdigit(buff[varLen])) {  //pokud nasleduje cislo, trasujme ho
			 //strncpy(tmpBuff, buff, varLen);
			 for(j = 0; j < varLen; j++){ //naplnit buffer pro nazev promenny
				 tmpBuff[j] = buff[j];
			 }
			 tmpBuff[varLen] = '\0'; //doplnit 0, jinak nefungujo textovy funkce

			 if( strcmp(tmpBuff, varBuff) == 0 ){ //porovnat
				 varFound = 1;
			 }


			 uint8_t k = 0;
			 while(k < strlen(buff) && isdigit(buff[k + varLen])){  //zjistit pocet cifer
				 k++;
			 }

			 if(varFound && k > 0 ){
				 char valBuff[k+1];
				 //strncpy(valBuff, buff+j, k);
				 for(uint8_t i=0; i<k; i++) { //naplnit buffer s ciframa
					 valBuff[i] = buff[i + varLen];
				 }
				 valBuff[k] = '\0';
				 *value = atoi(valBuff);

				 char buff2[512];
				 sprintf((char*)buff2,"|buff:%s| |varLen:%i| |j:%i| |varBuff:%s| |tmpBuff:%s| |varFound:%i| |k:%i| |valBuff:%s| |value:%i|\r\n", buff, varLen, j, varBuff, tmpBuff, varFound, k, valBuff, atoi(valBuff));  // `strstr` returns NULL if search string not found
			     CDC_Transmit_FS((uint8_t*)buff2, strlen((char*)buff2));
			 }
		 }
		 return 1;
	} else {
		return 0;
	}

}





int JKstrpos(char *hay, char *needle, int offset)
{
   char haystack[strlen(hay)];
   strncpy(haystack, hay+offset, strlen(hay)-offset);
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack+offset;
   return -1;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
