#include "uart.h"
//#include <iostream>



int main(){
	

  uart_init();
	
	int num = 234;
	int &ref_alias = num;
	
  int number2;
	
	ref_alias++;
	

	printf("num is %d \r\n",num);
	printf("ref_alias is %d \r\n",ref_alias);

	


	
	while(1){

	}
}

