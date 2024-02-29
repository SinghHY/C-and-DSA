#include "uart.h"
//#include <iostream>


class Led
{
	private:
		int diameter;
		float voltage;
		float current;
	
  public :
			Led(){
			
			 diameter = 3;
			 voltage = 3.3;
			 current= 200.0;
				}
			Led(int d,float v,float c ){
			  setDiameter(d);
				setCurrent(c);
				setVoltage(v);
			}

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
	
	Led  power_led;
	Led  indicator_led(5,5.0,100);

	printf("The power_led voltage is %f \r\n", power_led.getVoltage());
	printf("The indicator_led voltage is %f\r\n", indicator_led.getVoltage());

	while(1){

	}
}

