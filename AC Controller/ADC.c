# include <reg52.h>
# include "lcd.h"
#include <math.h>



sbit CS =P3^5;         
sbit DCLK =P3^6;      
sbit DIN =P3^4;         
sbit DOUT =P3^7;     

float displayTemperature();
unsigned int displayLDR();
void AC_Controller ();

unsigned char ADC_Driver (bit A0,bit A1,bit A2)          //SPI Protcol
{  
	
	unsigned char i, temp=0;
	 DCLK=0;                                       
   CS=0;                                          
	 //Control Byte Transfer Start(On Rising Edge of DCLK, The data byte bits are triggred)
	 DCLK=0;  DIN=1;  DCLK=1;                      
	 DCLK=0;  DIN=A2;  DCLK=1;                     
   DCLK=0;  DIN=A1;  DCLK=1;                      
	 DCLK=0;  DIN=A0;  DCLK=1;                    
	 DCLK=0;  DIN=1;  DCLK=1;                      
	 DCLK=0;  DIN=1;  DCLK=1;                     
	 DCLK=0;  DIN=1;  DCLK=1;                      
	 DCLK=0;  DIN=1;  DCLK=1;                     
	
   //After control Byte transfer take a while To start conversion of data
	DCLK = 0;          
	Delay(10);

//Start Data Conversion (the Data Conversion bits are triggred on falling edge)
	for(i=0;i<=7;i++)
	{	 
		temp <<=1;                
		              
		 DCLK=1;                 
	 	 DCLK=0; 
	   temp |=DOUT;            
		   
	}
	
	 CS=1;              
	
	return temp;       
}

//Temprature Measurment
float displayTemperature()
{
    unsigned int temp_Value;
    float T, Temperature;

    temp_Value = ADC_Driver(1, 0, 1);                                        
    T = 1 / ((1 / 298.15) + ((1 / 3380.0) * log((255.0 / temp_Value) - 1)));  //Special Formula
    Temperature = T - 273.15;                                                 //Kelvin to degree conversiuon

	  LCD_ShowString(1, 0, "T:");
    LCD_ShowNum(1, 2, (unsigned int)Temperature, 2);               
    LCD_ShowChar(1, 4, '.');
    LCD_ShowNum(1, 5, (unsigned int)(Temperature * 1000) % 1000, 3);
  //	LCD_ShowChar(2, 11, 'C');
  	Delay(1000);
	return Temperature;
}
//LDR Measurment 
unsigned int displayLDR()
{
    unsigned int value, scaledValue;

    value = ADC_Driver(0, 1, 0);       
    scaledValue = (value * 100) / 255;   

  	LCD_ShowString(1,9, "LDR: ");
    LCD_ShowNum(1, 13, scaledValue, 3);  
	  Delay(1000);
	  return scaledValue;
}

void AC_Controller ()                 
{
	
	float LDR_Value;
  unsigned int	Temprature_Value;

       LDR_Value=displayLDR();                
			 Temprature_Value=displayTemperature();
			 
			if(LDR_Value>30)      
				{
				
				if(Temprature_Value<26)          
					{ LCD_ClearSecondLine();
					LCD_ShowString(2,0 , "Control: AC OFF ");Delay(10000);}
				else if(Temprature_Value>29)
					{LCD_ClearSecondLine();   
					LCD_ShowString(2,0 , "Control: AC ON ");Delay(10000);}
				
				}
				else                  
				{ 
				LCD_ClearSecondLine();
					LCD_ShowString(2,0 , "NightTime AC OFF ");  Delay(10000);
				}


}
void main()           //main function
{    
    LCD_Intialization(); 

    while (1)
    {
       AC_Controller ();    
    }
}