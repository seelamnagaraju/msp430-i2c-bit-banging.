
/********************************************************************************
Module      : I2C_SW
Description : I2C software using bit-banging.
 ********************************************************************************/

/*-----------------------------------------------------------------------------*/
/* Header inclusions */
/*-----------------------------------------------------------------------------*/
#include <msp430.h>
#include "i2c_gpio.h"

/*-----------------------------------------------------------------------------*/
/* Local Macro definitions */
/*-----------------------------------------------------------------------------*/
// Note: I2C SDA is open drain, this means we need to use INPUT for an SDA high logic.
// I.e. we can't just write digitalWrite(pin, HIGH) for the SDA.
#define SDA_H      { I2C_PxDIR &= ~SDA_PIN;  I2C_PxOUT |=  SDA_PIN; }
#define SDA_L      { I2C_PxDIR |=  SDA_PIN;  I2C_PxOUT &= ~SDA_PIN; }
#define SCL_H      { I2C_PxDIR &= ~SCL_PIN;  I2C_PxOUT |=  SCL_PIN; }  // set
#define SCL_L      { I2C_PxDIR |=  SCL_PIN;  I2C_PxOUT &= ~SCL_PIN; }  // clear

#define SDA_READ       Read_SDA()
#define I2C_DATA_IN    I2C_PxIN & SDA
#define iDelay         200
#define I2C_DELAY()     __delay_cycles(TIME_DELAY)

/*-----------------------------------------------------------------------------*/
/* Function prototypes */
/*-----------------------------------------------------------------------------*/
void start(void);
void stop(void);
unsigned char Read_SDA(void);
unsigned char send_byte(unsigned char c);
unsigned char receive_byte(unsigned char master_ack);
/*-----------------------------------------------------------------------------*/
/* Function implementations */
/*-----------------------------------------------------------------------------*/
void inline sleep_us(unsigned int uiDelay)
{
     while(uiDelay--);
}
/*--------------------------------------------------------------------------------
Function    : start
Purpose     : i2c bus start
Parameters  : None
Return      : None
--------------------------------------------------------------------------------*/
void I2C_Init(void)
{
    I2C_PxSEL  &= ~( SCL_PIN | SDA_PIN );   // I/O enabled
    I2C_PxSEL2 &= ~( SCL_PIN | SDA_PIN );
    sleep_us(iDelay);
    SCL_H
    SDA_H
    sleep_us(iDelay);
}
/*--------------------------------------------------------------------------------
Function    : start
Purpose     : i2c bus start
Parameters  : None
Return      : None
--------------------------------------------------------------------------------*/
void start(void)
{
    SDA_H
    sleep_us(iDelay);
    SCL_H
    sleep_us(iDelay);
    SDA_L
    sleep_us(iDelay);
    SCL_L
    sleep_us(iDelay);
}
/*--------------------------------------------------------------------------------
Function    : stop
Purpose     : i2c bus stop
Parameters  : None
Return      : None
--------------------------------------------------------------------------------*/
void stop(void)
{
    SDA_L
    sleep_us(iDelay);
    SCL_H
    sleep_us(iDelay);
    SDA_H
    sleep_us(iDelay);
    SCL_L
    sleep_us(iDelay);
}
/*--------------------------------------------------------------------------------
Function    : Read_SDA
Purpose     : Read bit to I2C bus
Parameters  : None
Return      : unsigned char
--------------------------------------------------------------------------------*/
unsigned char Read_SDA(void)
{
    I2C_PxDIR  &= ~SDA_PIN;
    return((I2C_PxIN & SDA_PIN) != 0);
}
/*--------------------------------------------------------------------------------
Function    : send_byte
Purpose     : Write a Byte to I2C bus
Parameters  : unsigned char Data
Return      : None
--------------------------------------------------------------------------------*/
unsigned char send_byte(unsigned char c)
{
  unsigned int i=0, mask=0x80, ack;

  for (i=0; i<8; i++) //transmits 8 bits
  {
    if(c & mask) //set data line accordingly(0 or 1)
      SDA_H //data high
    else
      SDA_L //data low

    SCL_H   //clock high
    sleep_us(iDelay);
    SCL_L   //clock low
    c <<= 1;  //shift mask
    sleep_us(iDelay);
  } // for i
  // read ack bit
  SDA_H  //release data line for acknowledge
  SCL_H  //send clock for acknowledge
  sleep_us(iDelay);
  ack=SDA_READ; //read data pin for acknowledge
  SCL_L  //clock low
  sleep_us(iDelay);
  SDA_L  // data low
  return (ack == 0) ? 1:0; // a low ACK bit means success
} /* send_byte() */
/*--------------------------------------------------------------------------------
Function    : receive_byte
Purpose     : Read a Byte to I2C bus
Parameters  : unsigned char
Return      : unsigned char
--------------------------------------------------------------------------------*/
//receives one byte of data from i2c bus
unsigned char receive_byte(unsigned char master_ack)
{
  unsigned char c=0,mask=0x80;
  SCL_H
  SDA_H
  do  //receive 8 bits
  {
     //sleep_us(iDelay);
     SCL_H //clock high
     sleep_us(iDelay);
     if(SDA_READ==1) //read data
      c |= mask;  //store data
     SCL_L  //clock low
     sleep_us(iDelay);
     mask/=2; //shift mask
  }while(mask>0);

  if(master_ack==1)
    SDA_H //don't acknowledge
  else
    SDA_L //acknowledge

  SCL_H //clock high
  sleep_us(iDelay);
  SCL_L //clock low
  sleep_us(iDelay);
  SDA_L //data low
  return c;
}

/*--------------------------------------------------------------------------------
Function    : write_i2c
Purpose     : Send a Byte to I2C Slave
--------------------------------------------------------------------------------*/
//writes one byte of data(c) to slave device(device_id) at given address(location)
unsigned char write_i2c(unsigned char device_id, unsigned char location, unsigned char c)
{
  unsigned char slave_ack;
  start();      //starts i2c bus
  slave_ack=send_byte(device_id); //select slave device
  send_byte(location); //send address location
  send_byte(c); //send data to i2c bus
  stop(); //stop i2c bus
  return slave_ack;
}

/*--------------------------------------------------------------------------------
Function    : receive_byte
Purpose     : Read a Byte to I2C Slave
--------------------------------------------------------------------------------*/
unsigned char read_i2c(unsigned char device_id,unsigned char location)
{
  unsigned char cData;
  start();   //starts i2c bus
  cData=send_byte(device_id); //select slave device
  send_byte(location);  //send address location
  stop(); //stop i2c bus
  start(); //starts i2c bus
  send_byte(device_id+1); //select slave device in read mode
  cData=receive_byte(1); //receive data from i2c bus
  stop(); //stop i2c bus
  return cData;
}

/*--------------------------------------------------------------------------------
Function    : multiByteWrite_i2c
Purpose     : Write n Byte to I2C bus
Parameters  : DevideAddr    - Devide Address
              Register      - Register Address
              Data          - Pointer to Data need to write
              nLength       - Number of Byte need to write
Return      : None
--------------------------------------------------------------------------------*/
//writes multi bytes data to slave device(device_id) at given address(location)
unsigned char multiByteWrite_i2c(unsigned char device_id, unsigned char location, unsigned char *pBuf, unsigned char len)
{
  unsigned char i,slave_ack;
  start();      //starts i2c bus
  slave_ack=send_byte(device_id); //select slave device
  send_byte(location); //send address location
  for (i=0; i<len; i++)
  {
       send_byte(pBuf[i]);  //send data to i2c bus
  }
  stop(); //stop i2c bus
  return slave_ack;
}
/*--------------------------------------------------------------------------------
Function    : multiByteRead_i2c
Purpose     : Read n Byte from I2C bus
Parameters  : DevideAddr    - Devide Address
              Register      - Register Address
              Buff          - Pointer to Buffer store value
              nLength       - Number of Byte need to read
Return      : None
--------------------------------------------------------------------------------*/
void multiByteRead_i2c(unsigned char device_id, unsigned char location, unsigned char *Buf, unsigned char len)
{
    int i;
    start();
    send_byte(device_id);
    send_byte(0x00);
    start();
    send_byte(device_id+1);
    for (i=0; i<len; i++)
    {
        if (i == 15)
           Buf[i] = receive_byte(1);
        else
           Buf[i] = receive_byte(0);
    }
    stop();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
unsigned char bin2bcd(unsigned char val)
{
  unsigned char bcdVal;
  bcdVal = (val + 6 * (val / 10));
  return bcdVal;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
unsigned char bcd2bin(unsigned char val)
{
  unsigned char bcdVal;
  bcdVal = (val - 6 * (val >> 4));
  return bcdVal;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
