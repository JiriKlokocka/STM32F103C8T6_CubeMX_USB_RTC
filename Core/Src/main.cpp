/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* JK Display Connection
 * Display wiki: http://www.lcdwiki.com/2.2inch_SPI_Module_ILI9341_SKU:MSP2202
 * Display				BluePill
 * 1) VCC				3.3V~5V
 * 2) GND				GND
 * 3) CS				PB3
 * 4) RESET				PB5
 * 5) DC/RS				PB4
 * 6) SDI/MOSI			PA7	(SPI1_MOSI)
 * 7) SCK				PA5	(SPI1_SCK)
 * 8) LED				PA8
 * 9) SDO/MISO			PA6	(SPI1_MISO)
 *
 * USART3
 * PB10 USART TX
 * PB11 USART RX
 *
 * TIM3 form PWM over USART3
 * PA8	TIM1_CH1
 * PA9	TIM1_CH2
 * PA10	TIM1_CH3
 *
 * TIM4 Encoder mode: PB6, PB7
 *
 * Encoder Button:  PA1
 *
 * LED: PA2
 *
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdint.h"
#include <string.h>
#include <JKDateTime.h>
#include <JKLCDMenu.h>
#include <JKEEPROM.h>
#include "JK_USART_Commands.h"
#include "ILI9341_STM32_Driver.h"
#include "eeprom.h"
#include "ringbuf.h"
#include <math.h>




/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
extern uint16_t LCD_HEIGHT;
extern uint16_t LCD_WIDTH;


int32_t pwm1_value_A= 0;
int32_t pwm2_value_A= 0;
int32_t pwm3_value_A= 0;
int32_t pwm1_value_B = 0;
int32_t pwm2_value_B = 500;
int32_t pwm3_value_B = 0;

uint8_t USARTcommandBuffer[USART_CMDBUFFERSIZE];
struct ringbuf ringBuffer;
uint8_t ringData(USART_RXBUFFERSIZE);

uint8_t btnFlag = 0;
uint8_t usartFlag = 0;


JKDateTime DateTime;
JKLCDMenu LCDMenu;
JKEEPROM MyEEPROM;



int32_t menu_Hours = 0;
int32_t menu_Minutes = 0;
int32_t menu_Seconds = 0;
int32_t menu_Day = 0;
int32_t menu_Month = 0;
int32_t menu_Year = 0;

struct ringbuf ringBufferCommands;
uint8_t ringDataCommands(USART_RXBUFFERSIZE);


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */



  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  //MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  uint16_t timeCnt = 0;
  char buff[100];

  /*---------UART Enable interrupts-------------*/
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);

  /*---------Timer Encoder-------------*/
  HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
  /*---------Timer PWM-------------*/
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  /*---------Display init -------------*/
  ILI9341_Init();
  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
  //ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
  ILI9341_Fill_Screen(BLACK);
  //Crosshair
  //ILI9341_Draw_Horizontal_Line(0, LCD_HEIGHT/2, LCD_WIDTH, DARKGREY);
  //ILI9341_Draw_Vertical_Line(LCD_WIDTH/2, 0, LCD_HEIGHT, DARKGREY);
  //Top Row
  ILI9341_Draw_Rectangle(0, 0, LCD_WIDTH, 20, NAVY);
  ILI9341_WriteString(320/3, 4, "System started", Font_7x10, WHITE, NAVY);

  //Spectrum
  LCD_DrawSpectrum(20, LCD_HEIGHT/2 + 40 , LCD_WIDTH/2 - 40 , LCD_HEIGHT/2 - 60, 255);
  ILI9341_Draw_Hollow_Rectangle(20, LCD_HEIGHT/2 + 40 , LCD_WIDTH/2 - 40 , LCD_HEIGHT/2 - 60, DARKGREY);

  //---------LCD Log init-------------
  ILI9341_Log_Init( LCD_WIDTH/2 + 20  , LCD_HEIGHT/2 + 40, LCD_WIDTH/2 - 40 , LCD_HEIGHT/2 - 60, BLACK, GREEN, 0);
  ILI9341_Draw_Hollow_Rectangle(LCD_WIDTH/2 + 20  , LCD_HEIGHT/2 + 40, LCD_WIDTH/2 - 40 , LCD_HEIGHT/2 - 60,  DARKGREY);

  /*---------USB CDC Test-------------*/
  //sprintf((char*)buff, "pokus");
  //CDC_Transmit_FS((uint8_t*)buff, strlen((char*)buff));


  MyEEPROM.addItem((char*)"Red", 0, (uint32_t*)&pwm1_value_A);
  MyEEPROM.addItem((char*)"Green", 0, (uint32_t*)&pwm2_value_A);
  MyEEPROM.addItem((char*)"Blue", 0, (uint32_t*)&pwm3_value_A);
  MyEEPROM.addItem((char*)"Day", 1, (uint32_t*)&menu_Day);
  MyEEPROM.addItem((char*)"Month", 2, (uint32_t*)&menu_Month);
  MyEEPROM.addItem((char*)"Year", 3, (uint32_t*)&menu_Year);

  //sprintf((char*)buff,"%s %li ",MyEEPROM.GetItemText(3), MyEEPROM.GetItemValue(3));
  //ILI9341_WriteString(0, 165, (char*)buff, Font_11x18, RED, WHITE);

  enum {
	  M_TP, //Menu Top level ID  = 0
	  M_CL,		//Menu Colors ID  = 1
	  M_TM,		//Menu Time ID  = 2
	  M_DT,		//Menu Date ID = 3
	  M_SB,			//Menu Submenu3 ID = 4

  };

  /*---------LCD Menu init-------------*/
  LCDMenu.addItem(M_TP,M_TP,M_CL,(char*)"R G B", 			ITEMTYPE_GOTOSUBMENU);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Back..", 		ITEMTYPE_EXITSUBMENU);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Red A", 			ITEMTYPE_NORMAL, 	&pwm1_value_A, 	Menu_Red_Callback);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Green A",			ITEMTYPE_NORMAL, 	&pwm2_value_A, 	Menu_Green_Callback);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Blue A",			ITEMTYPE_NORMAL, 	&pwm3_value_A, 	Menu_Blue_Callback);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Red B", 			ITEMTYPE_NORMAL, 	&pwm1_value_B);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Green B",			ITEMTYPE_NORMAL, 	&pwm2_value_B);
	  LCDMenu.addItem(M_TP,M_CL,M_TP,(char*)"Blue B",			ITEMTYPE_NORMAL, 	&pwm3_value_B);
  LCDMenu.addItem(M_TP,M_TP,M_TM,(char*)"Set time", 		ITEMTYPE_GOTOSUBMENU);
  	  LCDMenu.addItem(M_TP,M_TM,M_TP,(char*)"Back..", 			ITEMTYPE_EXITSUBMENU);
  	  LCDMenu.addItem(M_TP,M_TM,M_TP,(char*)"Hours", 			ITEMTYPE_NORMAL, 	&menu_Hours, 	Menu_SetHours_Callback);
  	  LCDMenu.addItem(M_TP,M_TM,M_TP,(char*)"Minutes", 			ITEMTYPE_NORMAL, 	&menu_Minutes, 	Menu_SetMinutes_Callback);
  	  LCDMenu.addItem(M_TP,M_TM,M_TP,(char*)"Seconds", 			ITEMTYPE_NORMAL, 	&menu_Seconds, 	Menu_SetSeconds_Callback);
  LCDMenu.addItem(M_TP,M_TP,3,(char*)"Set date", 			ITEMTYPE_GOTOSUBMENU);
  	  LCDMenu.addItem(M_TP,3,M_TP,(char*)"Back..", 				ITEMTYPE_EXITSUBMENU);
  	  LCDMenu.addItem(M_TP,3,M_TP,(char*)"Day", 				ITEMTYPE_NORMAL, 		&menu_Day, 	Menu_SetDay_Callback);
  	  LCDMenu.addItem(M_TP,3,M_TP,(char*)"Month", 				ITEMTYPE_NORMAL, 	&menu_Month, 	Menu_SetMonth_Callback);
  	  LCDMenu.addItem(M_TP,3,M_TP,(char*)"Year", 				ITEMTYPE_NORMAL, 	&menu_Year, 	Menu_SetYear_Callback);
      LCDMenu.addItem(M_TP,3,4,(char*)"To submenu3", 			ITEMTYPE_GOTOSUBMENU);
  	  	  LCDMenu.addItem(M_TM,4,M_TP,(char*)"Back..", 				ITEMTYPE_EXITSUBMENU);
  	  	  LCDMenu.addItem(M_TM,4,M_TP,(char*)"Submenu3.1", 			ITEMTYPE_NORMAL, 	&pwm1_value_A);
  	  	  LCDMenu.addItem(M_TM,4,M_TP,(char*)"Submenu3.2", 			ITEMTYPE_NORMAL, 	&pwm2_value_A);
  	  	  LCDMenu.addItem(M_TM,4,M_TP,(char*)"Submenu3.3", 			ITEMTYPE_NORMAL, 	&pwm3_value_A);
  LCDMenu.addItem(M_TP,M_TP,M_TP,(char*)"Store settings", 	ITEMTYPE_CALLBACK_ONLY,	 	 NULL, 	Menu_StoreSettings_Callback);
  LCDMenu.addItem(M_TP,M_TP,M_TP,(char*)"Load settings", 	ITEMTYPE_CALLBACK_ONLY, 	 NULL, 	Menu_LoadSettings_Callback);

  LCDMenu.DrawMenu();

  /*---------Ring Buffer init-------------*/
  ringbuf_init(&ringBuffer, &ringData, USART_RXBUFFERSIZE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  /*--- LCD Menu process ---*/
	  LCDMenu.ProcessMenu();

	  /*--- USART bufer process ---*/
	  while(ringbuf_elements(&ringBuffer) != 0) {

		  uint8_t c = ringbuf_get(&ringBuffer);

		  uint8_t commandLen = USART_Extract_Commands(c, USARTcommandBuffer,  USART_CMDBUFFERSIZE);
		  if(commandLen !=0 ) {
			  USART_Process_Commands((char*)USARTcommandBuffer,commandLen );
			  HAL_UART_Transmit(&huart3, (uint8_t*)USARTcommandBuffer,commandLen+1, HAL_MAX_DELAY);
		  } else {
			  //HAL_UART_Transmit(&huart3, (uint8_t*)&c,1, HAL_MAX_DELAY);
		  }
	  }

	/* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		  if(timeCnt % 500 == 0) {
			  //==========BEGIN 1 second===========
			  DateTime.FillDate();
			  DateTime.FillTime();
 
			  //Print Date
			  DateTime.DateForPrint((char*)buff);
			  ILI9341_WriteString(0, 2,(char*)buff, Font_11x18, DARKGREY, NAVY);
			  //Print Time
			  DateTime.TimeForPrint((char*)buff);
			  ILI9341_WriteString(LCD_WIDTH-90, 2, (char*)buff, Font_11x18, DARKGREY, NAVY);			  //Serial output
			  //Print Time to serial

			  DateTime.TimeForPrintLn((char*)buff);

			  CDC_Transmit_FS((uint8_t*)buff, strlen((char*)buff));

			  //HAL_UART_Transmit(&huart3, (uint8_t*)buff, strlen((char*)buff), 10);
			  //sprintf((char*)buff, "Time Counter:%i  ", timeCnt );
			  //ILI9341_WriteString(10, 75, (char*)buff, Font_7x10, WHITE, BLACK);
			  //=========gradient=======
			  /*for(uint16_t i = 0; i <= (320); i++)
			  {
				  uint16_t RGB = ((2560/320)*i)/10;
			  	  ILI9341_Draw_Rectangle(i, 200, 1, 40, ASSEMBLE_RGB(0, 256-RGB, RGB));
			  }*/
			  //==========END===========

		  }

		  if(timeCnt % 250 == 0) {
			  //==========BEGIN 0.5 sec===========

			  sprintf((char*)buff, "timeCnt: %i\n", timeCnt );

			  //sprintf((char*)buff, "ENCODER: %i     ", __HAL_TIM_GET_COUNTER(&htim4) );
			  //ILI9341_WriteString(160+10, 75, (char*)buff, Font_7x10, ORANGE, BLACK);
			  //CDC_Transmit_FS((uint8_t*)buff, strlen((char*)buff));
			  //sprintf((char*)buff, "Time Counter:%i\n", timeCnt );
			  //sprintf((char*)buff, "R:%i G:%i B:%i\n", (int16_t) pwm1_value, (int16_t) pwm1_value, (int16_t) pwm2_value);

			  //sprintf((char*)buff, " #: %i\n", ringbuf_elements(&ringBuffer));

			  ILI9341_LogToLCD(buff);

			  //==========END 0.5 sec===========
		  }

		  if(timeCnt % 50 == 0) {
			  //==========BEGIN 0.1 sec===========

			  if(btnFlag == USER_BUTTON_ENTER && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0) {
				  //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
				  //TIM4->CNT = 0;
				  LCDMenu.CheckButtons(btnFlag);
				  btnFlag = USER_BUTTON_NONE;
			  }

			  //==========END 0.1 sec===========
		  }

		  //==========BEGIN 1 ms===========

		  //PWM RED
		  if(pwm1_value_B < pwm1_value_A && pwm1_value_B < 500) {
			  pwm1_value_B+=1;
			  pwm1_setvalue(pwm1_value_B);
		  } else if(pwm1_value_B > pwm1_value_A && pwm1_value_B > 0) {
			  pwm1_value_B-=1;
			  pwm1_setvalue(pwm1_value_B);
		  }
		  //PWM GREEN
		  if(pwm2_value_B < pwm2_value_A && pwm2_value_B < 500) {
			  pwm2_value_B+=1;
			  pwm2_setvalue(pwm2_value_B);
		  } else if(pwm2_value_B > pwm2_value_A && pwm2_value_B > 0) {
			  pwm2_value_B-=1;
			  pwm2_setvalue(pwm2_value_B);
		  }
		  //PWM BLUE
		  if(pwm3_value_B < pwm3_value_A && pwm3_value_B < 500) {
			  pwm3_value_B+=1;
			  pwm3_setvalue(pwm3_value_B);
		  } else if(pwm3_value_B > pwm3_value_A && pwm3_value_B > 0) {
			  pwm3_value_B-=1;
			  pwm3_setvalue(pwm3_value_B);
		  }


		  //==========END 1 ms===========
		  HAL_Delay(1);
		  timeCnt++;


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 500;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MY_LED_GPIO_Port, MY_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : MY_BTN_Pin */
  GPIO_InitStruct.Pin = MY_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MY_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MY_LED_Pin */
  GPIO_InitStruct.Pin = MY_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MY_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 4 */
void pwm1_setvalue(uint16_t value)
{
	if(logaritmize(&value, 500, 500)) {
		TIM1->CCR1 = (uint32_t)value;
	}
}
void pwm2_setvalue(uint16_t value)
{
	if(logaritmize(&value, 500, 500)) {
		TIM1->CCR2 = (uint32_t)value;
	}
}
void pwm3_setvalue(uint16_t value)
{
	if(logaritmize(&value, 500, 500)) {
		TIM1->CCR3 = (uint32_t)value;
	}
}


void SetMyDay(uint8_t val) {
	DateTime.SetDay(val);
}

void SetMyMonth(uint8_t val) {
	DateTime.SetMonth(val);
}

void SetMyYear(uint8_t val) {
	DateTime.SetYear(val);
}

void SetMyHours(uint8_t val) {
	DateTime.SetHours(val);
	  //EE_WriteVariable(VirtAddVarTab[0], 717);
	  //EE_WriteVariable(VirtAddVarTab[1], 818);
}

void SetMyMinutes(uint8_t val) {
	DateTime.SetMinutes(val);
}

void SetMySeconds(uint8_t val) {
	DateTime.SetSeconds(val);
}

void Interrupt_EncoderChange(){
	static int16_t previousEncoderCnt = 0;
	//uint8_t encoderDirection = 0;

	int16_t encoderCnt = 0;
	uint8_t btnState = USER_BUTTON_NONE;
	encoderCnt = ((int16_t)__HAL_TIM_GET_COUNTER(&htim4))/2;

	if(encoderCnt > previousEncoderCnt) {
		btnState = USER_BUTTON_UP;
	} else if(encoderCnt < previousEncoderCnt) {
		btnState = USER_BUTTON_DOWN;
	} else {
		btnState = USER_BUTTON_NONE;
	}

	LCDMenu.CheckButtons(btnState);

	previousEncoderCnt = encoderCnt;
}

void Interrupt_ButtonPressed() {
	btnFlag = USER_BUTTON_ENTER;
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
}



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
	ILI9341_Draw_Hollow_Rectangle_Coord(posx, posy, posx + width, posy + height, BLUE);
}
void LCDMENU_RectangleAround_Active(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height) {
	ILI9341_Draw_Hollow_Rectangle_Coord(posx, posy, posx + width, posy + height, GREEN);
}

void Menu_Red_Callback(int param) {
	pwm1_value_A = pwm1_value_A + param * 100;
	if(pwm1_value_A > 499) pwm1_value_A = 500;
	if(pwm1_value_A < 0) pwm1_value_A = 0;
}

void Menu_Green_Callback(int param) {
	pwm2_value_A = pwm2_value_A + param * 100;
	if(pwm2_value_A > 499) pwm2_value_A = 500;
	if(pwm2_value_A < 0) pwm2_value_A = 0;
}
void Menu_Blue_Callback(int param) {
	pwm3_value_A = pwm3_value_A + param * 100;
	if(pwm3_value_A > 499) pwm3_value_A = 500;
	if(pwm3_value_A < 0) pwm3_value_A = 0;
}

void Menu_SetHours_Callback(int param) {
	menu_Hours = DateTime.GetHours();
	menu_Hours = menu_Hours + param;
	if(menu_Hours < 0) menu_Hours = 0;
	if(menu_Hours > 23) menu_Hours = 23;
	DateTime.SetHours((uint8_t)menu_Hours);
}

void Menu_SetMinutes_Callback(int param) {
	menu_Minutes = DateTime.GetMinutes();
	menu_Minutes = menu_Minutes + param;
	if(menu_Minutes < 0) menu_Minutes = 0;
	if(menu_Minutes > 60) menu_Minutes = 23;
	DateTime.SetMinutes((uint8_t)menu_Minutes);
}

void Menu_SetSeconds_Callback(int param) {
	menu_Seconds = DateTime.GetSeconds();
	menu_Seconds = menu_Seconds + param;
	if(menu_Seconds < 0) menu_Seconds = 0;
	if(menu_Seconds > 60) menu_Seconds = 23;
	DateTime.SetSeconds((uint8_t)menu_Seconds);
}

void Menu_SetDay_Callback(int param) {
	menu_Day = DateTime.GetDay();
	menu_Day = menu_Day + param;
	if(menu_Day < 0) menu_Day = 0;
	if(menu_Day > 31) menu_Day = 23;
	DateTime.SetDay((uint8_t)menu_Day);
}

void Menu_SetMonth_Callback(int param) {
	menu_Month = DateTime.GetMonth();
	menu_Month = menu_Month + param;
	if(menu_Month < 0) menu_Month = 0;
	if(menu_Month > 12) menu_Month = 12;
	DateTime.SetMonth((uint8_t)menu_Month);
}

void Menu_SetYear_Callback(int param) {
	menu_Year = DateTime.GetYear();
	menu_Year = menu_Year + param;
	if(menu_Year < 0) menu_Month = 0;
	if(menu_Year > 99) menu_Year = 99;
	DateTime.SetYear((uint8_t)menu_Year);
}



void Menu_StoreSettings_Callback(int param) {

	//Brightness
	MyEEPROM.SaveItemVariableToEEPROM(0);
	//Day
	menu_Day = DateTime.GetDay();
	MyEEPROM.SaveItemVariableToEEPROM(1);
	//Month
	menu_Month = DateTime.GetMonth();
	MyEEPROM.SaveItemVariableToEEPROM(2);
	//Year
	menu_Year = DateTime.GetYear();
	MyEEPROM.SaveItemVariableToEEPROM(3);

	ILI9341_WriteString(5, 80, (char*)"Settings stored", Font_11x18, RED, WHITE);
}


uint8_t logaritmize(uint16_t* inputValue, uint16_t inputMax, uint16_t outputMax) {

	double logarithm = 0.0;
	uint16_t value = *inputValue;
	if(value <= inputMax && value >= 0) {
	    logarithm = outputMax - (outputMax * log10(500 - value + 1) / log10(500));
	    if(logarithm < 0) {
	        logarithm = 0;
	    }
	    logarithm = round(logarithm);
	    *inputValue = (uint16_t)logarithm;
	    return 1;
	} else {
		return 0;
	}
    //Vystupni_maximum - (Vystupni_maximum * LOG(inputMax - value + 1)  /  LOG(inputMax))
}


void Menu_LoadSettings_Callback(int param) {

	//Brightness
	MyEEPROM.UpdateItemVariableFromEEPROM(0);
	//Day
	MyEEPROM.UpdateItemVariableFromEEPROM(1);
	DateTime.SetDay(menu_Day);
	//Month
	MyEEPROM.UpdateItemVariableFromEEPROM(2);
	DateTime.SetMonth(menu_Month);
	//Year
	MyEEPROM.UpdateItemVariableFromEEPROM(3);
	DateTime.SetYear(menu_Year);

	ILI9341_WriteString(5, 80, (char*)"Settings Loaded", Font_11x18, RED, WHITE);
}


/* - Makes input range logaritmic (0-50-100 to 0-10-100)
 * - Trasposes input range to outputMax (0-50-100 to 0-100-1000)
 * returns 0 if inputValue is < 0 or > inputMax
 * returns 1 if inputValue is OK
 * inputValue - pointer to input value
 * inputMax - maximum of input range
 * outputMax - maximum of output range
 */
void LCD_DrawSpectrum(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t brightness) {

    float c = 0.0f;
    uint16_t red, green, blue = 0;
    float fIncrement = 1.0 / width;
    uint16_t sixTimes = brightness * 6; //1530=255*6

	for(uint16_t i = x; i <= (x + width); i++) {

		if(c >= 0 && c <= (1/6.f)){
			red = brightness;
			green = sixTimes * c;
			blue = 0;
		} else if( c > (1/6.f) && c <= (1/3.f) ){
			red = brightness - (sixTimes * (c - 1/6.f));
			green = brightness;
			blue = 0;
		} else if( c > (1/3.f) && c <= (1/2.f)){
			red = 0;
			green = brightness;
			blue = sixTimes * (c - 1/3.f);
		} else if(c > (1/2.f) && c <= (2/3.f)) {
			red = 0;
			green = brightness - ((c - 0.5f) * sixTimes);
			blue = brightness;
		} else if( c > (2/3.f) && c <= (5/6.f) ){
			red = (c - (2/3.f)) * sixTimes;
			green = 0;
			blue = brightness;
		} else if(c > (5/6.f) && c <= 1 ){
			red = brightness;
			green = 0;
			blue = brightness - ((c - (5/6.f)) * sixTimes);
		}

		ILI9341_Draw_Vertical_Line( i, y, height, ASSEMBLE_RGB(red, green, blue));
		//HAL_Delay(1);

	  c += fIncrement;
  }

  c=0.0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{

	if (UartHandle->Instance == USART3) {
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
		//USART_Extract_Commands(receiveBuffer, USART_RXBUFFERSIZE);
		//if (HAL_UART_Receive_IT(&huart3, receiveBuffer, USART_RXBUFFERSIZE) != HAL_OK){
	    //    Error_Handler();
		//}
	}
}




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

