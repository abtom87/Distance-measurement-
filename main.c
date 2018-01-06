/*
 * main.c
 *
 *  Created on: Nov 17, 2017
 *      Author: abel
 */

#include "i2c_lcd_pc8574.h"
#include "inputCapture_timer.h"
#include "usart.h"

//#define DEBUG_ON

#define SPEED_OF_SOUND_IN_CM_S (331/10)

volatile float temp = 0;
volatile float distance = 0;
volatile uint16_t dist_whole = 0;
volatile uint16_t dist_dec = 0;
volatile uint16_t ticks_t1 = 0;
volatile uint16_t ticks_t2 = 0;
volatile uint16_t elapsed_time = 0;
volatile static uint16_t tick_count = 0;
char value_buf[7] = { 0 };
char dec_val_buf[7] = { 0 };

void init_input_capture() {
	TIMSK1 = ((1 << ICIE1)); /* Enable ICP Interrupt */
	TCCR1B = ((1 << ICES1) | (1 << ICNC1) | (1 << CS12)); /* Enable rising edge detection,
	 noise cancellation,
	 clock Pre-scaler 256*/
	edge.current_edge = INIT_RISING;
	edge.next_edge = INIT_RISING;
}

void init_timer2() {
	TIFR2 = 1 << OCF2A; /* Clear Output compare match flag  */
	TIMSK2 = 1 << OCIE2A; /* Timer 2 compare match is enabled*/
	TCCR2A = 1 << WGM21; /* CTC Mode*/
	TCCR2B = ((1 << CS21)); /* clock Prescaler 8 */
}

int main() {

	DDRC = 0xFF;
	PORTC = 0x00;   //Enable internal pullups on PORTC PINS  SDA(PC4) ,SCL(PC5)

	DDRB = 0b00000010; /*PB0 as INPUT to receive Echo, PB1 as output for Trigger*/
	PORTB = (1 << PB0) | (1 << PB1);

	OCR2A = (uint8_t) OCR2A_VALUE;  /*Timer that generates an interrupt every 20uS*/

	usart_init(BAUD_VAL);
	set_i2c_clock(100);

	/**Initialize LCD**/
	LCD_Init();

	LCD_Clear();
	LCD_Write_String("Distance: ");
	LCD_Clear();

	/**Initialize Timer and ICP**/
	init_input_capture();
	init_timer2();

	sei();

	while (1) {

		if (edge.current_edge == FALLING) {
			cli();
			if (ticks_t2 > ticks_t1)
				temp = (float) (ticks_t2 - ticks_t1) / (float) TICKS_VAL;
			else
				temp = (float) ((65535 - ticks_t1) + ticks_t2)
						/ (float) TICKS_VAL;
			temp *= 1000000; /*Calculate the pulse width in uS*/

			elapsed_time = (uint16_t) temp;

			distance = ((float) SPEED_OF_SOUND_IN_CM_S * (float) elapsed_time)
					/ (float) 2000;
			dist_whole = (uint16_t) distance; /*Characteristic part of Number*/
			dist_dec = (uint16_t) (((float) distance - (float) dist_whole)
					* 1000); /*Mantissa of the number*/
			sei();
			sprintf(dec_val_buf, "%1u", dist_dec);
			sprintf(value_buf, "%u", dist_whole);
			strcat(value_buf, ".");
			strcat(value_buf, dec_val_buf);

		}

#ifdef DEBUG_ON
		USART_Tx_string(value_buf);
#endif

		/*Send values to LCD*/
		LCD_Goto(1, 1);
		LCD_Write_String("Dist:");
		LCD_Write_String(value_buf);
		LCD_Write_String("cm");

	}

	return 0;
}

ISR(TIMER1_CAPT_vect) {

	cli();

	switch (edge.next_edge) {
	case INIT_RISING: {

		ticks_t1 = ICR1L;
		ticks_t1 |= (ICR1H << 8);
		TCCR1B &= ~(1 << ICES1); /*Next Interrupt on Falling edge*/
		edge.next_edge = FALLING;

	}
		break;

	case RISING: {
		ticks_t1 = (uint16_t) ICR1L;
		ticks_t1 |= (uint16_t) (ICR1H << 8);
		TCCR1B &= ~(1 << ICES1); /*Next Interrupt on Falling edge*/
		edge.current_edge = RISING;
		edge.next_edge = FALLING;

	}
		break;

	case FALLING: {
		ticks_t2 = (uint16_t) ICR1L;
		ticks_t2 |= (uint16_t) (ICR1H << 8);
		TCCR1B |= (1 << ICES1); /*Next Interrupt on Rising edge*/
		edge.current_edge = FALLING;
		edge.next_edge = RISING;
	}
		break;

	default:
		break;
	}
	sei();

}

ISR(TIMER2_COMPA_vect) {
	cli();
	tick_count++;

	if (tick_count == 1) {
		PORTB |= 1 << PB1;

	}

	if (tick_count == 2) {
		PORTB &= ~(1 << PB1);

	}

	if (tick_count == 700) {
		tick_count = 0;
	}

	sei();
}

