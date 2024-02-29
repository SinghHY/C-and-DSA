#include "uart.h"
//#include <iostream>


class Led
{
	private:
		int diameter;
		float voltage;
		float current;
	
  public :


    /*Accessors*/	
	  int getDiameter(){
		  return diameter;
		}		
    
    float getVoltage(){
		  return voltage;
		}			
		
		float getCurrent(){
		  return current;
		}
		
		 /*Mutator*/
		
		void setDiameter(float d){
		   if(d>0)
				 diameter =d;
			 else
				 diameter =0;
		}
		
		void setVoltage(float v){
		   voltage = v;
		}
		
		void setCurrent(float c){
		   current = c;
		}
		
		float power(){
		 return voltage*current;
		}
};


int main(){

 uart_init();
	
	Led green_led; //Creating an Led object
	Led *pt_green;
	
	pt_green = &green_led;
	
	pt_green->setCurrent(0.8);
	pt_green->setVoltage(3.3);
	green_led.setDiameter(2);
	
	printf("The power of green led is %f ",green_led.power());
	
	while(1){

	}
}

