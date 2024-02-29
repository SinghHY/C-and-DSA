#include "uart.h"

void uart_init(void){

	RCC->AHB1ENR |= 0x1; /*Enable GPIOA clock*/
	RCC->APB1ENR |= 0x20000; /*Enale USART2 clock*/
	
	/*Configure PA2,PA3 for USART2 TX, RX*/
	GPIOA->AFR[0] |= 0x7700;   /*7 =0111, alt7 for usart2*/
	GPIOA->MODER |= 0x00A0; /*enable alternate function for PA2,PA3*/
	
  USART2->BRR = 0x0683;  /*9600 baud @ 16Mhz*/
	USART2->CR1 = 0x000C;  /*enbale Tx,Rx, 8-bit data*/
	USART2->CR2 = 0x000;  /*1 stop bit*/
	USART2->CR3 = 0x000; /*no flow control*/
	USART2->CR1 |= 0x2000; /*enable USART2*/
	

}

int  uart_write(int ch){
	
	 while(!(USART2->SR & 0x0080)){} //wait until Tx empty
	 USART2->DR=(ch & 0xFF);
	return 1;
}
int uart_read(void){
	
	while(!(USART2->SR & 0x020)){} // 
		return USART2->DR;
}


namespace std{
//struct __FILE
//{  int handle;};

FILE __stdout;
FILE __stdin;
FILE __stderr;

 int fgetc(FILE *f){
    int c;
	 c = uart_read();
 if(c =='\r'){
   uart_write(c);
	 c = '\n';
  }
  uart_write(c);
  return c;	
 }

 
   int fputc(int c, FILE *stream){
	   return uart_write(c);
	 }
	 
	 int ferror(FILE *stream){
	  return 1;
	 }
	 
	
	 long int ftell(FILE *stream){
	 	  return 1;

	 }
	 int fclose(FILE *f){
	  return 1;
	 }
	 int fseek(FILE *f, long nPos, int nMode){
		 return 0;
	 }
	 
	 int fflush(FILE *f){
	  return 1;
	 }

}
