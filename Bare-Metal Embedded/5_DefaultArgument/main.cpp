#include "uart.h"


int  add_tripple(int a, int b, int c=0){

 return a+b+c;
	
}
int main(){
	
	  uart_init();

	 printf("The sum is %d\r\n",add_tripple(50,50));


	while(1){

	}
}

