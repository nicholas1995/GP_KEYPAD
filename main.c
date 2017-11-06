#include<p18f452.h>
#include"xlcd.h"
#include<delays.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>


#pragma config OSC= HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void DelayFor18TCY( void )
{

    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
    Delay10TCYx(1);
}

void DelayPORXLCD (void)
{
 Delay10KTCYx(6);
}

void DelayXLCD (void)
{
 Delay10KTCYx(2); 
}

void init_LCD(void){
    OpenXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( BLINK_ON );
    while( BusyXLCD() );
    WriteCmdXLCD( SHIFT_DISP_LEFT );
}

void Configure_LCD(void){
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    WriteCmdXLCD(BLINK_ON);
}

int main()
{
    //------------------Initiate Values
    int value;
    char output;
    int bitmask=15;
    char value_out[];
    char clear_screen[]="               ";
    //------------------Configure PortB 
    TRISB=0xFF;  


     
     init_LCD();
     

     while(1){
     //------------------Main Code
         if(PORTBbits.RB4==1)
            {  
              value=(bitmask&PORTB);
               // itoa(value,value_out);
             
              switch(value)
                 {
                     case 0:
                         value_out[0]='1';
                                 
                     break;
                     case 1:
                         value_out[0]='2';
                     break;
                     case 2:
                         value_out[0]='3';
                     break;
                     case 3:
                         value_out[0]='A';
                     break;
                     case 4:
                         value_out[0]='4';
                     break;
                     case 5:
                         value_out[0]='5';
                     break;
                     case 6:
                         value_out[0]='6';
                     break;
                     case 7:
                         value_out[0]='B';
                     break;
                     case 8:
                         value_out[0]='7';
                     break;   
                     case 9:
                         value_out[0]='8';
                     break;
                     case 10:
                         value_out[0]='9';
                     break;
                     case 11:
                         value_out[0]='C';
                     break;
                    case 12:
                         value_out[0]='*';
                     break;
                     case 13:
                         value_out[0]='0';
                     break;
                     case 14:
                         value_out[0]='#';
                     break;
                     case 15:
                         value_out[0]='D';
                     break;
              }
             
                Delay1KTCYx(0x64);
                while(BusyXLCD());
                SetDDRamAddr( 0x00 );
                putsXLCD(clear_screen);

               
                while(BusyXLCD());
                SetDDRamAddr( 0x00 );
                putrsXLCD( "Keypad Value:");
                putsXLCD(value_out);
                
  
            }
     }
   
}
