//***************************************************************************************
//***************************************************************************************
#include "main.h"
#include "i2c_gpio.h"
#include "ds1307.h"

#define AT24C04_ADDR   0xA0 // 0b1010000  EEPROM 0x50
#define MAX            16
#define FAILURE    -1
#define SUCCESS     0

const unsigned char *s="NAGARAJU\n";
unsigned char wrArray[MAX]={"123KRISHNA123456"}; //DaysOfTheWeek123  NAGARAJUNAGARAJU 1234567887654321
unsigned char rdArray[MAX];
unsigned char read_string[16];
unsigned char RTC_Buffer[20];

volatile unsigned int timerVal;
unsigned char eeprom_address=0x00, write_String[] = {"hello world"}, read_string[16];
void gpioInit(void);

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#if 0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_ISR (void)
{
    P1OUT ^= BIT0;                   // Toggle P1.0
}

#endif
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// next isr is working well
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    switch(__even_in_range(P1IV, P1IV_P1IFG7))
    {
        case P1IV_NONE : break;
        case P1IV_P1IFG0 : break;
        case P1IV_P1IFG1 :
                        P1OUT ^= BIT0;                   // Toggle P1.0
                        break;
        case P1IV_P1IFG3 : break;
        case P1IV_P1IFG4 : break;
        case P1IV_P1IFG5 : break;
        case P1IV_P1IFG6 : break;
        case P1IV_P1IFG7 : break;
    }
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//#define RTC_EN     1
#define EEPROM_EN    1

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void main(void) {
   // volatile unsigned int i;               // volatile to prevent optimization
    volatile unsigned char idata=0, add=0;

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
      P1DIR |= 0x01;                          // Set P1.0 to output direction

    //  gpioInit();
      I2C_Init();

      printf((char*)wrArray);

#if 0
    // Timer code
    TA0CCTL0 = CCIE;                   // CCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_0;   // SMCLK / 1 = 1 MHz / 1 = 1 MHz = 0.001 ms, upmode
    TA0CCR0 =  1000;    // 1ms
#endif
  //  _BIS_SR(GIE);          // Enter LPM0 w/ interrupt
  //  I2C_Gpio_Init();

#ifdef EEPROM_EN
  //  printf("Writing Data to EEPROM....!\n");
  multiByteWrite_i2c(AT24C04_ADDR, add, wrArray,16);
  for(add=0; add<16; add++)
  {
      //write_i2c(AT24C04_ADDR, add, wrArray[add]);
      //delay(10);
  }
#endif

#ifdef RTC_EN
  printf("Set RTC Time....!");
  set_RTC();
#endif

    for(;;)
    {

     #ifdef EEPROM_EN
        printf((const char*)"EEPROM Read Data: ");
        printf((char*)wrArray);
        multiByteRead_i2c(AT24C04_ADDR, add, rdArray, 16);
        printf((char*)rdArray);
      #endif

      #ifdef RTC_EN
         printf("RTC Data:");
         read_RTC();
      #endif

        LED0_OUT ^= LED0_PIN;  // P1OUT ^= 0x01; Toggle P1.0 using exclusive-OR
        swDelay(50000);
    }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void gpioInit(void)
{
    P1OUT &= 0x00;                  // Shut down everything
    P1DIR &= 0x00;
    P2OUT &= 0x00;
    P2DIR &= 0x00;
    P3OUT &= 0x00;
    P3DIR &= 0x00;
    P4OUT &= 0x00;
    P4DIR &= 0x00;
    P5OUT &= 0x00;
    P5DIR &= 0x00;
    P6OUT &= 0x00;
    P6DIR &= 0x00;
    P7OUT &= 0x00;
    P7DIR &= 0x00;
    P8OUT &= 0x00;
    P8DIR &= 0x00;
    P9OUT &= 0x00;
    P9DIR &= 0x00;
    P10OUT &= 0x00;
    P10DIR &= 0x00;
}
/*----------------------------------------------------------------------------------*/
//----------------------------------------------------------------------------------
void swDelay(volatile unsigned int ulDelay) {        // SW Delay
   if (ulDelay > 0) {
        do ulDelay--;
        while(ulDelay != 0);
   }
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
