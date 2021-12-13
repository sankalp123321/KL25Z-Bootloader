/**
 * @file loadApp.c
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief The file provides definiton to the funtions
 *        for the erasing and loading the s record into the flash.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// C Standard lib
#include <stdio.h>
#include <string.h>
// Chip specific libs
#include "MKL25Z4.h"
// Application specific libs
#include "loadApp.h"
#include "bootApp.h"
#include "bootloader.h"
#include "../flash/flash.h"
#include "../uart/uart.h"
#include "../tpm/tpm.h"

#define BITS_TO_BYTES(x) ((x)>>1)
#define BYTES_TO_BITS(x) ((x)*2)

#define KL25Z_FLASH_BOUNDARY 0x20000
#define SECTOR_SIZE 0x400

static uint8_t gSrecLine[256];
static uint8_t cntr = 0;

void FlashErase()
{
	int eraseTopValue = KL25Z_FLASH_BOUNDARY - SECTOR_SIZE;
	// top value is 0x20000(128KB), sectors is in 1kb stretch. hence the last sector starts at 0x20000-0x400 = 0x1FC00
	for(uint32_t i = FIRMWARE_VERIFICATION; i < eraseTopValue; i+= SECTOR_SIZE)
	{
		Flash_erase(i);
	}
}

static uint32_t get_hex_equiv(uint8_t* val, uint16_t input_string_size, uint16_t pos, uint16_t num_bytes_required)
{
	if(num_bytes_required > input_string_size)
	{
		return 0;
	}
    uint32_t num = 0;
    char str[100] = " ";
    strncpy(str, (char* )&val[pos], num_bytes_required);
    sscanf(str, "%X", &num);
    return num;
}

void Load_SRECLine(uint8_t byte)
{
	gSrecLine[cntr++] = byte;

	if(byte == '\r')
	{
		uint16_t pos = 0;
    	uint32_t itr = 0;
    	uint8_t address_siz = 0; // in bits
    	uint32_t address = 0;
    	uint16_t data_size = 0;
    	uint16_t payload_size = 0;
    	uint8_t crc = 0;
    	pos = pos + 2;

    	payload_size = get_hex_equiv(gSrecLine, cntr, pos, 2);
    	crc = get_hex_equiv(gSrecLine, cntr, cntr-3, 2);

    	if(gSrecLine[1] == '0') { address_siz = 4; cntr = 0; return;}
    	else if(gSrecLine[1] == '1') { address_siz = 4; }
    	else if(gSrecLine[1] == '2') { address_siz = 6; }
    	else if(gSrecLine[1] == '3') { address_siz = 8; }
    	else if(gSrecLine[1] == '5') { address_siz = 0; }
    	else if(gSrecLine[1] == '7') { address_siz = 8; }
    	else if(gSrecLine[1] == '8') { address_siz = 6; }
    	else if(gSrecLine[1] == '9') {
    		address_siz = 4;
    		BootApp_WriteFirmwareVerificationArea();
    		Bootloader_SetState(eBOOTAPPL);
    	}

    	pos += 2;

    	address = get_hex_equiv(gSrecLine, cntr, pos, address_siz);
    	data_size = payload_size - BITS_TO_BYTES(address_siz) - sizeof(crc);

    	for (uint8_t i = 0; i < data_size; i+=4)
    	{
    		uint32_t data = get_hex_equiv(gSrecLine, cntr, pos + address_siz + itr, 2) << 24;
    		itr += 2;
    		data |= get_hex_equiv(gSrecLine, cntr, pos + address_siz + itr, 2) << 16;
    		itr += 2;
    		data |= get_hex_equiv(gSrecLine, cntr, pos + address_siz + itr, 2) << 8;
    		itr += 2;
    		data |= get_hex_equiv(gSrecLine, cntr, pos + address_siz + itr, 2);
    	    if(address >= BOOTLOADER_BOUNDARY)
    	    {
    	    	Flash_write(address+i, data);
    	    }
    	    else
    	    {
    	    	// Error condition.
    	    	Bootloader_SetState(eADDRESSCORRUPTERROR);
    	    	return;
    	    }
    	    itr += 2;
    	}
    	cntr = 0;
	}
}
