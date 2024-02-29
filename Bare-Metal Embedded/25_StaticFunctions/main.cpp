#include "uart.h"


class Led
{
	private:
		float diameter;
	protected:
		float noOfpins;
	public:
		float current;
		float voltage;
	
	  static int totalCount;
	
	  Led(){
		
		  totalCount++;
		}
		
		static int getTotalLedCount(){
			
			return totalCount;
		
		}
	
};


int Led::totalCount =0;
	 
int main(){

  uart_init();
	
	Led led1,led2,led3;
	
	printf("Total number of LEDs created : %d\n\r",led1.getTotalLedCount());
	
	printf("Total number of LEDs created : %d\n\r",led2.getTotalLedCount());

	printf("Total number of LEDs created : %d\n\r",led3.getTotalLedCount());

  Led led4,led5,led6;
		printf("Total number of LEDs created : %d\n\r",led1.getTotalLedCount());

	while(1){

	}
}

