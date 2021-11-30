/*
 * flash.c
 *
 *  Created on: 16-Nov-2021
 *      Author: Sankalp
 */
#include "flash.h"
#include "MKL25Z4.h"

int Flash_erase(int sector)
{
    uint8_t byte_16 = (sector << 8) >> 24;
    uint8_t byte_8 = (sector << 16) >> 24;
    uint8_t byte_0 = (sector << 24) >> 24;
    while(!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)) {}

    if((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) ||
    (FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK))
    {
        FTFA->FSTAT = 0x30;
    }

    FTFA->FCCOB0 = 9;
    FTFA->FCCOB1 = byte_16;
    FTFA->FCCOB2 = byte_8;
    FTFA->FCCOB3 = byte_0;

    FTFA->FSTAT |= FTFA_FSTAT_CCIF_MASK;
    FTFA->FSTAT = 0x80;

    while(!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)) {}
    return 1;
}

int Flash_read(int sector, int* value)
{

}

int Flash_write(int sector, int value)
{
    uint8_t byte_16 = (sector << 8) >> 24;
    uint8_t byte_8 = (sector << 16) >> 24;
    uint8_t byte_0 = (sector << 24) >> 24;

    uint8_t value_byte_24 = value >> 24;
    uint8_t value_byte_16 = (value << 8) >> 24;
    uint8_t value_byte_8 = (value << 16) >> 24;
    uint8_t value_byte_0 = (value << 24) >> 24;
    while(!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)) {}

    if((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) ||
    (FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK))
    {
        FTFA->FSTAT = 0x30;
    }

    FTFA->FCCOB0 = 6;
    FTFA->FCCOB1 = byte_16;
    FTFA->FCCOB2 = byte_8;
    FTFA->FCCOB3 = byte_0;
    FTFA->FCCOB4 = value_byte_0;
    FTFA->FCCOB5 = value_byte_8;
    FTFA->FCCOB6 = value_byte_16;
    FTFA->FCCOB7 = value_byte_24;

    FTFA->FSTAT |= FTFA_FSTAT_CCIF_MASK;
    FTFA->FSTAT = 0x80;

    while(!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)) {}
    return 1;
}