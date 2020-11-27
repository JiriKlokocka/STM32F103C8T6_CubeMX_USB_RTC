/*
 * JKDateTime.h
 *
 *  Created on: Oct 18, 2020
 *      Author: KlokockaJir
 */

#ifndef INC_JKDATETIME_HPP_
#define INC_JKDATETIME_HPP_

#include "main.h"

class JKDateTime
{
	public:
		JKDateTime();

		RTC_DateTypeDef rtcDate;
		RTC_TimeTypeDef rtcTime;

		void FillDate();
		void FillTime();

		uint8_t GetDay();
		uint8_t GetMonth();
		uint8_t GetYear();

		uint8_t GetHours();
		uint8_t GetMinutes();
		uint8_t GetSeconds();
		void DateForPrint(char* buffer);
		void TimeForPrint(char* buffer);
		void TimeForPrintLn(char* buffer);

		void SetDay(uint8_t val);
		void SetMonth(uint8_t val);
		void SetYear(uint8_t val);
		void SetHours(uint8_t val);
		void SetMinutes(uint8_t val);
		void SetSeconds(uint8_t val);


	private:

};



#endif /* INC_JKDATETIME_HPP_ */
