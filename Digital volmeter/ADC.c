# include <reg52.h>
# include "lcd.h"

// In this Code we are using SPI communication 

sbit CS =P3^5;          
sbit DCLK =P3^6;       
sbit DIN =P3^4;        
sbit DOUT =P3^7;       

        
void ADC_VOLTAGE ();
unsigned char ADC_Driver (bit A0,bit A1,bit A2)          //SPI Protcol
{  
	
	unsigned char i, temp=0;
	 DCLK=0;                                        
   CS=0;                                         
	
	 //Control Byte Transfer Start
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
	   temp |=DOUT;            //data Stored in temp
		   
	}
	
	 CS=1;               
	
	return temp;        
}

   //Voltage Measurment
void ADC_VOLTAGE ()	   
{
	float voltage,value;    
	
  value = ADC_Driver(1,0,0);                   
	
		     voltage=value*(5.0/255.0);             //(0 to 5V scale) voltage= 5.0(Maxiuim Voltage on 255.0)*ADC_Value/255.0 (highest Value to which ADC measure)
         LCD_ShowString(1,0,"Voltage: ");
         LCD_ShowNum(2,1,voltage,1);            
	    	 LCD_ShowChar(2,2,'.'); 
		     LCD_ShowNum(2,3,(unsigned int)(voltage*1000)%1000,3); 
		     LCD_ShowChar(2,6,'V'); 
		     Delay(1000);

}
	void main ()	
{
	LCD_Intialization();          
	
  while(1)
	{    
		      ADC_VOLTAGE ();       // Call the ADC voltage Function
	}   
     
}

	
	