#include "uart.h"
#include <stdio.h>
#include "mcal_reg.h"
#include "spi.h"


int main(){

  spi_init();

	
	 while(1){
	   spi1_write('Y');
	 }
}


