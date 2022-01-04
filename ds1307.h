#ifndef __I2C_RTC_H
#define __I2C_RTC_H
#ifdef __cplusplus
extern "C" {
#endif

#include <msp430.h>

/*-----------------------------------------------------------------------------*/
/* Macro definitions  */
/*-----------------------------------------------------------------------------*/

typedef struct
{
    int tm_sec;   /* seconds after the minute, 0 to 60 */
    int tm_min;   /* minutes after the hour, 0 to 59 */
    int tm_hour;  /* hours since midnight, 0 to 23 */
    int tm_mday;  /* day of the month, 1 to 31 */
    int tm_mon;   /* months since January, 0 to 11 */
    int tm_year;  /* years since 1900 */
    int tm_wday;  /* days since Sunday, 0 to 6 */
}st_time;

extern st_time yh;
/*-----------------------------------------------------------------------------*/
/* Function prototypes  */
/*-----------------------------------------------------------------------------*/
void set_RTC(void);
void read_RTC(void);

//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif
//*****************************************************************************
//              @End of file
//*****************************************************************************
