#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "typedef.h"

uint8 g_sec_unit = 0; 	//54:32:1(0)
uint8 g_sec_tenth = 0;  //54:32:(1)0
uint8 g_min_unit = 0; 	//54:3(2):10
uint8 g_min_tenth = 0;  //54:(3)2:10
uint8 g_hour_unit = 0; 	//5(4):32:10
uint8 g_hour_tenth = 0; //(5)4:32:10

void Timer1_CTC_one_second(void){
	//NON_PWM
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	/*(WGM12) set to 1 for CTC MODE
	 * (CS10) and (CS12) set to 1 for prescaler of 1024
	 */
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
	TCNT1 = 0; // start counting from zero
	OCR1A = 976; //compare value is set to 976 to rise a flag every one second
	SREG |= (1<<7); //activate global interrupt
	TIMSK |= (1<<OCIE1A); //activate MIE for compare A match interrupt
}


void INT0_init(void){
	DDRD &= ~(1<<PD2);  //set pin2 in PORTD as input pin
	PORTD |= (1<<PD2);  //activate internal pull up resistor for PD2
	MCUCR |= (1<<ISC01);   //The falling edge of INT0 generates an interrupt request
	SREG |= (1<<7); //activate global interrupt
	GICR |= (1<<INT0);  //activate MIE for INT0
}

void INT1_init(void){
	DDRD &= ~(1<<PD3);  //set pin3 in PORTD as input pin
	MCUCR |= (1<<ISC10) | (1<<ISC11);  //The rising edge of INT1 generates an interrupt request.
	SREG |= (1<<7); //activate global interrupt
	GICR |= (1<<INT1);   //activate MIE for INT1
}

void INT2_init(void){
	DDRB &= ~(1<<PB2);    //set pin2 in PORTB as input pin
	PORTB |= (1<<PB2);					  //activate internal pull up resistor for PD2
	MCUCSR &= ~(1<<ISC2); //a falling edge on INT2 activates the interrupt.
	SREG |= (1<<7); //activate global interrupt
	GICR |= (1<<INT2);   //activate MIE for INT2
}


//ISR for INT0
ISR(INT0_vect){
	/*reset timer and start counting from zero*/
	g_sec_unit = 0;
	g_sec_tenth = 0;
	g_min_unit = 0;
	g_min_tenth = 0;
	g_hour_unit = 0;
	g_hour_tenth = 0;
	TCNT1 = 0;
}


//ISR for INT1
ISR(INT1_vect){
	TCCR1B &= ~(1<<CS10) & ~(1<<CS12); //clear clock to pause countdown
}

//ISR for INT2
ISR(INT2_vect){
	TCCR1B |= (1<<CS10) | (1<<CS12); //resume timer and continue countdown
}


//ISR for timer
ISR(TIMER1_COMPA_vect){
	g_sec_unit++;
	if(g_sec_unit > 9){
		g_sec_unit = 0;
		g_sec_tenth++;
	}
	if(g_sec_tenth > 5){
		g_sec_tenth = 0;
		g_min_unit++;
	}
	if(g_min_unit > 9){
		g_min_unit = 0;
		g_min_tenth++;
	}
	if(g_min_tenth > 5){
		g_min_tenth = 0;
		g_hour_unit++;
	}
	if(g_hour_unit == 4 && g_hour_tenth == 2){
		g_hour_unit = 0;
		g_hour_tenth = 0;
	}
	if(g_hour_unit > 9){
		g_hour_unit = 0;
		g_hour_tenth++;
	}
}

int main(void){
	DDRC |= 0x0F;  //set first 4 pins in PORTC as output pins
	DDRA |= 0x3F;  //set first 6 pins in PORTA as output pins
	INT0_init();
	INT1_init();
	INT2_init();
	Timer1_CTC_one_second();
	while(1){
		PORTA = (PORTA & ~(0x3F)) | (1<<PA5);
		PORTC = (PORTC & 0xF0) | (g_sec_unit & 0x0F);
		_delay_us(20);
		PORTA = (PORTA & ~(0x3F)) | (1<<PA4);
		PORTC = (PORTC & 0xF0) | (g_sec_tenth & 0x0F);
		_delay_us(20);
		PORTA = (PORTA & ~(0x3F)) | (1<<PA3);
		PORTC = (PORTC & 0xF0) | (g_min_unit & 0x0F);
		_delay_us(20);
		PORTA = (PORTA & ~(0x3F)) | (1<<PA2);
		PORTC = (PORTC & 0xF0) | (g_min_tenth & 0x0F);
		_delay_us(20);
		PORTA = (PORTA & ~(0x3F)) | (1<<PA1);
		PORTC = (PORTC & 0xF0) | (g_hour_unit & 0x0F);
		_delay_us(20);
		PORTA = (PORTA & ~(0x3F)) | (1<<PA0);
		PORTC = (PORTC & 0xF0) | (g_hour_tenth & 0x0F);
		_delay_us(20);
	}
}
