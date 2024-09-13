# include <reg51.h>
# include "lcd.h"
#include <math.h>

sbit CS =P3^5;          
sbit DCLK =P3^6;       
sbit DIN =P3^4;      
sbit DOUT =P3^7;        
sbit BP = P1^5;         //Buzzer Pin

float displayVoltage();
float displayTemperature();
unsigned int displayLDR();
void Beep();
float Set_temp();
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

//Voltage Measurment 
 float displayVoltage()
{
    unsigned int value1;
    float voltage;

    value1 = ADC_Driver(1, 0, 0);      
    voltage = value1 * (5.0 / 255.0);  
	  Delay(1000);
	return voltage;
	
}

//Temprature Measurment
float  displayTemperature()
{
    unsigned int temp_Value;
    float T, Temperature;

    temp_Value = ADC_Driver(1, 0, 1);                                    
    T = 1 / ((1 / 298.15) + ((1 / 3380.0) * log((255.0 / temp_Value) - 1)));  //Special Formula
    Temperature = T - 273.15;                                                

    LCD_ShowString(1, 10, "Temp: ");
    LCD_ShowNum(2, 10, (unsigned int)Temperature, 2);                  
    LCD_ShowChar(2, 12, '.');
    LCD_ShowNum(2, 13, (unsigned int)(Temperature * 1000) % 1000, 3);  
  //	LCD_ShowChar(2, 11, 'C');
  	Delay(1000);
	return Temperature;
}


//Temprature Setting
float Set_temp()
{
	     float Set_Temp,voltage1;
	
       voltage1=displayVoltage();
			 Set_Temp=(40.0*voltage1)/5.0; 
			 LCD_ShowString(1,0, "SetTemp ");
			 LCD_ShowNum(2, 0, Set_Temp, 2);
		   LCD_ShowChar(2, 2, '.');
       LCD_ShowNum(2, 3, (int)( Set_Temp* 1000) % 1000, 3);
	
    return Set_Temp;
}

void Beep()
{
    unsigned char i;
	

   for(i=0;i<200;i++)            
	{
	   BP=0;
	   Delay(16);
	   BP=1;
     Delay(16);
}
}
void main()
{   
	float SET_temp,Temprature1;              
    LCD_Intialization();                  

    while (1)
    {
       
			SET_temp=Set_temp();                 
			Delay(1000);                          
			Temprature1=displayTemperature();  
			Delay(1000);
			
			if(Temprature1>SET_temp)           
			{
				Beep();
			}
	
    }
	}
