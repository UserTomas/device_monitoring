#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SDA	PORTC4
#define SCL PORTC5
#define INT_1 PORTD2
#define INT_2 PORTD4
#define DRDY PORTD3			//data ready
//=================LED=========================================
#define LEDM PORTB0		//led red
#define LEDGREEN PORTB1		//led green
//==================UART=======================================
#define RXD PORTD0		//UART
#define TXD PORTD1
//==================SW UART=================================
#define SRXD PORTC3			//SW UART
#define STXD PORTC2
//===================TLACIDLO================================
#define S2 PORTD5			// Tlacidlo

#define BAUDERATE 9600
#define UBRR_VALUE ((F_CPU / (BAUDERATE * 8L)) -1)
#define B_SIZE 128			//<<<<<<<<<<<<<<dlzka buffer-a pre formatovany vstup<<<<<<
#define sbi(x,y) x |= _BV(y)	 //set bit     (1<<y)
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y)    //change bit - using bitwise XOR operator
//================LSM303=======================================
#define SDA	PORTC4
#define SCL PORTC5
#define INT_1 PORTD2
#define INT_2 PORTD4
#define DRDY PORTD3			//data ready
#define B_SIZE 128


