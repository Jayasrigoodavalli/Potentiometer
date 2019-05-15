/*
 * GccApplication2.c
 *
 * Created: 15-May-19 12:13:38 PM
 * Author : JAYASREE
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <stdlib.h>
#define enable 5
#define registerselection 7
void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);
void PWM_init()
{
	/*set fast PWM mode with non-inverting o/p*/
TCCR0=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00);
DDRB|=(1<<PB3);/*set OC0 pin as o/p*/
}


int main(void)
{
	PWM_init();
	DDRC=0XFF;
	DDRA=0X00;
	DDRB=0XFF;
	DDRD=0XFF;
	_delay_ms(50);
	//Voltage selection
	ADMUX|=(1<<REFS0)|(1<<REFS1);
	//Prescale selection(ADPS0,ADPS1,ADPS2) and enable the ADC(ADEN) and auto triggering(ADATE);
	ADCSRA|=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	int16_t COUNTA=0;
	char SHOWA[3];
	send_a_command(0x01);//Clear Screen 0X01=00000001
	_delay_ms(50);
	send_a_command(0X38);
	_delay_ms(50);
	send_a_command(0b00001111);
	_delay_ms(50);
	ADCSRA|=(1<<ADSC);//ADC Conversion Start
    /* Replace with your application code */
    while (1) 
    {
		COUNTA=ADC/4;
		send_a_string("Smartbridge");
		send_a_command(0X80+0X40+0);//Shifting cursor to 1st shell of 2nd line
		send_a_string("Resistance=");
		send_a_command(0X80+0X40+11);//Shifting cursor to 9th shell of 2nd line
		//command for putting variable number in LCD
		//(variable number,in which character to replace,which base is variable(ten here as we are counting number in base10))
		itoa(COUNTA,SHOWA,10);
		send_a_string(SHOWA);
		send_a_string("");
		send_a_command(0X80+0);//Returning to 1st line is shell
		OCR0=COUNTA;
	}
}
void send_a_command(unsigned char command)
{
	PORTC=command;
	PORTD&=~1<<registerselection;
	PORTD|=1<<enable;
	_delay_ms(20);
	PORTD&=~1<<enable;
	PORTC=0;
}
void send_a_character(unsigned char character)
{
	PORTC=character;
	PORTD|=1<<registerselection;
	PORTD|=1<<enable;
	_delay_ms(20);
	PORTD&=~1<<enable;
	PORTC=0;
}
void send_a_string(char *string_of_characters)
{
	while(*string_of_characters>0)
	{
		send_a_character(*string_of_characters++);
	}
}

    


