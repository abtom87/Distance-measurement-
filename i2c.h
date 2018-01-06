#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SLAV_ADDR       0x27
#define U8        		unsigned char
#define ON              1
#define OFF             0
#define DEBUG_LED(val)  {(val)?(PORTB |= (1<<PORTB0) ):(PORTB &= ~(1<<PORTB0));}

typedef enum access_t{ write_bit,read_bit}access_mode;

typedef enum status_codes_t{
	START           = 0x08,
	REP_START       = 0x10,
	MT_SLA_ACK      = 0x18,
	MT_SLA_NACK     = 0x20,
	MT_SLA_DATA_ACK = 0x28,
	MR_SLA_ACK      = 0x40,
	MR_SLA_NACK     = 0x48,
	MR_SLA_DATA_ACK = 0x50,
	MR_SLA_DATA_NACK= 0x58 }statuscode;

typedef enum result_t{FAIL, SUCCESS}result;
typedef enum switch_t{SET,RESET}swit;

result check_TWI_status(statuscode);
void   debug_led(swit);
void send_start(void);
void send_stop(void);
void send_address(U8, access_mode );
void transmit_data(unsigned char);
void set_i2c_clock(unsigned long);
unsigned char receive_byte();
