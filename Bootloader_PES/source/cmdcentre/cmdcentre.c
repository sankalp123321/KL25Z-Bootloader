/**
 * @file cmdcentre.c
 * @brief
 *
 *      This file provides functions for running the cmd engine
 *      and also cmd parser.
 *
 * @author Sankalp Agrawal
 * @date 2021-10-29
 * @version 0.1
 */
#include "cmdcentre.h"
#include "../uart/uart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct command_t
{
	char *cmd;
	const char *args;
	const char *desc;
	void (*fp)(int argc, char* argv[]);
}command_t;

// Command table
command_t cmd[] =
{
		// Command | Arguments | Command description | command function handler
		{"Help", "<none>", "help command view", CmdCentre_HelpHandler},
		{"author", "<none>", "displays the author name", CmdCentre_Author},
};

const int cmd_size = sizeof(cmd)/sizeof(cmd[0]);

void CmdCentre_HelpHandler(int argc, char* argv[])
{
	// Hex Dump
	uint8_t t = 0;
	printf("%-10s %-24s %s\r\n", "Command", "Arguments", "Description");
	printf("---------------------------------------------------\r\n");
	while(t < cmd_size)
	{
		printf("%-10s %-24s %s\r\n", cmd[t].cmd, cmd[t].args, cmd[t].desc);
		t++;
	}
	printf("\r\n");
}

void CmdCentre_Author(int argc, char* argv[])
{
	// print author name
	printf("Sankalp Agrawal\r\n");
}



int CmdCentre_WordEngine(char *cmd_new)
{
	char ch;
	static int count = 0;
	if(UART_RecvChar(&ch) > 0)
	{
		/**
		 * Send the recv'd character from the UART to the
		 * terminal.
		 */
		printf("%c", ch);
		/**
		 * if there is a backspace detected, then delete the last character from the
		 * uart. Reduce the last character from the buffer.
		 */
		if(ch == 0x08) // backspace
		{
			char ch;
			UART_RecvChar(&ch);
			printf(" \b");
			count--;
			return 0;
		}
		/**
		 * Add each inputed character from the UART into the buffer.
		 */
		cmd_new[count++] = ch;

		/**
		 * When enter is pressed then we know a certain command entering is
		 * complete and we need to process that.
		 */
		if((ch == '\n') || (ch == '\r'))
		{
			count = 0;
			return 1;
		}
	}
	return 0;
}

void CmdCentre_CommandEngine(char *cmd_new)
{
	uint8_t t = 0;
	while(t < cmd_size)
	{
		/**
		 * This function will compare string and it is case insensitive.
		 */
		if(!strcmpi(cmd[t].cmd, cmd_new))
		{
			char* argv[10];
			printf("\r\n");
			uint8_t i = _de_tokentizer(cmd_new, argv);
			cmd[t].fp(i, argv);
			break;
		}
		t++;
	}
	// if the the 't' counter reaches the end, that means
	// the command is not present.
	if((t == cmd_size) && (strlen(cmd_new) > 1))
	{
		printf("\r\nUnknown Command: %s\r\n", cmd_new);
	}
	printf("? ");
}
