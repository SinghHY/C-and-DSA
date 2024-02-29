#include "uart.h"
#include <stdio.h>
#include "systick.h"


int sysflag;

int main()
{

	uart2_init(); 

	systick_init_1sec();


	while(1){
	
		
	 		   sysflag  =get_count_flag();
  if(sysflag)	
	{
		printf("Systick Timeout just occured !\r\n");
	}
		
		



	}
}





