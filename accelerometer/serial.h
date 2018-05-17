#include "define.h"
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//int USART_send_byte(char u8data, FILE *stream);
//int USART_receive_byte(FILE *stream);
void SW_UART_send(uint8_t a);	// dlzka 8, 9600, bez parity, bez riad.;
uint8_t SW_UART_receive(void);	// dlzka 8, 9600, bez parity, bez riad.;
void USART0_Init(void);
void USART0_SendByte(uint8_t u8Data);
void USART0_Send(char *s);
unsigned char USART_Receive( void );
//ISR(USART_RXC_vect); //interrupt on Rx
