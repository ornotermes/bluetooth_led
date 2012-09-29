/*
	Copyright (c) 2011-2012 Rikard Lindström <ornotermes@gmail.com>
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>


#define LED_R		(1<<PC6)
#define LED_R_PORT	PORTC
#define LED_R_DDR	DDRC
#define LED_R_PWM	OCR1A

#define LED_G		(1<<PC5)
#define LED_G_PORT	PORTC
#define LED_G_DDR	DDRC
#define LED_G_PWM	OCR1B

#define LED_B		(1<<PB7)
#define LED_B_PORT	PORTB
#define LED_B_DDR	DDRB
#define LED_B_PWM	OCR1C

#define BTN			(1<<PD7)
#define BTN_PORT	PORTD
#define BTN_PIN		PIND
#define BTN_DDR		DDRD

char hexchar2hex(char);
int uart_tx (unsigned char);
unsigned char uart_rx (void);
int i;

unsigned char hexcolors[8] = "NULL", charcount = 0;
unsigned int val_r, val_g, val_b;

FILE uart_str = FDEV_SETUP_STREAM(uart_tx, uart_rx, _FDEV_SETUP_RW);

void init_hw( void )
{
	//Set full speed (disables CKDIV8 by registers)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;

	//setup LEDs
	LED_R_DDR |= LED_R;
	LED_G_DDR |= LED_G;
	LED_B_DDR |= LED_B;
	
	TCCR1A = (1<<COM1A1)|(1<<COM1A0)|(1<<COM1B1)|(1<<COM1B0)|(1<<COM1C1)|(1<<COM1C0)|(1<<WGM11);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10);
	
	ICR1 = 0xFFFF;
	
	LED_R_PWM = 0;
	LED_G_PWM = 0;
	LED_B_PWM = 0;
	
	//Init button
	//Has external pullup due to HWB
	EICRB |= (1<<ISC71);
	EIMSK |= (1<<INT7);
	
	//setup serial
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
	UBRR1L = 103;
	stdout = stdin = &uart_str;

}

void main(void)
{
	init_hw();
	sei();
	
	printf("Ready.\n");
	
	while(1)
	{
	}
}

ISR(USART1_RX_vect)
{
	if(UDR1 == '#')
	{
		scanf("%02X%02X%02X", &val_r, &val_g, &val_b);
		LED_R_PWM = val_r * val_r;
		LED_G_PWM = val_g * val_g;
		LED_B_PWM = val_b * val_b;
	}
}

int uart_tx (unsigned char data)
{
	while ( !( UCSR1A & (1<<UDRE1) ) );
	UDR1 = data;
	return 0;
}
unsigned char uart_rx ( void )
{
	while ( !(UCSR1A & (1<<RXC1) ) );
	return UDR1;
}

ISR(INT7_vect)
{
	printf_P(PSTR("Button Pressed.\n"));
}
