#include "mcal_reg.h"


/**@program :  Driver to toggle PA5**/

int main()
{
	/**1.Enable clock access to GPIOA**/
 	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::ahb1enr,(1U<<0)>::reg_or();
	
 /**2.Set PA5 as output pin**/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_moder,(1U<<10)>::reg_or();
	 
	
	while(1){
	
  /**3.Toggle PA5**/
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_bsrr,(1U<<5)>::reg_set();
	 for(int i=0;i<1800000;i++){}
	 mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::gpioa_bsrr,(1U<<21)>::reg_set();
	 for(int i=0;i<1800000;i++){}


	}
}

