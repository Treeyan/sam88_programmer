/*
 *   parport_driver.c - Written by Sergey Kiselev
 *
 *   This file is part of spi_programmer.
 *
 *   spi_programmer is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   spi_programmer is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with spi_programmer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/io.h>
#include "parport_driver.h"

static unsigned char data_reg = 0;

void parport_init()
{
    if ( ioperm( PARPORT_DATA, PARPORT_STATUS - PARPORT_DATA + 1, 1 ) ) {
        perror( "Can't get I/O privileges for parallel port" );
        exit( 1 );
    }
}

/*
void SI (int si)
{
	data_reg = (data_reg & 0xfe) | (0x1 ^ (0x1 & si));
	outb (data_reg, PARPORT_DATA);
}

void SCK (int sck)
{
	data_reg = (data_reg & 0xfd) | (0x2 ^ (0x2 & (sck << 1)));
	outb (data_reg, PARPORT_DATA);
}

void CE (int ce)
{
	data_reg = (data_reg & 0xfb) | (0x4 ^ (0x4 & (ce << 2)));
	outb (data_reg, PARPORT_DATA);
}

int SO (void)
{
	return ((inb (PARPORT_STATUS) >> 4) & 0x1) ^ 0x1;
}
*/

//
// treeyan. declare parallel wiggler jtag port 
//

#define TRST_N_SHIFT_N      4
#define TDI_SHIFT_N         3
#define TCK_SHIFT_N         2
#define TMS_SHIFT_N         1
#define TDO_SHIFT_N         7

#define ZERO_BIT(byt, idx)   (byt) &= ~(1 << (idx))


void mark_bit(int i, int v)
{
    ZERO_BIT(data_reg, i);
    data_reg |= (v &= 1) << i;
}

void TRST_N(int v)
{
    mark_bit(TRST_N_SHIFT_N, v);
    outb(data_reg, PARPORT_DATA);
}

void TDI(int v)
{
    mark_bit(TDI_SHIFT_N, v);
    outb(data_reg, PARPORT_DATA);
}

void TCK(int v) 
{
    mark_bit(TCK_SHIFT_N, v);
    outb(data_reg, PARPORT_DATA);
}

void TMS(int v)
{
    mark_bit(TMS_SHIFT_N, v);
    outb(data_reg, PARPORT_DATA);
}

int TDO(void)
{
    return ((inb(PARPORT_STATUS) >> TDO_SHIFT_N) & 1) ^ 1;
}

