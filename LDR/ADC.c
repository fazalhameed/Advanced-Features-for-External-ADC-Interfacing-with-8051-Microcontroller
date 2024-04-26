# include <reg52.h>
# include "lcd.h"
#include <math.h>



sbit CS =P3^5;          
sbit DCLK =P3^6;      
sbit DIN =P3^4;         
sbit DOUT =P3^7;        

void LDR_Measurment();
unsigned char ADC_Driver (bit A0,bit A1,bit A2)         
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

//Voltage Measurment 
void displayVoltage()
{
    unsigned int value1;
    float voltage;

    value1 = ADC_Driver(1, 0, 0);      
    voltage = value1 * (5.0 / 255.0); 

    LCD_ShowString(1, 0, "Vol: ");
    LCD_ShowNum(2, 0, (unsigned int)voltage, 1);                  
    LCD_ShowChar(2, 1, '.');
    LCD_ShowNum(2, 2, (unsigned int)(voltage * 1000) % 1000, 3); 
	  LCD_ShowChar(2, 4, 'V');
	  Delay(1000);
}

//Temprature Measurment
void displayTemperature()
{
    unsigned int temp_Value;
    float T, Temperature;

    temp_Value = ADC_Driver(1, 0, 1);                                         
    T = 1 / ((1 / 298.15) + ((1 / 3380.0) * log((255.0 / temp_Value) - 1)));  //Special Formula
    Temperature = T - 273.15;                                                 //Kelvin to degree conversiuon

    LCD_ShowString(1, 6, "Temp: ");
    LCD_ShowNum(2, 6, (unsigned int)Temperature, 2);                  
    LCD_ShowChar(2, 8, '.');
    LCD_ShowNum(2, 9, (unsigned int)(Temperature * 1000) % 1000, 3); 
  //	LCD_ShowChar(2, 11, 'C');
  	Delay(1000);
}
//LDR Measurment 
void displayLDR()
{
    unsigned int value, scaledValue;

    value = ADC_Driver(0, 1, 0);        
    scaledValue = (value * 100) / 255; 

    LCD_ShowString(1, 13, "LDR ");
    LCD_ShowNum(2, 13, scaledValue, 3);   
	  Delay(1000);
}

void main()
{
    LCD_Intialization();                    

    while (1)
    {
        displayVoltage();               //ADC Voltage 
        Delay(1000);

        displayTemperature();           //ADC Temprature
        Delay(1000);

        displayLDR();                   //ADC LDR 
        Delay(1000);
    }
}