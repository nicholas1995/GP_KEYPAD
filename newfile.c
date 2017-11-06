//THIS CODE WORKS IS THE LATEST VERSION 2nd NOVEMBER 2017
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


#define bitmask 15
#define DATA_AVAILABLE  1
#define NO_NEW_DATA 0
int read=0;
/*
 ***********************************************************************************************************
 *                                      DELAYS FOR XLCD
 ***********************************************************************************************************
 */
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
void low_isr(void);
void high_isr(void);

/*
 ***********************************************************************************************************
 *                               FUNCTIONS
 ***********************************************************************************************************  
 */

/*
 ****************************************************************
 *                        value_to_display
 **************************************************************** 
 */
void value_to_display(void)
{
    char value_out[];
    int value;
    value=(bitmask&PORTC);
                                        //PIN LAYOUT  ...RC3,RC2,RC1,RC0
              switch(value)
                 {
                     case 0:                //corresponds to 0b00000000
                         value_out[0]='1';  
                     break;     
                     case 1:               //corresponds to 0b0000001
                         value_out[0]='2';  
                     break;
                     case 2:               //corresponds to 0b00000010
                         value_out[0]='3';  
                     break;
                     case 3:               //corresponds to 0b00000011
                         value_out[0]='A';  
                     break;
                     case 4:               //corresponds to 0b00000100
                         value_out[0]='4';  
                     break;
                     case 5:               //corresponds to 0b00000101
                         value_out[0]='5';  
                     break;
                     case 6:               //corresponds to 0b00000110
                         value_out[0]='6';  
                     break;
                     case 7:              //corresponds to 0b00000111
                         value_out[0]='B';  
                     break;
                     case 8:              //corresponds to 0b00001000
                         value_out[0]='7';  
                     break;   
                     case 9:              //corresponds to 0b00001001
                         value_out[0]='8';  
                     break;
                     case 10:              //corresponds to 0b00001010
                         value_out[0]='9';  
                     break;
                     case 11:              //corresponds to 0b00001011
                         value_out[0]='C';  
                     break;
                    case 12:               //corresponds to 0b00001100
                         value_out[0]='*';  
                     break;
                     case 13:              //corresponds to 0b00001101
                         value_out[0]='0';  
                     break;
                     case 14:              //corresponds to 0b00001110
                         value_out[0]='#';  
                     break;
                     case 15:               //corresponds to 0b00001111
                         value_out[0]='D';
                     break;
              }
                WriteCmdXLCD(0b00000001);
                while(BusyXLCD());
                SetDDRamAddr( 0x00 );
                putrsXLCD( "Keypad Value:");
                putsXLCD(value_out);
}

/*
 ****************************************************************
 *                        init_LCD
 **************************************************************** 
 */
void init_LCD(void){
    OpenXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( BLINK_ON );
    while( BusyXLCD() );
    WriteCmdXLCD( SHIFT_DISP_LEFT );
}
/*
 ****************************************************************
 *                        start_up
 **************************************************************** 
 */
void start_up(void)
{
    //------------------CONFIGURE INTERRUPT
    INTCON2bits.INTEDG1=1;
    INTCON3bits.INT1E=1;
    INTCON3bits.INT1F=0;
    INTCONbits.GIE=1;
    //------------------Configure Ports
     TRISB=0xFF;
     TRISC=0xFF;

     init_LCD();
     
}
/*
 **********************************************************************************************************
 *                               LOW PRIORITY INTERRUPT
 **********************************************************************************************************
 */
#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
    _asm
    goto low_isr
    _endasm
}
#pragma code        //return to the default section of code

#pragma interruptlow low_isr
void low_isr(void)
{ 
        //CODE FOR LOW PRIORITY INTERRUPTS 
    
}
/*
 **********************************************************************************************************
 *                               HIGH PRIORITY INTERRUPT
 **********************************************************************************************************
 */
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
    _asm
    goto high_isr
    _endasm
}
#pragma code

#pragma interrupt high_isr
void high_isr(void)                       //CODE FOR HIGH PRIORITY INTERRUPTS 
{           
    INTCONbits.GIE=0;
    if(INTCON3bits.INT1F==1)              //RB0 changed states.
    {
        read=DATA_AVAILABLE;
        
        INTCON3bits.INT1F=0;
    }
   
    INTCONbits.GIE=1;
}
/*
 ***********************************************************************************************************
 *                               MAIN
 ***********************************************************************************************************  
 */
int main()
{
    start_up();
     while(1)
     {
         if(read==1)
         {
             value_to_display();
             read=NO_NEW_DATA;
         }
     }
}
