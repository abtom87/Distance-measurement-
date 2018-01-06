#include "i2c.h"

#define Fosc 16000000UL

result check_TWI_status(statuscode stat) {
	switch (stat) {
	case START:
		if ((TWSR & 0xF8) == START)
			return SUCCESS;
		else
			return FAIL;
		break;

	case REP_START:
		if ((TWSR & 0xF8) == REP_START)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MT_SLA_ACK:
		if ((TWSR & 0xF8) == MT_SLA_ACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MT_SLA_NACK:
		if ((TWSR & 0xF8) == MT_SLA_NACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MT_SLA_DATA_ACK:
		if ((TWSR & 0xF8) == MT_SLA_DATA_ACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MR_SLA_ACK:
		if ((TWSR & 0xF8) == MR_SLA_ACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MR_SLA_NACK:
		if ((TWSR & 0xF8) == MR_SLA_NACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MR_SLA_DATA_ACK:
		if ((TWSR & 0xF8) == MR_SLA_DATA_ACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	case MR_SLA_DATA_NACK:
		if ((TWSR & 0xF8) == MR_SLA_DATA_NACK)
			return SUCCESS;
		else
			return FAIL;
		break;

	default:
		return FAIL;
		break;

	}

}

void debug_led(swit val) {
	if (val == SET)
		PORTB |= (1 << PORTB1);
	else
		PORTB &= ~(1 << PORTB1);

}

void set_i2c_clock(unsigned long freq_in_khz) {
	TWCR = 1 << TWEN;
	TWSR |= (1 << TWPS0); //Prescaler 4

	unsigned long baud = 0, freq = 0;

	freq = freq_in_khz * 1000;
	baud = (Fosc / freq - 16) / (2 * 4);

	TWBR = (U8) baud;

}

void send_start(void) {

	TWCR = ((1 << TWEN) | (1 << TWSTA) | (1 << TWINT));
	while (!(TWCR & (1 << TWINT)))
		;  //wait for TWINT to be set

}

void send_address(U8 add, access_mode acc) {
	if (acc == read_bit) {
		TWDR = ((U8) (add << 1) | (U8) 0x01);
		TWCR = ((1 << TWINT) | (1 << TWEN));
		while (!(TWCR & (1 << TWINT)))
			;  //wait for TWINT flag SET
	} else {
		TWDR = ((U8) (add << 1) | (U8) 0x00);
		// TWDR = 0x68 ;
		TWCR = ((1 << TWINT) | (1 << TWEN));
		while (!(TWCR & (1 << TWINT)))
			;  //wait for TWINT flag SET
	}

}

void transmit_data(unsigned char dat) {

	TWDR = dat;
	TWCR = ((1 << TWINT) | (1 << TWEN));
	while (!(TWCR & (1 << TWINT)))
		;
}

void send_stop() {

	TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));

}

unsigned char receive_byte() {

	TWCR = ((1 << TWINT) | (1 << TWEN));
	while (!(TWCR & (1 << TWINT)))
		;
	return TWDR;

}

