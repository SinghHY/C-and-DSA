#include "uart.h"
#include <stdio.h>
#include "mcal_reg.h"

#include "adc.h"

std::uint32_t sensorVal;

int main()
{
    adc1_init();

	 while(1){
	
    sensorVal  = adc_getValue();


	}
}





