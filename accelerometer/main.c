/*
 * TWI3.c
 *
 * Created: 07.11.2017 10:44:39
 * Author : PC
 */

#ifndef F_CPU
#define F_CPU 12000000UL
#endif


#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdio.h>
#include "vector.h"
#include <string.h>
#include <math.h>
#include "define.h"
#include "i2c.h"
#include "serial.h"
#include "compass.h"
#include <avr/interrupt.h>
#include <stdbool.h>

char buf[B_SIZE];
bool flag = false;
bool firstMsg = false;
char receivedByte;
int text [16];

void setup(void);
void test_tl(void);
void calibrateMagetometer(int *x, int *y, int *z);
//void send_averageData();

ISR(USART_RX_vect)
{
  cli();
  receivedByte = UDR0; // Fetch the received byte value into the variable

  if(receivedByte == 'S' ) {
   // PORTB |= (1<<LEDM);
   // _delay_ms(200);
    flag = true;
   // PORTB &=~ (1<<LEDM);
  }else if(receivedByte == 'E'){
  //  PORTB |= (1<<LEDGREEN);
   // _delay_ms(200);
    flag = false;
    //PORTB &=~ (1<<LEDGREEN);
  }
  firstMsg = true;
  sei();
}


int main(void)
{


  _delay_ms(100);
	int XA, YA, ZA, magXA, magYA, magZA;

  DDRB=0xff;
  setup();

  USART0_Init();
  TWI_init_master();   // Function to initialize TWI

	write_register_acc(0x20, 0x47);  // CTR_REG1  50 Hz, normal mode
	write_register_mag(0x02, 0x00);  // MR_REG_M,  Continuous-conversion mode
	write_register_mag(0x01, 0x20);  //CRB_REG_M, The gain configuration ±1.3

  sei();
 	_delay_ms(1000);
	while(1)
	{
		if((PIND & (1<<S2)) == 0){
      flag = !flag;
      firstMsg = true;
		}
    if (firstMsg)
    {
      if(flag){
        PORTB |= (1<<LEDGREEN);
        _delay_ms(200);
        USART0_Send("S\r\n");
        PORTB &=~ (1<<LEDGREEN);
      }else{
        PORTB |= (1<<LEDM);
        _delay_ms(200);
        USART0_Send("E\r\n");
        PORTB &=~ (1<<LEDM);
      }
      firstMsg = !firstMsg;
    }

		if (flag)
		{
			XA = read_acc('X');
			printf("%d  ",XA);

			YA = read_acc('Y');
			printf("%d ",YA);

			ZA = read_acc('Z');
			printf("%d \r",ZA);

			magXA = read_mag('X');
      printf("%d  ",magXA);

			magYA = read_mag('Y');
			printf("%d ",magYA);

			magZA = read_mag('Z');
			printf("%d \r",magZA);

      calibrateMagetometer(&magXA, &magYA, &magZA);
			sprintf(text, "%d,%d,%d,%d,%d,%d\r\n", XA, YA, ZA, magXA, magYA, magZA);
			USART0_Send(text);
			_delay_ms(10);
			//start();
			/*send_averageData();
			_delay_ms(10);*/
		}
	}
 }

 void calibrateMagetometer(int *x, int *y, int *z) {
   *x += -40;
   *y += 300;
 }

 /* void send_averageData(){
    vector a_avg = {0,0,0}, m_avg = {0,0,0};
    for(int i = 0; i < 5; i++){
      a_avg.x += read_acc('X');
			a_avg.y += read_acc('Y');
			a_avg.z += read_acc('Z');
			m_avg.x += read_mag('X');
			m_avg.y += read_mag('Y');
			m_avg.z += read_mag('Z');
    }
    a_avg.x /= 5;
    a_avg.y /= 5;
    a_avg.z /= 5;
    m_avg.x /= 5;
    m_avg.y /= 5;
    m_avg.z /= 5;

    sprintf(text, "%d,%d,%d,%d,%d,%d\r\n", a_avg.x, a_avg.y, a_avg.z, m_avg.x, m_avg.y, m_avg.z);
    USART0_Send(text);

  }*/

  void setup(void)
  {
	  // LSM303
	  cbi(DDRD,INT_1);		//input
	  cbi(DDRD,INT_2);
	  cbi(DDRD,DRDY);
	  //LED
	  sbi(DDRB,LEDGREEN);			// output, LED off
	  sbi(DDRB,LEDM);
	  //USART
	  sbi(DDRD,TXD);			// output
	  //button
	  cbi(DDRD,S2);			// input
	  sbi(PORTD,S2);			//pull-up resistor
  }

  void test_tl()
  {
	  while((PIND & (1<<S2)) == (1<<S2)){};	//while  H  wait
	  _delay_ms(100);
	  DDRB &=~ (1<<LEDM);
	  while((PIND & (1<<S2)) == 0){};	// while L wait
	  _delay_ms(100);
  }

