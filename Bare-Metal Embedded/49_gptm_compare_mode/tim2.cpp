#include "tim2.h"
#include "mcal_reg.h"

#define TIM2   ((mcal::reg::TIM_TypeDef*)mcal::reg::tim2_base)

 /*Configure TIM2 for 1hz timeout*/
 
/*Sysclock  16Mhz  =  16 000 000*/

void tim2_init_1hz(void){

   //Enable clock to TIM2
	
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::apb1enr,(1U<<0)>::reg_or();
	
	/*16 000 000 /1600 = 10000*/
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_psc,(1600-1)>::reg_set();
	
  /*10000/10000 =1*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_arr,(10000-1)>::reg_set();
	
  //Clear timer counter 
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_cnt,0>::reg_set();
	//Enable Timer
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_cr1,(1U<<0)>::reg_set();


}

void tim2_ch1_compare_mode_init(void){
	 /*Enable clock for GPIOA*/
  mcal::reg::reg_access<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::ahb1enr,
                               0x01>::reg_or();


	/*Set PA5 to alternate functions*/
														 
	mcal::reg::reg_access<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::gpioa_moder,
                               (0U<<10)>::reg_or();
	
	mcal::reg::reg_access<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::gpioa_moder,
                               (1U<<11)>::reg_or();
		
	//Select AF1 i.e TIM2													 

	mcal::reg::reg_access<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::gpioa_afrl,
                               (1U<<20)>::reg_or();
															 
 //Enable clock to TIM2
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::apb1enr,(1U<<0)>::reg_or();
	/*16 000 000 /1600 = 10000*/
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_psc,(1600-1)>::reg_set();
	 /*10000/10000 =1*/
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_arr,(10000-1)>::reg_set();
	//Set output to toggle on match
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_ccmr1,(0x30)>::reg_set();
	//set match value
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_ccr1,0>::reg_set();
  //Enable CH5 compare mode
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_ccer,1>::reg_or();
 // Clear timer counter
  mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_cnt,0>::reg_set();
 //Enable TIM2
	mcal::reg::reg_access<std::uint32_t,std::uint32_t,mcal::reg::tim2_cr1,1>::reg_set();

}

void wait_for_timeout(void){
 
	 /*Wait until UIF is set*/
	 while(!(TIM2->SR &1)){}
	 /*Claer UIF*/
	 TIM2->SR &=~1;

}