/**
 * @file uart.c
 * @brief
 *
 *      This file provides functions for initializing the UART driver
 *      and also for sending and recv'ing the data over UART.
 *
 * @author Sankalp Agrawal
 * @date 2021-10-29
 * @version 0.1
 */

#include <stdarg.h>

#include "uart.h"
#include "MKL25Z4.h"

#include "../fifo/cbfifo.h"

#undef _NO_INTERRUPTS_

#define OSR 15
#define BUFFFER_SIZE 256

static buffer_t recv_buf;
uint8_t t_buf[BUFFFER_SIZE];
const int t_buf_siz = BUFFFER_SIZE;
static buffer_t xmit_buf;
uint8_t r_buf[BUFFFER_SIZE];
const int r_buf_siz = BUFFFER_SIZE;

void _xmit_status(uint8_t state) { UART0->C2 |= UART0_C2_TE(state); }
void _recv_status(uint8_t state) { UART0->C2 |= UART0_C2_RE(state); }

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		char a = UART0->D;
		cbfifo_enqueue(&recv_buf, &a, sizeof(char));
		return;
	}

	if(UART0->S1 & UART0_S1_TDRE_MASK)
	{
		char ch;
		if(cbfifo_dequeue(&xmit_buf, &ch, sizeof(char)) > 0)
		{
			UART0->D = ch;
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK;
			UART0->C2 |= UART0_C2_RIE_MASK;
		}
	}
}

void UART_Init(uint32_t baudrate)
{
// enable the uart clock
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
//	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM->SOPT5 &= ~SIM_SOPT5_UART0RXSRC_MASK | ~SIM_SOPT5_UART0TXSRC_MASK;

	_xmit_status(0);
	_recv_status(0);
	/*
	 * enable the alternate gpio
	 * PTA1 - alt2 - uart0_rx
	 * PTA2 - alt2 - uart0_tx
	 */
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx
	/*
	 * Set the baud rate of the UART
	 * baud rate is calculated using:
	 *     baudate clock
	 *    ---------------
	 *     (OSR + 1)*val
	 *
	 *  val = 1 to 8191
	 */
	UART0->BDH = 0;
	UART0->BDL = 0;
	UART0->C4 |= UART0_C4_OSR(OSR); // OSR = 9

	uint16_t sbr = SystemCoreClock/(baudrate*OSR);

	UART0->BDL |= UART0_BDL_SBR(sbr & 0xFF); // val = 125

	UART0->BDH |= UART0_BDH_SBNS(1) | UART0_BDH_SBR(sbr >> 8); // Set 2 stop bits
	UART0->C1 = 0; // No parity, 8 bit

	cbfifo_init(&recv_buf, r_buf, r_buf_siz);
	cbfifo_init(&xmit_buf, t_buf, t_buf_siz);

#ifndef _NO_INTERRUPTS_
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	// Enable interrupts
	UART0->C2 |= UART0_C2_RIE_MASK;
#endif

	_xmit_status(1);
	_recv_status(1);
}

void UART_SendByte(char byte)
{
#ifndef _NO_INTERRUPTS_
	cbfifo_enqueue(&xmit_buf, &byte, sizeof(char));
	UART0->C2 |= UART0_C2_TIE_MASK;
#endif
}

void UART_SendBytes(char* bytes)
{
#ifdef _NO_INTERRUPTS_
	uint16_t i = 0;
	while(i < count)
	{
		while(!(UART0bytes->S1 & UART0_S1_TDRE_MASK)){}
		UART0->D = bytes[i];
		i++;
	}
#else
	cbfifo_enqueue(&xmit_buf, bytes, string_len(bytes));
	UART0->C2 |= UART0_C2_TIE_MASK;
#endif
}

void UART_SendByBytes(char* bytes, int count)
{
#ifdef _NO_INTERRUPTS_
	uint16_t i = 0;
	while(i < count)
	{
		while(!(UART0bytes->S1 & UART0_S1_TDRE_MASK)){}
		UART0->D = bytes[i];
		i++;
	}
#else
	cbfifo_enqueue(&xmit_buf, bytes, count);
	UART0->C2 |= UART0_C2_TIE_MASK;
#endif
}

void UART_printf(const char* str, ...)
{
	char s[256];
	__builtin_memset(s, 0, sizeof(256));
	va_list arguments;
	va_start(arguments, str);
	vsprintf(s, str, arguments);
	va_end(arguments);                  // Cleans up the list

	UART_SendBytes(s);
}

int UART_RecvChar(char* ch)
{
	return cbfifo_dequeue(&recv_buf, ch, sizeof(char));
}

int __sys_write(int handle, char *buf, int size)
{
	UART_SendByBytes(buf, size);
	return 0;
}

int __sys_readc(void)
{
	while(!(UART0->S1 & UART0_S1_RDRF_MASK))
	{
		char a = UART0->D;
		return cbfifo_enqueue(&recv_buf, &a, sizeof(char));
	}
	return -1;
}
