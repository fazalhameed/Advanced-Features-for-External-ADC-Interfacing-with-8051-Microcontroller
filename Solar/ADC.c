# include <reg51.h>
# include "lcd.h"
# include <math.h>

sbit CS =P3^5;          
sbit DCLK =P3^6;        
sbit DIN =P3^4;         
sbit DOUT =P3^7;       
sbit LED =P2^0; 

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

	  LCD_ShowString(1, 0, "V:");
    LCD_ShowNum(1, 3, (unsigned int)voltage, 1);                   
    LCD_ShowChar(1, 4, '.');
    LCD_ShowNum(1, 5, (unsigned int)(voltage * 1000) % 1000, 3); 
	  //LCD_ShowChar(2, 4, 'V');
	  Delay(1000);
	
    return voltage;
}


//LDR Measurment 
unsigned int displayLDR()
{
    unsigned int value, scaledValue;

    value = ADC_Driver(0, 1, 0);        
    scaledValue = (value * 100) / 255;  

    LCD_ShowString(1, 9, "LDR");
    LCD_ShowNum(1, 13, scaledValue, 3);   
	  Delay(1000);
	
	return scaledValue;
}
void Contactor ()
{
	
	   float Voltage_Value;
	   unsigned int LDR_Value;
	
      Voltage_Value=displayVoltage();
			Delay(1000);
			LDR_Value=displayLDR();
			Delay(1000);
		
			if(LDR_Value<30)  //Night Time
			{
			
			  if(Voltage_Value>=4)
				{
					LCD_ClearSecondLine();
					LCD_ShowString(2, 0, "WAPDA SUPPLY ON");
           LED=1;
					Delay(1000);
				}
				  else
			   	{ LCD_ClearSecondLine(); 
						LCD_ShowString(2, 0, "RemoveHeavy load");
            LED=0;
					  Delay(1000);
				   }
			
			
			
			}
				else
         {LCD_ClearSecondLine();	
				  LCD_ShowString(2, 0, "Day Enjoy Solar ");		Delay(1000);}

}
void main()
{    
    LCD_Intialization();
    while (1)
    {
      Contactor ();      
    }
	}