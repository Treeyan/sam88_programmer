/*
 * sam88rcri.c
 * the programming timing based on https://download.csdn.net/download/nizhenyuchina/9511329
 *
 * this file is part of sam88_programmer.
 * the programmer for samsung sam88rcri family mcu. test on s3f9454 only
 * 
 * treeyan 7/16/2024
 * 
 * You should have received a copy of the GNU General Public License
 * along with sam88_programmer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/io.h>
#include "parport_driver.h"
#include "sam88chip.h"

#define SDA_H TDI(1)
#define SDA_L TDI(0)
#define SDI   TDO
#define SCL_H TCK(1)
#define SCL_L TCK(0)

static inline uchar_t uword_h(uword_t v)
{
    return (v >> 8) & 0xff;
}

static inline void ack() 
{
    SDA_H;
    SCL_H;
    // 忽略掉应答位
    SCL_L;
}

static inline uchar_t uword_l(uword_t v)
{
    return (uchar_t)(v & 0xff);
}

static inline void i2c_crazy()
{
    SDA_H;
    SCL_H;
    SDA_L;
    SCL_L;

    SCL_H;
    SDA_H;
    SCL_L;
}

static inline void sam88_core_reset(void)
{
    /*
     */
    int times = 6;

    while (times--)
    {
        i2c_crazy();
    }
}

void send_byte(uchar_t sbt)
{
    for (int i = 0; i < 8; i++)
    {
        if (sbt & (0x80 >> i))
            SDA_H;
        else
            SDA_L;

        SCL_H;
        // 发送一位 bit 到单片机
        SCL_L;
    }

    ack();
}

uchar_t recv_byte(void)
{
    uchar_t rbt;
    int     i;

    for (rbt = 0, i = 0; i < 8; i++)
    {
        rbt <<= 1;
        SCL_H;
        rbt |= SDI() ? 1 : 0;
        SCL_L;
    }

    ack();
    return rbt;
}

void sam88_chip_erase(void)
{
    sam88_core_reset();

    send_byte(0xe0);
    send_byte(0x15);
    send_byte(0x55);
    send_byte(0xaa);
}

void sam88_sector_write(uword_t addr, uchar_t *buff)
{
    sam88_core_reset();

    send_byte(0x60);
    // 先发送地址高位BYTE
    send_byte(uword_h(addr));
    send_byte(uword_l(addr));

    for (uword_t i = 0; i < SAMSEC_BYTES; i++)
    {
        send_byte(buff[i]);
    }
}

void sam88_sector_read(uword_t addr, uchar_t *buff)
{
    sam88_core_reset();

    send_byte(0x61);
    // 先发送地址高位BYTE
    send_byte(uword_h(addr));
    send_byte(uword_l(addr));
 
    for (uword_t i = 0; i < SAMSEC_BYTES; i++)
    {
        buff[i] = recv_byte();
    }
}
