#include "uart.h"
#include "Led.h"




int main(){

 uart_init();
	
	Led  power_led;
	Led  indicator_led(5,5.0,100);

	printf("The power_led voltage is %f \r\n", power_led.getVoltage());
	printf("The indicator_led voltage is %f\r\n", indicator_led.getVoltage());

	while(1){

	}
}

