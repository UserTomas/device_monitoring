#include "define.h"
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

void TWI_init_master(void); // Function to initialize master
void TWI_start(void);
void TWI_repeated_start(void);
void TWI_write_address(unsigned char data);
void TWI_read_address(unsigned char data);
void TWI_write_data(unsigned char data);
void TWI_stop(void);
uint8_t citanie_registra_ak(unsigned char adr);  // vysledok v ext premennej recv_data;
uint8_t citanie_registra_mag(unsigned char adr);
int read_acc(uint8_t os );  // os X, Y, Z;
int read_mag(uint8_t os);
void write_register_acc(unsigned char adr, unsigned char data); //0x20, 0x47;
void write_register_mag(unsigned char adr, unsigned char data);



/*void enable_accelerometer();
void enable_magnetometer();
char i2c_read_byte();
void i2c_stop();
char i2c_read_last_byte();
void i2c_start();
void i2c_write_byte(char byte);
void read_data(vector *a, vector *m);
void read_data_raw(vector *a, vector *m);
int get_heading(const vector *a, const vector *m, const vector *p);*/
