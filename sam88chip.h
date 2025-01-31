
#ifndef H_SAM88RCRI_SAMSUNG_MCU_PROGRAMMER
#define H_SAM88RCRI_SAMSUNG_MCU_PROGRAMMER

enum
{
    false,
    true,
};

typedef char bool_t;

typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef ushort_t uword_t;

#define SAMSEC_BYTES (128)
#define MAX_ROM_SIZE (64 * 1024)
#define MAX_SECTOR (MAX_ROM_SIZE / SAMSEC_BYTES)

void sam88_chip_erase(void);
void sam88_sector_write(uword_t addr, uchar_t *buff);
void sam88_sector_read(uword_t addr, uchar_t *buff);

#endif // H_SAM88RCRI_SAMSUNG_MCU_PROGRAMMER
