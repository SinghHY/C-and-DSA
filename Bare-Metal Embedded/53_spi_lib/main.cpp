

#include "stm32f4xx_hal_spi.h"


SPI_HandleTypeDef hspi1;


uint8_t tx_buffer[10] = {10,20,30,40,50,60,70,80,90,100};
uint8_t rx_buffer[10];

int  main()
{
	
	Spi spi1(&hspi1,SPI1,SPI_MODE_MASTER,SPI_DIRECTION_2LINES,SPI_DATASIZE_8BIT,
					SPI_POLARITY_HIGH,SPI_PHASE_2EDGE,SPI_NSS_SOFT,SPI_BAUDRATEPRESCALER_64,
					SPI_FIRSTBIT_MSB,SPI_TIMODE_DISABLED,SPI_CRCCALCULATION_DISABLE,0);
	
  
	spi1.SPI_TransmitReceive(&hspi1,tx_buffer,rx_buffer,10,100);
	
	while(1){
	
	}
	
	
}