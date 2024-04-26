# include <reg52.h>
# include "lcd.h"
#include <math.h>


sbit CS =P3^5;          
sbit DCLK =P3^6;        
sbit DIN =P3^4;         
sbit DOUT =P3^7;        

void ADC_Temprature ();
unsigned char ADC_Driver (bit A0,bit A1,bit A2)          //SPI Protcol
{  
	
	unsigned char i, temp=0;
	 DCLK=0;                                        
   CS=0;                                          
	 DCLK=0;  DIN=1;  DCLK=1;                     
	 DCLK=0;  DIN=A2;  DCLK=1;                     
   DCLK=0;  DIN=A1;  DCLK=1;                      
	 DCLK=0;  DIN=A0;  DCLK=1;                       
	 DCLK=0;  DIN=1;  DCLK=1;                     
	 DCLK=0;  DIN=1;  DCLK=1;                      
	 DCLK=0;  DIN=1;  DCLK=1;                      
	 DCLK=0;  DIN=1;  DCLK=1;                      
	
   
	DCLK = 0;           
	Delay(10);


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

void ADC_Temprature ()
{
	
 unsigned int temp_Value;
    float T, Temperature;

    temp_Value = ADC_Driver(1, 0, 1);      
	
    T = 1 / ((1 / 298.15) + ((1 / 3380.0) * log((255.0 / temp_Value) - 1)));  //Special Formula
    Temperature = T - 273.15;          //Kelvin to degree conversiuon

    LCD_ShowString(1, 0, "Temp: ");
    LCD_ShowNum(2, 0, Temperature, 2);     
    LCD_ShowChar(2, 2, '.');
    LCD_ShowNum(2, 3, (unsigned int)(Temperature * 1000) % 1000, 3);  
  	LCD_ShowChar(2, 6, 'C');
	  Delay(1000);



}	
void main ()	
{
   LCD_Intialization();      
	
  while(1)
	{ 
    ADC_Temprature ();
	}	    
       
}