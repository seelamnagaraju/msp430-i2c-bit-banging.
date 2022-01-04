#ifndef __MAIN_H
#define __MAIN_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <msp430.h>

//*****************************************************************************
#define LED0_OUT      P1OUT
#define LED0_DIR      P1DIR
#define LED0_PIN      BIT1

#define LED0_ON       LED0_DIR &= ~LED0_PIN; // LED0_OUT |=  LED0_PIN; }
#define LED0_OFF      LED0_DIR |=  LED0_PIN; // LED0_OUT &=~ LED0_PIN; }

//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif
//*****************************************************************************
//              @End of file
//*****************************************************************************
