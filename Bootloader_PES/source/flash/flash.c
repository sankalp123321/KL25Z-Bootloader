/**
 * @file flash.c
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief This file defines function that allow for flash operation
 *        to be possible.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "flash.h"
#include "MKL25Z4.h"

void Flash_StartFlashOperations()
{
    // Make sure that the flash operation is not affected by any other operation. 
	MCM->PLACR |= MCM_PLACR_ESFC_MASK;
    // Clear the flags
	FTFA->FSTAT = FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_RDCOLERR_MASK;
    // Enable the flash operation
	FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;
	while(!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)) {}
}

int Flash_erase(int sector)
{
    // divide the values into u8s 
    uint8_t byte_16 = (sector << 8) >> 24;
    uint8_t byte_8 = (sector << 16) >> 24;
    uint8_t byte_0 = (sector << 24) >> 24;

    // Set the register values
    FTFA->FCCOB0 = 9;
    FTFA->FCCOB1 = byte_16;
    FTFA->FCCOB2 = byte_8;
    FTFA->FCCOB3 = byte_0;

    // Kickstart the flash operation operation
    Flash_StartFlashOperations();
    return 1;
}

int Flash_write(int address, int value)
{
    // divide the values into u8s 
    uint8_t byte_16 = (address << 8) >> 24;
    uint8_t byte_8 = (address << 16) >> 24;
    uint8_t byte_0 = (address << 24) >> 24;

    uint8_t value_byte_24 = value >> 24;
    uint8_t value_byte_16 = (value << 8) >> 24;
    uint8_t value_byte_8 = (value << 16) >> 24;
    uint8_t value_byte_0 = (value << 24) >> 24;

    // Set the register values
    FTFA->FCCOB0 = 6;
    FTFA->FCCOB1 = byte_16;
    FTFA->FCCOB2 = byte_8;
    FTFA->FCCOB3 = byte_0;
    FTFA->FCCOB4 = value_byte_0;
    FTFA->FCCOB5 = value_byte_8;
    FTFA->FCCOB6 = value_byte_16;
    FTFA->FCCOB7 = value_byte_24;

    // Kickstart the flash operation operation
    Flash_StartFlashOperations();
    return 1;
}
