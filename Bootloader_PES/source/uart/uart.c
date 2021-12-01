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
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../fifo/cbfifo.h"

#define XON 0x11
#define XOFF 0x13

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

static void SendXONOFF(uint8_t ch) {
  unsigned char buf[2];

  buf[0] = ch;
  buf[1] = '\0';
  UART_SendByBytes(buf, sizeof(buf));
}

void UART0_IRQHandler()
{
//	int send_xoff = 0;
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		uint8_t a = UART0->D;
//		printf("%c", a);
		cbfifo_enqueue(&recv_buf, &a, sizeof(uint8_t));

//		if((recv_buf.gTotalBuffElements+2) == recv_buf.totalCapacity)
//		{
//			send_xoff = 1;
//		}
//		else
//		{
//			send_xoff = 0;
//			return;
//		}
		return;
	}

	if(UART0->S1 & UART0_S1_TDRE_MASK)
	{
//		if(send_xoff)
//		{
//			UART0->D = XOFF;
//			return;
//		}
		uint8_t ch;
		if(cbfifo_dequeue(&xmit_buf, &ch, sizeof(uint8_t)) > 0)
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

	UART0->BDH |= UART0_BDH_SBR(sbr >> 8); // Set 2 stop bits
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

void UART_SendByte(uint8_t byte)
{
#ifndef _NO_INTERRUPTS_
	cbfifo_enqueue(&xmit_buf, &byte, sizeof(uint8_t));
	UART0->C2 |= UART0_C2_TIE_MASK;
#endif
}

void UART_SendBytes(uint8_t* bytes)
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
	cbfifo_enqueue(&xmit_buf, bytes, strlen((char*)bytes));
	UART0->C2 |= UART0_C2_TIE_MASK;
#endif
}

void UART_SendByBytes(uint8_t* bytes, int count)
{
	uint16_t i = 0;
	while(i < count)
	{
		while(!(UART0->S1 & UART0_S1_TDRE_MASK)){}
		UART0->D = bytes[i];
		i++;
	}
}

void UART_printf(const uint8_t* str, ...)
{
	char s[256];
	__builtin_memset(s, 0, sizeof(256));
	va_list arguments;
	va_start(arguments, str);
	vsprintf(s, (char*)str, arguments);
	va_end(arguments);                  // Cleans up the list

	UART_SendBytes((uint8_t*)s);
}

int UART_RecvChar(char* ch)
{
	return cbfifo_dequeue(&recv_buf, ch, sizeof(uint8_t));
}

int UART_RecvByte(uint8_t* ch)
{
	return cbfifo_dequeue(&recv_buf, ch, sizeof(uint8_t));
}

int __sys_write(int handle, char *buf, int size)
{
	int i = 0;
	while (i < size) { // copy characters up to null terminator
		while (cbfifo_length(&xmit_buf) == cbfifo_capacity(&xmit_buf)); // wait for space to open up
		cbfifo_enqueue(&xmit_buf, &buf[i], 1);
		i++;
	}

	UART0->C2 |= UART0_C2_TIE_MASK;
	return 0;
}

int __sys_readc(void)
{
	while(!(UART0->S1 & UART0_S1_RDRF_MASK))
	{
		uint8_t a = UART0->D;
		return cbfifo_enqueue(&recv_buf, &a, sizeof(uint8_t));
	}
	return -1;
}
