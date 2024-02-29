#include "mcal_reg.h"


/**@program : Control LED at PA5 with push button at PC13**/


std::int32_t button_state;

int main()
{
	/**.Enable clock access to GPIOA**/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<0)>::reg_or();
	
	/**.Enable clock access to GPIOC**/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<2)>::reg_or();

 /**.Set PA5 as output pin**/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<10)>::reg_or();
	 
 /**.Set PC13 as input pin**/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioc_moder,(0U<<26)>::reg_or();
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioc_moder,(0U<<27)>::reg_or();

	while(1){
	
	/**.Read PC11**/
	button_state =  mcal::reg::reg_access<std::uint32_t, std::uint32_t,mcal::reg::gpioc_idr,0U>::reg_get();
		
		
		if(button_state & 0x2000){ // 0b 0010 0000 0000 0000
			
	      mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_bsrr,(1U<<21)>::reg_set();
		}
		
		else{
			
	   mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_bsrr,(1U<<5)>::reg_set();
		}

	}
}

