#include "uart.h"
//#include <iostream>


class Led
{
	private:
		int diameter;
		float voltage;
		float current;
	
  public :
		
	  /*Constructors*/
			Led();	
			Led(int d,float v,float c );
	

    /*Accessors*/	
	  int getDiameter();
		float getVoltage();
		float getCurrent();

		
		 /*Mutator*/
		
		void setDiameter(float d);	
		void setVoltage(float v);
		void setCurrent(float c);

		/*Facilitators*/
		float power();
	
	  /*Inspectors*/
	   int isOn();
		
		/*Destructor*/
		~Led();

};


Led::Led(){
			
			 diameter = 3;
			 voltage = 3.3;
			 current= 200.0;
		}
	Led::Led(int d,float v,float c ){
			  setDiameter(d);
				setCurrent(c);
				setVoltage(v);
			}

    /*Accessors*/	
int Led::getDiameter(){
		  return diameter;
		}		
    
 float Led::getVoltage(){
		  return voltage;
		}			
		
	float Led::getCurrent(){
		  return current;
		}
		
		 /*Mutator*/
		
	void Led::setDiameter(float d){
		   if(d>0)
				 diameter =d;
			 else
				 diameter =0;
		}
		
		void Led::setVoltage(float v){
		   voltage = v;
		}
		
		void Led::setCurrent(float c){
		   current = c;
		}
		
		float Led::power(){
		 return voltage*current;
	}

int main(){

 uart_init();
	
	Led  power_led;
	Led  indicator_led(5,5.0,100);

	printf("The power_led voltage is %f \r\n", power_led.getVoltage());
	printf("The indicator_led voltage is %f\r\n", indicator_led.getVoltage());

	while(1){

	}
}

