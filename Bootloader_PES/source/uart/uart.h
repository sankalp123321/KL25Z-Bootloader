/**
 * @file uart.h
 * @brief
 *
 *      This file provides functions for initializing the UART driver
 *      and also for sending and recv'ing the data over UART.
 *
 * @author Sankalp Agrawal
 * @date 2021-10-29
 * @version 0.1
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#define XON 0x11
#define XOFF 0x13
int IsXmmitBufferEmpty();
void SendXONOFF(uint8_t ch);
/**
 * @brief Initialize the UART driver.
 * @param baudrate specify the baudrate for the UART.
 */
void UART_Init(uint32_t baudrate);

/**
 * @brief Send 1 byte over the UART.
 * @param byte Byte to be sent.
 */
void UART_SendByte(uint8_t byte);

/**
 * @brief Send n bytes over the UART.
 * @param bytes Data to be sent.
 */
void UART_SendBytes(uint8_t* bytes);

/**
 * @brief printf wrapper for uart.
 * @param str Formatted string to be sent
 */
void UART_printf(const uint8_t * str, ...);

/**
 * @brief Recv'ing 1 character from the UART.
 * @param ch 1 byte to be recv'd from the UART.
 * @return successful or not.
 */
int UART_RecvChar(char* ch);

int UART_RecvByte(uint8_t* ch);

/**
 * @brief Send data by specifying number of bytes.
 * @param bytes Data to be sent.
 * @param count Bytes to be sent.
 */
void UART_SendByBytes(uint8_t* bytes, int count);

/**
 * @brief This function is called when a system printf
 *        is called. The implementation inside this function
 *        send data over the UART.
 * @param handle 0 stdout, 1 stderror
 * @param buf data buffer to be sent over UART.
 * @param size Number of bytes to be sent over the UART.
 * @return 0, 1 for sucess or failure.
 */
int __sys_write(int handle, char *buf, int size);

/**
 * @brief This function is called when a system input functions
 *        are called. This is a blocking call.
 * @return 1, -1 for sucess or failure.
 */
int __sys_readc(void);
#endif /* UART_H_ */
