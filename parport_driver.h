/*
 *   parport_driver.h - Written by Sergey Kiselev
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

#define PARPORT_DATA	0x378
#define PARPORT_STATUS	0x379

void parport_init ();

/*
void SI (int si);
void SCK (int sck);
void CE (int ce);
int SO (void);
*/

/*
Parallel wiggle jtag cable interface.
*/

void TRST_N(int v);
void TDI(int v);
void TCK(int v);
void TMS(int v);
int TDO(void);
