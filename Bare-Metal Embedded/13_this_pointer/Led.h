#ifndef __LED_H
#define __LED_H

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

#endif

