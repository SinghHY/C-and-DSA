#include "uart.h"



void swapper(int a, int b){

	 int tmp;
	tmp = a;
	a=b;
	b=tmp;
	
	printf("a : %d ,  b : %d\n\r",a,b);


}
int main(){
	
	  uart_init();
	
	int num1 =45;
	int num2 = 67;
	
	swapper(num1,num2);
	
	printf("num1 : %d ,  num2 : %d\n\r",num1,num2);




	while(1){

	}
}

