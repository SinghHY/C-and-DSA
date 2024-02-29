#include "systick.h"
#include "mcal_reg.h"

#define SYSTICK   ((mcal::reg::SYSTICK_TypeDef*)mcal::reg::systick_base)

 /*sys clock at 16MHz*/
 


void  systick_init_1sec(void){

	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_load,(16000000-1)>::reg_set();
	
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_val,0>::reg_set();

  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_ctrl,5>::reg_set(); //enabl systick, use system clock, no interrupt

}

 int get_count_flag(void){

	
	return (SYSTICK->CTRL & 0x10000);

}
 
void systick_delayMs(std::uint32_t dly){

	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_load,(16000-1)>::reg_set();
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_val,0>::reg_set();
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_ctrl,5>::reg_set(); //enabl systick, use system clock, no interrupt

	for(int i=0;i<dly;i++){
		
	 /*Wit until count flag is set*/
		 while(!(SYSTICK->CTRL & 0x10000)){}
	}
	
	 /*stop timer by writing 0 to ctrl reg*/

	  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::systick_ctrl,0>::reg_set();

}