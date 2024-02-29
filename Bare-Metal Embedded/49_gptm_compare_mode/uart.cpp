#include "uart.h"
#include "mcal_reg.h"
#include <stdio.h>

#define UART2                ((mcal::reg::USART_TypeDef *) mcal::reg::uart2_base)

/*
search on google : 
"Redefining low-level library functions 
to enable direct use of high-level library 
functions in the C library"

*/
void uart2_init(void)
{
	
		 /*Enable clock to GPIOA*/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<0)>::reg_or();
	
	 /*Enable clock to UART2*/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::apb1enr,(1U<<17)>::reg_or();
	
  /*Configure PA2,PA3 as  ALT pin UART2 TX,RX*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0U<<6)>::reg_or();
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<7)>::reg_or();
	
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0U<<4)>::reg_or();
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<5)>::reg_or();
	
  /*Set ALT type  to UART2 for PA3 and PA2*/  
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_afrl,0x7700>::reg_or();
	
	/*Confidure USART2 baudrate 9600  @ 16MHz*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_brr,0x0683>::reg_set();
 
	/*Enable Rx,8-bit*/
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr1,0x00C>::reg_set();
	
  /*1 stop bit*/
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr2,0x000>::reg_set();
  /*No flow control*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr3,0x000>::reg_set();
	
   /*Enable UART*/
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr1,0x2000>::reg_or();

}

int uart2_read(void){
	
	char rcv_ch;

	
	/*wait until char arrives*/
	while(!(UART2->SR) & 0x0020) {}   //0010 0000  
	
		rcv_ch  =  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_dr,0U>::reg_get();
  
		return rcv_ch;
}


int uart2_write(int ch){
 
	std::uint32_t _ch =  ch & 0xFF;
	
	/*Wait until TX buffer is empty*/
	while(!(UART2->SR & 0x0080)){}
		mcal::reg::reg_access_dynamic<std::uint32_t,std::uint32_t>::reg_set(mcal::reg::uart2_dr,_ch);

		 return 1;
}


namespace std {
	
  struct __FILE
  {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
  };
  FILE __stdout;
  FILE __stdin;
  FILE __stderr;
  int fgetc(FILE *f)
  {
    /* Your implementation of fgetc(). */
		
		int c;
		
		 c = uart2_read();
		
		 /*if '\r' after it is echoed, a '\n'is appended*/
		 if(c == '\r'){
		    uart2_write(c); /*echo*/
			 c  ='\n';
			 
		 }
		 
		 uart2_write(c); /*echo*/
    return c;
		 
  }
  int fputc(int c, FILE *stream)
  {
    /* Your implementation of fputc(). */
		
		 return uart2_write(c);
  }
  int ferror(FILE *stream)
  {
    /* Your implementation of ferror(). */
		return 0;
  }
  long int ftell(FILE *stream)
  {
    /* Your implementation of ftell(). */
		    return 0;

  }
  int fclose(FILE *f)
  {
    /* Your implementation of fclose(). */
    return 0;
  }
  int fseek(FILE *f, long nPos, int nMode)
  {
    /* Your implementation of fseek(). */
    return 0;
  }
  int fflush(FILE *f)
  {
    /* Your implementation of fflush(). */    
    return 0;
  }
}
