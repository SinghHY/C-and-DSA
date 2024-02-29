#include "mcal_reg.h"



/*Procedure:
1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
2. Program the M bit in USART_CR1 to define the word length.
3. Program the number of stop bits in USART_CR2.
4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
place. Configure the DMA register as explained in multibuffer communication.
5. Select the desired baud rate using the USART_BRR register.
6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
for each data to be transmitted in case of single buffer.
8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
that the transmission of the last frame is complete. This is required for instance when
the USART is disabled or enters the Halt mode to avoid corrupting the last
transmission.*/


void uart_write(int ch);


int main()
{
	 /*Enable clock to GPIOA*/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<0)>::reg_or();
	
	 /*Enable clock to UART2*/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::apb1enr,(1U<<17)>::reg_or();
    /*Configure PA2 as UART2 TX*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0U<<4)>::reg_or();
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<5)>::reg_or();
	
  /*Set ALT type  to UART2*/ 
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_afrl,0x0700>::reg_or();
	
	/*Confidure USART2 baudrate 9600  @ 16MHz*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_brr,0x0683>::reg_set();
 
	/*Enable Tx,8-bit*/
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr1,0x008>::reg_set();
  /*1 stop bit*/
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr2,0x000>::reg_set();
  /*No flow control*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr3,0x000>::reg_set();
   /*Enable UART*/
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::uart2_cr1,0x2000>::reg_or();
	 

	while(1){
	
		 uart_write('Z');
		 for(int i=0;i<1800000;i++){}


	}
}

void uart_write(int ch){
 
	std::uint32_t _ch =  ch & 0xFF;
	
	/*Wait until TX buffer is empty*/
	while(!(mcal::reg::UART2->SR & 0x0080)){}
		mcal::reg::reg_access_dynamic<std::uint32_t,std::uint32_t>::reg_set(mcal::reg::uart2_dr,_ch);

}

