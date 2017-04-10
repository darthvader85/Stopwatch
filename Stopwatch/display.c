/*
 * display.c
 *
 * Created: 18.10.2012 22:17:26
 *  Author: Stefan
 */ 

#include <avr/io.h>
#include "Utils.h"
#include "lcd-routines.h"

void writeTime2Display(int hundertstel, int sekunden, int minuten, uint8_t zeile, uint8_t spalte)
{
	char output[4];
	
	lcd_setcursor( spalte+6, zeile);
	int2str(output, hundertstel, 2);
	lcd_string(output);
	if(hundertstel<10)
	{
		lcd_setcursor( spalte+6, zeile);
		lcd_string("0");
	}
	
	lcd_setcursor( spalte+5, zeile);
	lcd_string(",");
	lcd_setcursor( spalte+3, zeile);
	int2str(output, sekunden, 2);
	lcd_string(output);
	if(sekunden<10)
	{
		lcd_setcursor( spalte+3, zeile);
		lcd_string("0");
	}
	
	lcd_setcursor( spalte+2, zeile);
	lcd_string(":");
	lcd_setcursor( spalte, zeile);
	int2str(output, minuten, 2);
	lcd_string(output);
}