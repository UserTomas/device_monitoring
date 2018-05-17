#include "serial.h"

//===========================================UART===============================

  /*int USART_send_byte(char u8data, FILE *stream)
  {
	  if(u8data == '\r')				// \n
	  {
		  //USART_send('\r');
		  //USART_send('\n');
		  SW_UART_send('\r');
		  SW_UART_send('\n');
		  //	USART_send_byte('\n',0);	// \r
		  return 0;
	  }

	  SW_UART_send(u8data);
	  //while(!(UCSR0A & (1<<UDRE0))){};
	  //UDR0 = u8data;
	  return 0;
  }*/
  /*int USART_receive_byte(FILE *stream)
  {
	  uint8_t u8data;



	  //while(!(UCSR0A& (1<<RXC0))){};
	  //u8data = UDR0;
	  //USART_send_byte(u8data,stream);  //echo
	  u8data = SW_UART_receive();
	  return u8data;
  }*/

  ////////////////////////////////////////
void SW_UART_send(uint8_t a)	// dlzka 8, 9600, bez parity, bez riad.
{
  sbi(DDRC,STXD);
  cbi(PORTC,STXD);	// start bit

  _delay_us(104);
  for(uint8_t i = 0; i < 8; i++)
  {
    if((a & 1) == 1)sbi(PORTC,STXD); else cbi(PORTC,STXD);
    a = a >> 1;
    _delay_us(103);
  }
  sbi(PORTC,STXD);				// stop bit
  _delay_us(104);
}

////////////////////////////////////////
uint8_t SW_UART_receive(void)	// dlzka 8, 9600, bez parity, bez riad.
{
  uint8_t a = 0;
  cbi(DDRC,SRXD);				// vstup
  sbi(PORTC,SRXD);				//pull-up
  _delay_us(2);
  while((PINC & (1<<SRXD))!= 0){}	//caka na start bit
  _delay_us(52);
  for(uint8_t i = 0; i < 8; i++)
  {
    a = a>>1;
    _delay_us(103);			// 103 us + 16 instrukcii
    if((PINC & (1<<SRXD)) == (1<<SRXD)) a = a | 0b10000000; else a = a & 0b01111111;
  }
  return(a);
}

void USART0_Init(void)
{
  DDRD |=(1<<PORTD1);					// Tx output
  UBRR0H = (uint8_t)(UBRR_VALUE>>8);	// BAUDERATE 9600
  UBRR0L = (uint8_t)UBRR_VALUE;
  UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);  //8 bits, parity non, 1 stop
  UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0);	// enable Rx,Tx, enable Receive Complete interrupt
  UCSR0A |= (1<<U2X0);
  sei(); // Enable the Global Interrupt
}

unsigned char USART_Receive( void )
{
   /* Wait for data to be received */
   while ( !(UCSR0A & (1<<RXC0)) )
      ;
   /* Get and return received data from buffer */
   return UDR0;
}
void USART0_SendByte(uint8_t u8Data)
{
  //wait while previous byte is completed
  while(!(UCSR0A&(1<<UDRE0))){};
  // Transmit data
  UDR0 = u8Data;
}

void USART0_Send(char *s)
{
  while (*s)
  USART0_SendByte(*s++);
}
