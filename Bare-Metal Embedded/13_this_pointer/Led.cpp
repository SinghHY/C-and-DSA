#include "Led.h"



Led::Led(){
			
			 this->diameter = 3;
			 this->voltage = 3.3;
			 this->current= 200.0;
		}
Led::Led(int diameter,float voltage,float current ){
			  setDiameter(diameter);
				setCurrent(current);
				setVoltage(voltage);
			}

    /*Accessors*/	
int Led::getDiameter(){
		  return this->diameter;
		}		
    
 float Led::getVoltage(){
		  return this->voltage;
		}			
		
	float Led::getCurrent(){
		  return this->current;
		}
		
		 /*Mutator*/
		
	void Led::setDiameter(float diameter){
		   if(diameter>0)
				 this->diameter =diameter;
			 else
				 this->diameter =0;
		}
		
		void Led::setVoltage(float voltage){
		   this->voltage = voltage;
		}
		
		void Led::setCurrent(float current){
		   this->current = current;
		}
		
		float Led::power(){
		 return this->voltage*this->current;
	}
			