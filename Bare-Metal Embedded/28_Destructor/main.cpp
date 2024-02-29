#include "uart.h"



class Led
{
	public:
		
		int * default_pattern; 

		Led(){
			
			printf("Led created \n\r");
			
      default_pattern =  new int[20];
		
		}
	
		~Led(){
     printf("Led destroyed \n\r");
     delete []default_pattern;		
		}
	
}	;


int main(){

  uart_init();
	
  Led * myLed2 =  new Led();
	
	printf("Shutdown successful\n\r");
	
	delete myLed2;


	
}

