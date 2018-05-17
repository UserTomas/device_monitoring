#include "i2c.h"

unsigned char sla=0x32, read=1, write=0;
unsigned char sla_mag=0x3C;
unsigned char write_data=0x01, recv_data;


void TWI_init_master(void) // Function to initialize master
{
  TWBR=52;  // Bit rate 100 kHz
  TWSR=(0<<TWPS1)|(0<<TWPS0); // Setting prescaler bits
}

     void TWI_start(void)
     {
	     // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	     TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	     while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
	     while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgement
     }

     void TWI_repeated_start(void)
     {
	     // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	     TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	     while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	     while((TWSR & 0xF8)!= 0x10); // Check for the acknowledgement
     }

     void TWI_write_address(unsigned char data)
     {
	     TWDR=data;  // Address and write instruction
	     TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	    while((TWSR & 0xF8)!= 0x18);  // Check for the acknowledgement   0x18 ACK  0x20  NACK
     }

     void TWI_read_address(unsigned char data)
     {
	     TWDR=data;  // Address and read instruction
	     TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	     while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
	     while((TWSR & 0xF8)!= 0x40);  // Check for the acknowledgement
     }

     void TWI_write_data(unsigned char data)
     {
	     TWDR=data;  // put data in TWDR
	     TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	     while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	     while((TWSR & 0xF8) != 0x28); // Check for the acknowledgement
     }

   uint8_t TWI_read_data(void)				// uint8_t
     {
	     TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	     while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	     while((TWSR & 0xF8) != 0x58); // Check for the acknowledgement
	    // recv_data=TWDR;
	    // data=recv_data;
		return(TWDR);
     }

     void TWI_stop(void)
     {
	     // Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	     TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	     while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
     }

uint8_t citanie_registra_ak(unsigned char adr)  // vysledok v ext premennej recv_data
{
	 TWI_start();						// Function to send start condition  scl=0
	 TWI_write_address(sla+write);    //  adresa akcelerometra
	 TWI_write_data(adr);				 //adresa registra
	//  _delay_ms(1);
	 TWI_repeated_start();			// opakovany start
	 TWI_read_address(sla+read); // zapis adresy akcelerometra a podmienka citania
	uint8_t pom = TWI_read_data();  //TWI_read_data();				// citanie dat vysledok v ext.premennej recv_data
	 TWI_stop();
	 return(pom);
}

int read_acc(uint8_t os )  // os X, Y, Z
{
	uint8_t ad = 0x29;
	int X;
	if(os == 'Y') ad=0x2b;
	if(os == 'Z') ad=0x2d;
	X =citanie_registra_ak(ad);
	ad=ad-1;
	X = X<<8;
	_delay_us(10);
	X = X|citanie_registra_ak(ad);  //0xad
	_delay_us(10);
	return(X);
}

uint8_t citanie_registra_mag(unsigned char adr)  // vysledok v ext premennej recv_data
{
	 TWI_start();						// Function to send start condition  scl=0
	 TWI_write_address(sla_mag+write);    //  adresa akcelerometra
	 TWI_write_data(adr);				 //adresa registra
	//  _delay_ms(1);
	 TWI_repeated_start();			// opakovany start
	 TWI_read_address(sla_mag+read); // zapis adresy akcelerometra a podmienka citania
	uint8_t pom = TWI_read_data();  //TWI_read_data();				// citanie dat vysledok v ext.premennej recv_data
	 TWI_stop();
	 return(pom);
}

int read_mag(uint8_t os) {
  uint8_t ad = 0x03;
	int X;
	if(os == 'Z') ad=0x05;
	if(os == 'Y') ad=0x07;
	X =citanie_registra_mag(ad);
	ad=ad+1;
	X = X<<8;
	_delay_us(10);
	X = X|citanie_registra_mag(ad);
	_delay_us(10);
	return(X);
}


void write_register_acc(unsigned char adr, unsigned char data) //0x20, 0x47
{
 TWI_start();						// Function to send start condition  scl=0
 TWI_write_address(sla+write);    //  adresa akcelerometra sla=0x32 write=0x00
 TWI_write_data(adr);				 //adresa registra
 TWI_write_data(data);				//data
 TWI_stop();
}

void write_register_mag(unsigned char adr, unsigned char data) //0x02, 0x00
{
 TWI_start();						// Function to send start condition  scl=0
 TWI_write_address(sla_mag+write);    //  adresa magnetometra sla_mag=0x3C write=0x00
 TWI_write_data(adr);				 //adresa registra
 TWI_write_data(data);				//data
 TWI_stop();
}

//---------------------------I2C------------------------------------------------
/*
void enable_accelerometer() {
 TWI_start();						// Function to send start condition  scl=0
 TWI_write_address(sla+write);    //  adresa akcelerometra sla=0x32 write=0x00
 TWI_write_data(0x20);				 //adresa registra
 TWI_write_data(0x47);				//data
 TWI_stop();
}

void enable_magnetometer() {
 TWI_start();						// Function to send start condition  scl=0
 TWI_write_address(0x3C);    //  adresa magnetomerta
 TWI_write_data(0x02);				 //adresa registra
 TWI_write_data(0x00);				//data
 TWI_stop();
}


void read_data_raw(vector *a, vector *m)
{
	// read accelerometer values
	TWI_start();
	i2c_write_byte(0x30); // write acc
	i2c_write_byte(0xa8); // OUT_X_L_A, MSB set to enable auto-increment
	i2c_start();		  // repeated start
	i2c_write_byte(0x31); // read acc
	unsigned char axl = i2c_read_byte();
	unsigned char axh = i2c_read_byte();
	unsigned char ayl = i2c_read_byte();
	unsigned char ayh = i2c_read_byte();
	unsigned char azl = i2c_read_byte();
	unsigned char azh = i2c_read_last_byte();
	i2c_stop();

	// read magnetometer values
	i2c_start();
	i2c_write_byte(0x3C); // write mag
	i2c_write_byte(0x03); // OUTXH_M
	i2c_start();		  // repeated start
	i2c_write_byte(0x3D); // read mag
	unsigned char mxh = i2c_read_byte();
	unsigned char mxl = i2c_read_byte();
	unsigned char myh = i2c_read_byte();
	unsigned char myl = i2c_read_byte();
	unsigned char mzh = i2c_read_byte();
	unsigned char mzl = i2c_read_last_byte();
	i2c_stop();

	a->x = axh << 8 | axl;
	a->y = ayh << 8 | ayl;
	a->z = azh << 8 | azl;
	m->x = mxh << 8 | mxl;
	m->y = myh << 8 | myl;
	m->z = mzh << 8 | mzl;
}
// Returns a set of acceleration and adjusted magnetic readings from the cmp01a.
void read_data(vector *a, vector *m)
{
	read_data_raw(a, m);

	// shift and scale
	m->x = (m->x - m_min.x) / (m_max.x - m_min.x) * 2 - 1.0;
	m->y = (m->y - m_min.y) / (m_max.y - m_min.y) * 2 - 1.0;
	m->z = (m->z - m_min.z) / (m_max.z - m_min.z) * 2 - 1.0;
}

// Returns a heading (in degrees) given an acceleration vector a due to gravity, a magnetic vector m, and a facing vector p.
int get_heading(const vector *a, const vector *m, const vector *p)
{
	vector E;
	vector N;

	// cross magnetic vector (magnetic north + inclination) with "down" (acceleration vector) to produce "east"
	vector_cross(m, a, &E);
	vector_normalize(&E);

	// cross "down" with "east" to produce "north" (parallel to the ground)
	vector_cross(a, &E, &N);
	vector_normalize(&N);

	// compute heading
	int heading = round(atan2(vector_dot(&E, p), vector_dot(&N, p)) * 180 / M_PI);
	if (heading < 0)
		heading += 360;
	return heading;
}


void i2c_start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send start condition
	while (!(TWCR & (1 << TWINT)));
}

void i2c_write_byte(char byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN); // start address transmission
	while (!(TWCR & (1 << TWINT)));
}

char i2c_read_byte() {
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // start data reception, transmit ACK
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

char i2c_read_last_byte() {
	TWCR = (1 << TWINT) | (1 << TWEN); // start data reception
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void i2c_stop() {
	  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // send stop condition
}
*/
//---------------------------/I2C------------------------------------------------

