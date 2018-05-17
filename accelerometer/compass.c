#include <avr/io.h>
//#include <pololu/orangutan.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "i2c.h"
#include "compass.h"
#include "serial.h"
#include "define.h"

/*
 * This program assumes that the LSM303DLH carrier is oriented with X pointing
 * to the right, Y pointing backward, and Z pointing down (toward the ground).
 * The code compensates for tilts of up to 90 degrees away from horizontal.
 * Vector p should be defined as pointing forward, parallel to the ground,
 * with coordinates {X, Y, Z}.
 */
vector p = {0, -1, 0};

/*
 * m_max and m_min are calibration values for the maximum and minimum
 * measurements recorded on each magnetic axis, which can vary for each
 * LSM303DLH. You should replace the values below with max and min readings from
 * your particular device.
 *
 * To obtain the max and min values, you can use this program's
 * calibration mode, which is enabled by pressing one of the pushbuttons. While
 * calibration mode is active, point each of the axes of the LSM303DLH toward
 * and away from the earth's North Magnetic Pole. Due to space constraints on an
 * 8x2 LCD, only one axis is displayed at a time; each button selects an axis to
 * display (top = X, middle = Y, bottom = Z), and pressing any button a second
 * time exits calibration mode and returns to normal operation.
 */
vector m_max = {340, 517, 416};
vector m_min = {-642, -496, -462};

char* ribbon = "   N   NE   E   SE   S   SW   W   NW   N   ";

enum calibration_mode {CAL_NONE, CAL_X, CAL_Y, CAL_Z};

// Returns a set of acceleration and raw magnetic readings from the cmp01a.
void read_data_raw(vector *a, vector *m)
{
	// read accelerometer and magnetometer values
	a->x = read_acc('X');
	a->y = read_acc('Y');
	a->z = read_acc('Z');
	m->x = read_mag('X');
	m->y = read_mag('Y');
	m->z = read_mag('Z');
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

void start()
{

	vector a, m;
	char ribbon_segment[8];
	unsigned char button;
	enum calibration_mode mode = CAL_NONE;
	vector cal_m_max = {0, 0, 0};
	vector cal_m_min = {0, 0, 0};
	char txt[16];

  	while(1)
	{
		if (mode == CAL_NONE) // normal mode - display heading and compass ribbon
		{
			vector a_avg = {0,0,0}, m_avg = {0,0,0};

			// take 5 acceleration and magnetic readings and average them
			for(int i = 0; i < 5; i++)
			{
				read_data(&a, &m);

				a_avg.x += a.x;
				a_avg.y += a.y;
				a_avg.z += a.z;
				m_avg.x += m.x;
				m_avg.y += m.y;
				m_avg.z += m.z;
			}
			a_avg.x /= 5;
			a_avg.y /= 5;
			a_avg.z /= 5;
			m_avg.x /= 5;
			m_avg.y /= 5;
			m_avg.z /= 5;

			int heading = get_heading(&a_avg, &m_avg, &p);

			// select the portion of the ribbon to display
			strlcpy(ribbon_segment, &ribbon[(heading + 5) / 10], 8);

/*			print_long(heading);
			lcd_goto_xy(4, 0);
			print_character('v');  // center indicator
			lcd_goto_xy(1, 1);
			print(ribbon_segment); // ribbon segment*/
      sprintf(txt, "%i", heading);
			USART0_Send(txt);
			USART0_Send("\t");
			USART0_Send(ribbon_segment);
			USART0_Send("\r\n");
		}
		/*else // calibration mode - record and display max/min measurements
		{
			read_data_raw(&a, &m);
			if (m.x < cal_m_min.x) cal_m_min.x = m.x;
			if (m.x > cal_m_max.x) cal_m_max.x = m.x;
			if (m.y < cal_m_min.y) cal_m_min.y = m.y;
			if (m.y > cal_m_max.y) cal_m_max.y = m.y;
			if (m.z < cal_m_min.z) cal_m_min.z = m.z;
			if (m.z > cal_m_max.z) cal_m_max.z = m.z;

			clear();

			switch (mode)
			{
				case CAL_X:
					print("Xmax:");
					print_long(cal_m_max.x);
					lcd_goto_xy(0, 1);
					print("min:");
					print_long(cal_m_min.x);
					break;
				case CAL_Y:
					print("Ymax:");
					print_long(cal_m_max.y);
					lcd_goto_xy(0, 1);
					print("min:");
					print_long(cal_m_min.y);
					break;
				default:
					print("Zmax:");
					print_long(cal_m_max.z);
					lcd_goto_xy(0, 1);
					print("min:");
					print_long(cal_m_min.z);
					break;
			}
		}*/

		_delay_ms(20);
	}
}

