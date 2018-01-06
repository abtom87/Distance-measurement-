#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "usart.h"

int j = 0;
volatile unsigned counter = 0;

void USART_Tx_byte(char* c) {
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = *c;
}

void usart_init(unsigned int b_rate) {
	UBRR0H = 0x00;
	UBRR0L = 0x00;
	UCSR0B = 0x00;
	UCSR0C = 0x00;
	UDR0 = 0x00;

	UBRR0H = (unsigned char) b_rate >> 8;
	UBRR0L = (unsigned char) b_rate;

	UCSR0B = ((1 << TXEN0)); // Polling mode: Wait till UDR is empty.
	UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));

}

void USART_Tx_string(char* str) {
	for (j = 0; j < strlen(str) + 1; j++)
		USART_Tx_byte(&str[j]);

	USART_Tx_byte("\n");
	USART_Tx_byte("\r");

}

