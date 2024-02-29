#include "adc.h"
#include "mcal_reg.h"
#define ADC1  ((mcal::reg:: ADC_TypeDef*)mcal::reg::adc1_base)

void adc1_init(void){
  
	//Enable clock access to PA1
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<0)>::reg_or();

	//Configure PA1 as analog pin
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<2)>::reg_or();
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<3)>::reg_or();

	//Enable clock access to ADC1
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::apb2enr,(1U<<8)>::reg_or();
	
	//SW Trigger
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::adc1_cr2,0x00>::reg_set();
  
	//Conversion sequence starts at ch1
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::adc1_sqr3,(1U<<0)>::reg_set();
	
  //Conversion seqeunce length 1
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::adc1_sqr1,0x00>::reg_set();
	
	//Enable ADC
		mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::adc1_cr2,(1U<<0)>::reg_or();
}


std::uint32_t adc_getValue(void){
//Start a conversion
mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::adc1_cr2,(1U<<30)>::reg_or();

//Wait for conversion completion
	while(!(ADC1->SR) & (0x02)){}
	
//Read value from data register
return mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::adc1_dr,0>::reg_get();

}