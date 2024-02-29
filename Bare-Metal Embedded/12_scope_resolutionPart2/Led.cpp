#include "Led.h"


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