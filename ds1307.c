#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "ds1307.h"
#include "main.h"

#define DS1307_ADDRESS 0xD0  ///< I2C address for DS1307
#define DS1307_CONTROL 0x07 ///< Control register

#define YEAR       0x20 // 2020
#define MONTH      0x11
#define DAY        0x29
#define WEEKDAY    0x06
#define HOUR       0x10
#define MINIT      0x24
#define SEC        0x10

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const unsigned char clock[7] = {SEC, MINIT, HOUR, WEEKDAY, DAY, MONTH, YEAR};

st_time yh;
extern unsigned char RTC_Buffer[32];
extern unsigned char bcd2bin(unsigned char);

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void set_RTC(void)
{
    //printf("Set RTC Time....!");
    multiByteWrite_i2c(DS1307_ADDRESS, 0x00, clock, 7);
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void read_RTC(void)
{
      int add=0;
      yh.tm_sec = bcd2bin(read_i2c(DS1307_ADDRESS, add++) & 0x7F);
      yh.tm_min = bcd2bin(read_i2c(DS1307_ADDRESS, add++));
      yh.tm_hour = bcd2bin(read_i2c(DS1307_ADDRESS, add++));
      yh.tm_wday = bcd2bin(read_i2c(DS1307_ADDRESS, add++));
      yh.tm_mday = bcd2bin(read_i2c(DS1307_ADDRESS, add++));
      yh.tm_mon = bcd2bin(read_i2c(DS1307_ADDRESS, add++));
      yh.tm_year = bcd2bin(read_i2c(DS1307_ADDRESS, add)) + 2000U;
      sprintf(RTC_Buffer, "%04d/%02d/%02d %02d:%02d:%02d", yh.tm_year, yh.tm_mon , yh.tm_mday, yh.tm_hour, yh.tm_min, yh.tm_sec);
      return;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


