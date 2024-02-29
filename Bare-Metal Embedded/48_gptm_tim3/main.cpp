#include "uart.h"
#include <stdio.h>
#include "mcal_reg.h"

#include "tim3.h"
int main()
{

	uart2_init();
  tim3_init_1hz();	

	 /*Enable clock for GPIOA*/
  mcal::reg::reg_access<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::ahb1enr,
                               0x01>::reg_or();


		 /*Set PA5 to output*/
														 
	mcal::reg::reg_access<std::uint32_t,
                               std::uint32_t,
                               mcal::reg::gpioa_moder,
                               (1U<<10)>::reg_or();
	while(1){
	

		 printf("GPTM Timeout just occured !\r\n");

																	 
		 mcal::reg::reg_access<std::uint32_t,
																	 std::uint32_t,
																	 mcal::reg::gpioa_odr,
																	 5>::bit_not();

     wait_for_tim3timeout();


	}
}





