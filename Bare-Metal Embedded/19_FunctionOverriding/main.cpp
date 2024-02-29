#include "uart.h"
//#include <iostream>


class Led
{
	private:
		int diameter;
		float voltage;
		float current;
	
	protected:
		int noOfpins;
	
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
		
		friend	void print_led_attributes(Led led);


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

	Led::~Led(){
	
	  printf("Led object destroyed !");
	}

	 void print_led_attributes(Led led){
	   printf("The led diameter is %d \n\r",led.diameter);
		 printf("The led current is %f \n\r",led.current);
		 printf("The led voltage is %f \n\r",led.voltage);

	 }
	 
	 
	class MedicalLed:public Led{
		
		private:
			int wavelength;
		
		public:
			MedicalLed(float d,float v, float c, float w, int p){
				
				setCurrent(c);
				setDiameter(d);
				setVoltage(v);
				
				wavelength =  w;
				noOfpins = p;
				
			}
			
			/*Mutators*/
			void setWavelength(int w){
			
			  wavelength = w;
			}
			
			/*Accessors*/
			int getWavelength(){
			  return wavelength;
			}
				
		  /*Facilitators*/
 			 float m_factor(){
			 
			   return (getCurrent()*getDiameter()*wavelength)/100;
			 }
			 
			 void getLedType(){
			 
			  if(noOfpins >3){
				   printf("This is a surface mount LED \n\r");
				}
				else{
				 printf("This is a through hole LED");
			 }
		 }
			
    float getVoltage(){
		  printf("This  LED has no voltage to show \n\r");
		}			
		 
	 
	 };
	 
int main(){

  uart_init();
	
  MedicalLed *mled_pt;
	mled_pt = new MedicalLed(2,5.5,400,890,4);
	
	printf("%f",mled_pt->getVoltage());
	
	


	while(1){

	}
}

