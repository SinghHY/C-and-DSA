#include "spi.h"
#include "mcal_reg.h"


#define SPI1 ((mcal::reg::SPI_TypeDef *)mcal::reg::spi1_base)

/*Configure SPI1 as master, software slave select, clock rate 1Mhz
*/
void spi_init(void){

//1. enable clock access to gpio
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<0)>::reg_or();
//2. enable clock access to spi1
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::apb2enr,(1U<<12)>::reg_or();
//3. set PA5 and PA7 as alternate pins
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0xCC00)>::reg_not();  
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0x8800)>::reg_or();  
//4. set PA5 and PA7 alternate function type as spi
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_afrl,(0xF0F00000)>::reg_not();  
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_afrl,(0x50500000)>::reg_or();  
//5. set PA4 as gpio output for SS
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0x300)>::reg_not();  
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(0x100)>::reg_or();  

//6. set spi baudrate and data frame
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::spi1_cr1,(0x31C)>::reg_set();  

	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::spi1_cr2,(0)>::reg_set();  

 //7.enable spi
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::spi1_cr1,(0x40)>::reg_set();  
 
}


void  spi1_write(unsigned char data ){
  
	//Wait until transfer buffer empty
	 while(!(SPI1->SR & 2)){}
		//assert slave select i.e. set pin low
		mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_bsrr,(0x00100000)>::reg_set();
		SPI1->DR  =  data;
		  
	 /*Wait for transmission done*/
	 while(!(SPI1->SR & 0x80)){}
	 
	 //deassert slave select i.e. set pin high
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_bsrr,(0x00000010)>::reg_set();

}