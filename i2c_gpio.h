#ifndef __I2C_GPIO_H
#define __I2C_GPIO_H
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
#define I2C_PxSEL       P1SEL0
#define I2C_PxSEL2      P1SEL1
#define I2C_PxDIR       P1DIR
#define I2C_PxOUT       P1OUT
#define I2C_PxIN        P1IN

#define SCL_PIN         BIT7
#define SDA_PIN         BIT6  // BIT6

#define ACK             0x00
#define NACK            0x01

#define TIME_DELAY      100

//*****************************************************************************
void I2C_Init(void);
unsigned char write_i2c(unsigned char device_id, unsigned char location, unsigned char c);
unsigned char multiByteWrite_i2c(unsigned char device_id, unsigned char location, unsigned char *pBuf, unsigned char len);
unsigned char read_i2c(unsigned char device_id,unsigned char location);
void multiByteRead_i2c(unsigned char device_id, unsigned char location, unsigned char *Buf, unsigned char len);
unsigned char bin2bcd(unsigned char val);
unsigned char bcd2bin(unsigned char val);
void swDelay(volatile unsigned int ulDelay);
void inline sleep_us(unsigned int uiDelay);

//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif
//*****************************************************************************
//              @End of file
//*****************************************************************************
