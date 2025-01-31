/*
 * main.c 
 * this file is part of sam88_programmer prj
 *
 * The programmer for samsung sam88rcri family mcu. 
 * by treeyan 7/16/2024
 */


#include <stdio.h>

#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdlib.h>     // make compiler happy.
#include <sys/stat.h>
#include <sys/ioctl.h>  // struct winsize
#include <ctype.h>

#include "parport_driver.h"
#include "sam88chip.h"

void usage(void)
{
    fprintf(stdout, "usage : sam88_programmer [command]\n"
             "  c                    erase chip.\n"
             "  p [filename]         write to mcu from file.\n"
             "  d [sector] [number]  dump data from mcu if eeprom can read.\n");
}

int console_w()
{
    struct winsize ws;

    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    return ws.ws_col;
}

void print_progress(int cur, int fin)
{
    static int wid = 0;
    int neq, nsp;
    uchar_t *equ;

    if (0 == wid)
    {
        wid = console_w() - 30;
    }

    if (0 >= wid)
        return;

    equ = malloc(wid);
    if (!equ)
        return;

    memset(equ, '=', wid);

    cur = cur * 100 / fin;
    neq = wid * cur / 100;
    nsp = wid - neq;

    printf("\r  Progress %3d%% : %.*s%c%*c", cur, neq, equ, '>', nsp, '|');

    if (100 == cur)
        printf("\n");

    fflush(stdout);
    free(equ);
}

void hex_dump_s(int sts, int num)
{
    uchar_t buff[SAMSEC_BYTES];
    int addr = SAMSEC_BYTES * sts;

    for (int i = 0; i < num; i++, addr += SAMSEC_BYTES)
    {
        sam88_sector_read(addr, buff);
        write(STDOUT_FILENO, buff, sizeof(buff));
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    parport_init();

    if (!strcmp(argv[1], "c"))
    {
        printf("erase chip.\n");
        sam88_chip_erase();
    }
    else if (!strcmp(argv[1], "d"))
    {
        int sector, number;

        if (argc < 4 || !isdigit(*argv[2]) || !isdigit(*argv[3]))
        {
            perror("wrong parameters\n");
            return 1;
        }
 
        sector = atol(argv[2]);
        number = atol(argv[3]);

        if (sector >= MAX_SECTOR)
        {
            fprintf(stderr, "error! sector of start be less < %d\n", MAX_SECTOR);
            return 1;
        }

        if (number > MAX_SECTOR)
        {
            fprintf(stderr, "error! sector of seek be less < %d\n", MAX_SECTOR);
            return 1;
        }

        hex_dump_s(sector, (sector + number) > MAX_SECTOR ? MAX_SECTOR - sector : number);
    }
    else if (!strcmp(argv[1], "p"))
    {
        int fd;
        int num;
        struct stat st;
        uchar_t *buff;

        if (argc < 3)
        {
            perror("file name missing\n");
            return 1;
        }

        if ((fd = open(argv[2], O_RDONLY)) == -1)
        {
            fprintf(stderr, "failed to open %s\n", argv[2]);
            return 2;
        }

        if (fstat(fd, &st) != 0)
        {
            perror("error to get file size.\n");
            return 3;
        }

        num = st.st_size;
        if (num & (SAMSEC_BYTES - 1)) 
        {
            perror("file not align.\n");
            return 4;
        }

        if (MAX_ROM_SIZE < num)
        { 
            perror("file too large.\n");
            return 5;
        }

        if ((buff = malloc(num)) == NULL)
        {
            perror("error memory\n");
            return 6;
        }

        memset(buff, 0xff, num);

        if (read(fd, buff, num) != st.st_size)
        {
            perror("error reading file\n");
            return 7;
        }

        for (ushort_t addr = 0;
             (print_progress((addr > num) ? num : addr, num), addr < num);
             addr += SAMSEC_BYTES, buff += SAMSEC_BYTES)
        {
            sam88_sector_write(addr, buff);
        }
    }
    else
    {
        perror("wrong parameters.\n");
        return 8;
    }

    return 0;
}
