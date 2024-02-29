#include "mcal_reg.h"


#define RCC_AHB1ENR  (*((volatile unsigned int *) 0x40023830)) 
	
//#define GPIOA_MODER  (*((volatile unsigned int *) 0x40020000)) 
//#define GPIOA_ODR    (*((volatile unsigned int *)  0x40020014)) 

class Led
{
	public:
		typedef std::uint32_t port_type;
	  typedef std::uint32_t bval_type;
	
	Led(const port_type p,bval_type b):port(p),bval(b)
	{
		     /*Disable pin*/
            *reinterpret_cast<volatile port_type *>(port)
							&= ~(1U<<bval);
						
					/*Set pin mode to output*/
					const port_type gpio_mode_reg =  port  - 0x14;
						*reinterpret_cast<volatile port_type *>(gpio_mode_reg) |= ( 1U<<(bval *2));
	}
	
	void toggle()const
	{
		*reinterpret_cast<volatile port_type *>(port) ^=(1U<<bval);
	}
	
	void pseudo_delayMs(){
	
   for(int i=0;i<190000;i++){}
	}
	
	private:
		const port_type port;
		const bval_type bval; 
	
};



int main()
{
	 RCC_AHB1ENR |= (1U<<0);  /*Enable clock to GPIOA*/
	
	 Led led5(mcal::reg::gpioa,mcal::reg::gpio_pin5);
	 
	 Led led6(mcal::reg::gpioa,mcal::reg::gpio_pin6);

	
	while(1){
	  
		led5.toggle();
		led6.toggle();
		led5.pseudo_delayMs();
	
	}
	
}