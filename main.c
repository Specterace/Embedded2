/*
 * Lab2Project.c
 *
 * Author : Oscar Choy
 * 
 * This project uses both the ATMega328P board and an output console (PuTTY in my case)
 * to achieve two actions:
 * 
 *	First, it will control the on-board LED. When the user types "q" on their keyboard, the
 *	LED with switch on. When the user types "w" on the keyboard, the LED will switch off. The
 * 	LED will not do anything if any other key is typed, no matter if the light is turned on or off.
 *	Nor will it do anything if the user types "q" when the LED is on, or "w" when the LED is off.
 *
 *	Second, it will output a message to the output window. When the LED is turned on by pressing "q",
 *	a message reading "ON" will be printed to the window. When the LED is turned off by pressing "w",
 * 	a message reading "OFF" will be printed to the window. Pressing any other keys will not print 
 *	anything to the console, nor will anything be printed if "q" is pressed when the light is on or "w"
 * 	is pressed when the light is off.
 *	
 */ 
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <avr/delay.h>

void UART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
unsigned char USART_Recieve(void);
void send_str(char* sPtr);

char onString[] = "ON";
char offString[] = "OFF";
char lf = 0x0a;
char cr = 0x0d;
unsigned char input;
char on = 0;

int main(void)
{
	DDRB |= 1<<DDB5;
	UART_Init(MYUBRR);
	while(1) {
		input = USART_Recieve();
		if ((input == 'q') && (on == 0)) {
		    PORTB |= 1<<PORTB5;
			send_str(onString);
			on = 1;
		} else if ((input == 'w') && (on == 1)) {
			PORTB &= ~(1<<PORTB5);
			send_str(offString);
			on = 0;
		}
	}
	return 0;
}

void UART_Init(unsigned int ubrr) {
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void USART_Transmit(unsigned char data) {
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

unsigned char USART_Recieve(void) {
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void send_str(char* sPtr) {
	while (*sPtr != 0x00) {
		USART_Transmit(*sPtr);
		_delay_ms(10);
		sPtr++;
	}
	USART_Transmit(cr);
	_delay_ms(10);
	USART_Transmit(lf);
}