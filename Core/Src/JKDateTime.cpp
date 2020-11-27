/*
 * JKDateTime.cpp
 *
 *  Created on: Oct 18, 2020
 *      Author: KlokockaJir
 */

#include "stdio.h"
#include "JKDateTime.h"


extern RTC_HandleTypeDef hrtc;

JKDateTime::JKDateTime()
{

}

void JKDateTime::FillDate()
{
	HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN); // Get Date
}


void JKDateTime::FillTime()
{
	HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN); // Get Date
}

uint8_t JKDateTime::GetDay()
{
	return rtcDate.Date;
}

uint8_t JKDateTime::GetMonth()
{
	return rtcDate.Month;
}

uint8_t JKDateTime::GetYear()
{
	return rtcDate.Year;
}

uint8_t JKDateTime::GetHours()
{
	return rtcTime.Hours;
}

uint8_t JKDateTime::GetMinutes()
{
	return rtcTime.Minutes;
}

uint8_t JKDateTime::GetSeconds()
{
	return rtcTime.Seconds;
}

void JKDateTime::DateForPrint(char* buffer) {
	uint8_t size = 9; //8 + one place for \0
	char text[] = "00.00.00";
	snprintf((char*)text, size, "%02d.%02d.%02d",GetDay(),GetMonth(),GetYear());

	for(uint8_t i =0; i<size;i++ ){
		buffer[i] = text[i];
	}
}

void JKDateTime::TimeForPrint(char* buffer) {
	uint8_t size = 9; //8 + one place for \0
	char text[] = "00:00:00";
	snprintf((char*)text, size, "%02d.%02d.%02d",GetHours(),GetMinutes(),GetSeconds());

	for(uint8_t i =0; i<size;i++ ){
		buffer[i] = text[i];
	}
}

void JKDateTime::TimeForPrintLn(char* buffer) {
	uint8_t size = 10; //9 + one place for \0
	char text[] = "00:00:00\n";
	snprintf((char*)text, size, "%02d.%02d.%02d\n",GetHours(),GetMinutes(),GetSeconds());

	for(uint8_t i =0; i<size;i++ ){
		buffer[i] = text[i];
	}
}


void JKDateTime::SetDay(uint8_t val) {
	rtcDate.Date = val;
	HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
	//HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, save);
}

void JKDateTime::SetMonth(uint8_t val) {
	rtcDate.Month = val;
	HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
}

void JKDateTime::SetYear(uint8_t val) {
	rtcDate.Year = val;
	HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
}

void JKDateTime::SetHours(uint8_t val) {
	rtcTime.Hours = val;
	HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
}

void JKDateTime::SetMinutes(uint8_t val) {
	rtcTime.Minutes = val;
	HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
}

void JKDateTime::SetSeconds(uint8_t val) {
	rtcTime.Seconds = val;
	HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
}




