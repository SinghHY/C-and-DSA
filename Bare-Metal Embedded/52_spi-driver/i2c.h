#ifndef __I2C_H__
#define __I2C_H__

#include <cstdint>
 void i2c_ds1307_rtc_Init(void);
 int  i2c_bytewrite(char saddr,char maddr,char data);
void i2c1_burstRead(char saddr,char maddr, int n, char * data);


#endif