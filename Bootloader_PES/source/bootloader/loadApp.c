/*
 * loadApp.c
 *
 *  Created on: 29-Nov-2021
 *      Author: Sankalp
 */
#include <stdio.h>
#include <string.h>
#include "loadApp.h"
#include "../srec/srec_decoder.h"

uint8_t srec_line[256];
uint8_t cntr = 0;

uint32_t get_hex_equiv(uint8_t* val, uint16_t input_string_size, uint16_t pos, uint16_t num_bytes_required)
{
    uint32_t num = 0;
    char str[100] = " ";
    strncpy(str, (char* )&val[pos], num_bytes_required);
    sscanf(str, "%X", &num);
//    memcpy(num, &val[pos], num_bytes_required);
//    for(int i = 0; i < (num_bytes_required); i++)
//    {
//    	num |= hex_int_equv(val[pos + i]);
//    	num <<= 4;
//    }
//    num>>=4
    return num;
}

void Load_SRECLine(uint8_t byte)
{
//	printf("%c", byte);
	srec_line[cntr++] = byte;

	// static uint8_t isHeaderRcvd = 0;
	// static uint8_t rcvAddress = 0;
	// static uint8_t addr_buffer[8] = 0;
	// static uint8_t addr_cntr = 0;
	// static uint8_t byte_form = 0;
	// uint8_t address_siz = 0;
	// uint8_t rcvByte = hex_int_equv(byte);

	// if(!isHeaderRcvd && byte == 'S')
	// {
	// 	isHeaderRcvd = 0;
	// 	return;
	// }
	// else if(!isHeaderRcvd)
	// {
	// switch(rcvByte)
	// {
	// case 0:
	// 	address_siz = 4;
	// 	break;
	// case 1:
	// 	address_siz = 4;
	// 	break;
	// case 2:
	// 	address_siz = 6;
	// 	break;
	// case 3:
	// 	address_siz = 8;
	// 	break;
	// case 5:
	// 	address_siz = 0;
	// 	break;
	// case 7:
	// 	address_siz = 8;
	// 	break;
	// case 8:
	// 	address_siz = 6;
	// 	break;
	// case 9:
	// 	address_siz = 4;
	// 	break;
	// };
	// isHeaderRcvd = 1;
	// }

	// if(rcvAddress)
	// {
	// 	if(!byte_form)
	// 	{
	// 		addr_buffer[addr_cntr] = (rcvByte << 4);
	// 		byte_form = 1;
	// 	}
	// 	else
	// 	{
	// 		addr_buffer[addr_cntr] = (rcvByte << 4) | rcvByte;
	// 		byte_form = 0; 
	// 	}
	// }

	if(byte == '\n' || byte == '\r')
	{
		if(cntr < 75)
		{
			printf("Oops\r\n");
		}
		uint16_t pos = 0;
//    	char s_type[3];
//    	memset(s_type, 0, sizeof(s_type));
    	uint32_t itr = 0;
    	uint8_t address_siz = 0; // in bits
    	uint32_t address = 0;
    	uint16_t data_size = 0;
    	uint8_t data[100];
    	uint16_t payload_size = 0;
    	uint8_t crc = 0;
    	pos = pos + 2;

    	payload_size = get_hex_equiv(srec_line, cntr, pos, 2);
    	crc = get_hex_equiv(srec_line, cntr, cntr-3, 2);

    	if(srec_line[1] == '0') { address_siz = 4;}
    	else if(srec_line[1] == '1') { address_siz = 4; }
    	else if(srec_line[1] == '2') { address_siz = 6; }
    	else if(srec_line[1] == '3') { address_siz = 8; }
    	else if(srec_line[1] == '5') { address_siz = 0; }
    	else if(srec_line[1] == '7') { address_siz = 8; }
    	else if(srec_line[1] == '8') { address_siz = 6; }
    	else if(srec_line[1] == '9') { address_siz = 4; }
    	else {}
    	pos += 2;
    	address = get_hex_equiv(srec_line, cntr, pos, address_siz);
    	data_size = payload_size - BITS_TO_BYTES(address_siz) - sizeof(crc);
    	for (uint8_t i = 0; i < data_size; i++)
    	{
    	    data[i] = get_hex_equiv(srec_line, cntr, pos + address_siz + itr, 2);
    	    itr += 2;
    	}

//    	printf("string_size[%d]bytes payload_size[0x%02X]bytes crc[0x%02X] s_type[%c]  address[0x%X] address_siz[%d]bytes\r\nData: ", cntr, payload_size, crc, srec_line[1], address, BITS_TO_BYTES(address_siz));
//    	for (uint8_t i = 0; i < data_size; i++)
//    	{
//    	    printf("%02X ", data[i]);
//    	}
//    	printf("\r\n");

//    	memset(srec_line, 0, sizeof(srec_line));
    	cntr = 0;
	}

}

void LoadApp(char *cmd_new)
{
//	printf("From LoadApp: %s\r\n", cmd_new);
//	for(uint8_t i =0; i < 127; i++)
//	{
//		printf("%c", srec_line[i]);
//	}
//	printf("\r\n");
//
//	if(cntr != 75)
//	{
//		(void)cmd_new;
//	}

//	srec_decoder(srec_line, cntr);

	// memset(srec_line, 0, sizeof(srec_line));
	cntr = 0;
}
