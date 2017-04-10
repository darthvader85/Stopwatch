/*
 * Stopwatch.c
 *
 * Created: 31.08.2012 13:03:36
 *  Author: Stefan
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "display.h"
#include "lcd-routines.h"
#include "Utils.h"

#define displayRefreshtime 100;

void init(void);
void start_timer0(void);
void stop_timer0(void);

volatile uint8_t counter=10;
volatile uint16_t displayCount = displayRefreshtime;

volatile int hundertstel=0;
volatile int sekunden=0;
volatile int minuten=0;

volatile int old_hundertstel=0;
volatile int old_sekunden=0;
volatile int old_minuten=0;

uint8_t running=0;
uint8_t pinchanger=0;

typedef enum {initialize, wait_for_start, count_time, wait_for_reset} state_t;

state_t state = initialize;

int main(void)
{
	while(1)
	{
		switch(state)
		{
			case initialize:
					init();
					lcd_init();
				
					// Die Ausgabemarke in die 2te Zeile setzen
					lcd_setcursor( 0, 1);
				
					// erneut Text ausgeben, aber diesmal komfortabler als String
					lcd_clear();
					lcd_string("Zeit: ");
					writeTime2Display(0,0,0,1,8);
				
					lcd_setcursor( 0, 2);
					lcd_string("Letzte:");
					writeTime2Display(0,0,0,2,8);
					state=wait_for_start;
				break;
			case wait_for_start:
				if(pinchanger==1)
				{
					start_timer0();
					pinchanger=0;
					state=count_time;
				}
				break;
			case count_time:
				if(pinchanger==3)
				{
					stop_timer0();
					pinchanger=0;
					writeTime2Display(hundertstel,sekunden,minuten,1,8);
					state=wait_for_reset;
				}
				break;
			case wait_for_reset:
				if(pinchanger==2)
				{
					old_hundertstel=hundertstel;
					old_minuten=minuten;
					old_sekunden=sekunden;
					   
					hundertstel=0;
					sekunden=0;
					minuten=0;
					pinchanger=0;
					   
					writeTime2Display(hundertstel,sekunden,minuten,1,8);
					writeTime2Display(old_hundertstel,old_sekunden,old_minuten,2,8);
					state=wait_for_start;
				}		
				break;			   			
		}
		
		//sleep_mode();
		//TODO:: Please write your application code
		if(!displayCount)
		{
			writeTime2Display(hundertstel,sekunden,minuten,1,8);
			displayCount=displayRefreshtime;
		}
	}								
}

void init()
{
	DDRB = 0x00;	// Port B als Eingang für Taster und Lichtschranken
	PORTB = 0xFF;	// Alle internen Pullups aktivieren 

	DDRC = 0x80;
	PORTC = 0x80;

// 	DDRD = 0x10;
	
	PCICR = 0x01;	// PinChange Interrupt Enable
	PCMSK0 = 0x07;	// PB0-PB2 als Interruptquelle
	
	
	sei();			// Enable Interrupts
	//set_sleep_mode(SLEEP_MODE_IDLE);
}

void start_timer0()
{
	counter=10;
	TIMSK0 = 0x01;	// Enable Timer0 Overflow Interrupt
	TCCR0B = 0x03;	// Prescaler 8
	TCNT0 = 130;	// To count only 125 Times 125 * 1/8e6 * 8 = 1ms
}

void stop_timer0()
{
	TIMSK0 = 0x00;
	TCCR0B = 0x00;
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 130;
	if(!counter)
	{	
		PORTC ^= (1 << PC7);	
		hundertstel++;
		counter=10;
		
		if(hundertstel==100)
		{
			sekunden++;
			hundertstel=0;
			
			if(sekunden==60)
			{
				minuten++;
				sekunden=0;
			}
		}
	}	
	counter--;
	displayCount--;
}

ISR(PCINT0_vect)
{
	if(!(PINB & (1<<PINB0)))
	{
		//PCMSK0 = 0x06;
		pinchanger=1;
	}
	
	if(!(PINB & (1<<PINB1)))
	{
		//PCMSK0 = 0x07;
		pinchanger=2;
	}
	
	if(!(PINB & (1<<PINB2)))
	{
		//PCMSK0 = 0x02;
		pinchanger=3;
	}		
}
