#include "uart.h"
#include "Led.h"


struct UniqueLED
{
		int diameter;
		float voltage;
		float current;
			
		float power(){
		 return this->voltage*this->current;
	}
};

int main(){

 uart_init();
	
	UniqueLED  power_led;
   power_led.current =200;
	power_led.voltage = 3.3;
	power_led.diameter =5;
	
	printf("The power is %f ",power_led.power());

	while(1){

	}
}

