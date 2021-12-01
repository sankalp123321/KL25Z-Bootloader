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

uint8_t srec_line[128];
uint8_t cntr = 0;

void Load_SRECLine(uint8_t byte)
{
	srec_line[cntr++] = byte;

	if((byte == '\n') || (byte == '\r'))
	{
		LoadApp(NULL);
	}
}

void LoadApp(char *cmd_new)
{
//	printf("From LoadApp: %s\r\n", cmd_new);
//	for(uint8_t i =0; i < 127; i++)
//	{
//		printf("%02X", srec_line[i]);
//	}
//	printf("\r\n");

	srec_decoder(srec_line, cntr);

	memset(srec_line, 0, sizeof(srec_line));
	cntr = 0;
}
