/**
 * @file cbfifo.c
 * @author Sankalp Agrawal(saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2021-09-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "cbfifo.h"
#include <string.h>
#include "MKL25Z4.h"

void cbfifo_init(buffer_t *b, uint8_t *buffer, uint16_t sizeof_buf)
{
	b->gCircBuf = buffer;
	memset(b->gCircBuf, 0, sizeof_buf);
	b->totalCapacity = sizeof_buf;
	b->gHead_ptr = 0;
	b->gTail_ptr = 0;
	b->gTotalBuffElements = 0;
}

size_t __attribute__((section(".ramfunc"), long_call)) cbfifo_enqueue(buffer_t *b, void *buf, size_t nbyte)
{
    // If the input value is null, return -1
    if(buf == NULL) { return -1; }
    void *hBuf = buf;
    /**
     * To enqueue, we want to add the input to where
     * the head is pointing and then increment the head
     * to point to the next location.
     * 
     * This next location may or may not be free.
     *   - If the location is not free that means that we have reached the end of the buffer
     *     i.e. the buffer is full. Return a -1
     *   - If the location is empty then the next element to be inserted will be inserted
     *     without any complications.
     * 
     * The next location could also mean that the buffer is full.
     *   - If the buffer is full, then return 0 indicating that no element was written into the
     *     buffer.
     * 
     * The input to the buffer can be of any kind since the type is a void*
     * But at the lowest level we will use a unsigned byte to typecast it.
     * This allows any kind of input values to be put into the buffer byte-wise. 
     */
    size_t i = 0;
    for (i = 0; i < nbyte; i++)
    {
        // The buffer is full and no further elements can be added.
        if(b->gTotalBuffElements >= b->totalCapacity)
        {
        	b->gTotalBuffElements = b->totalCapacity;
            break;
        }

        // Each byte of the input value is stored in the buffer until
        // byte-wise until total number of bytes have been added.
        b->gCircBuf[b->gHead_ptr] = *((uint8_t*)hBuf);

        // Increment the buffer by uint8_t size
        hBuf = ((uint8_t*)hBuf) + 1;

        // Wrap the head pointer back to 0 position cause "circular buffer"
        b->gHead_ptr = (b->gHead_ptr + 1) % b->totalCapacity;
        NVIC_DisableIRQ(UART0_IRQn);
        // increment the element count after successful addition of element.
        b->gTotalBuffElements++;
        NVIC_EnableIRQ(UART0_IRQn);
    }

    // return the number of elements added to the circular buffer
    return i;
}

size_t __attribute__((section(".ramfunc"), long_call)) cbfifo_dequeue(buffer_t *b, void *buf, size_t nbyte)
{
    // If the input value is null, return -1
    if(buf == NULL) { return -1; }
    void *hBuf = buf;
    /**
     * To dequeue, we want to remove the element from where the tail
     * is pointing and return at element in the void* argument. We will
     * then increment the tail pointer to the next location.
     * 
     * This next location may or may not be free.
     *   - If the location is not free that means we can still take out elements
     *     from the buffer
     *   - If the location is empty that means the buffer is empty and we cannot
     *     take out any elements from the buffer.
     * 
     * The next location could also mean that the buffer is empty.
     *   - If the buffer is empty, then return 0 indicating that no element was taken out of the
     *     buffer
     * 
     * The circular buffer stores elements bytes-wise and hence, when take out
     * we follow the same convention. The number of bytes that need to be taken out
     * depends on the number of bytes requested to be taken out in the argument.
     */

    size_t i = 0;
    for (i = 0; i < nbyte; i++)
    {

        // gTotalBuffElements = 0, means that the buffer is empty. Cannot take out any element
        // This is not an error condition and hence we are returning a 0.
        if (!b->gTotalBuffElements) { break; }

        // Remove the element from the buffer and store in the variable by type casting it into
        // a uint8_t pointer. 

        *((uint8_t*)hBuf) = b->gCircBuf[b->gTail_ptr];
        b->gCircBuf[b->gTail_ptr] = 0;

        // The elements are removed and stored bytes-wise, so the next position for the next element
        // is one byte away from the current element. Hence, increment it by that much.
        hBuf = ((uint8_t*)hBuf) + 1;

        // Wrap the tail pointer back to 0 position cause "circular buffer"
        b->gTail_ptr = (b->gTail_ptr + 1) % b->totalCapacity;
        NVIC_DisableIRQ(UART0_IRQn);
        // Decrement the element count after successful addition of element.
        b->gTotalBuffElements--;
        NVIC_EnableIRQ(UART0_IRQn);
    }

    // return the number of elements removed to the circular buffer
    return i;
}

size_t cbfifo_length(buffer_t *buf)
{
//	NVIC_DisableIRQ(UART0_IRQn);
	size_t val = buf->gTotalBuffElements;
//	NVIC_EnableIRQ(UART0_IRQn);
    // return the number of elements in the circular buffer at any given time.
    return val;
}

size_t cbfifo_capacity(buffer_t *buf)
{
    // return the max capacity of the circular buffer.
    return buf->totalCapacity;
}
