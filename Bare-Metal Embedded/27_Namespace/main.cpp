#include "uart.h"


namespace Led{

	void diagnosticsReport(){
	 
		 printf("Led module is working within normal parameters \n\r");

	}
}
	
namespace Motor{
	
	void  diagnosticsReport(){
		
			 printf("Motor module is working within normal parameters \n\r");


	}
}

using namespace Led;
int main(){

  uart_init();
	
	
	Motor::diagnosticsReport();
	Led::diagnosticsReport();
	
	 diagnosticsReport();
	 diagnosticsReport();
	 diagnosticsReport();
	 diagnosticsReport();



	while(1){

	}
}

