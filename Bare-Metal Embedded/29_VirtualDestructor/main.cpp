#include "uart.h"



class Led
{
	public:
		

		 Led(){
			
			printf("Led created \n\r");
			
		
		}
	
		virtual  ~Led(){
     printf("Led destroyed \n\r");
		}
	
}	;

class PowerLed:public Led{

public:
		PowerLed()
    {
			printf("PowerLed is created \n\r");
		}
		
		~PowerLed()
		{
			printf("PwerLed is destroyed \n\r");
		}
	


};

int main(){

  uart_init();
	
	Led  *led_pt;
	
	led_pt =  new PowerLed();
	
	delete led_pt;
	


	
}

