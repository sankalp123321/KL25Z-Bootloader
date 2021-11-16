/*
 * cbfifo.h - a fixed-size FIFO implemented via a circular buffer
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * 
 */

#ifndef _CBFIFO_H_
#define _CBFIFO_H_

#include <stdlib.h>  // for size_t
#include <stdint.h>

struct buffer {
	// Array for the buffer
	uint8_t *gCircBuf;
	uint16_t totalCapacity;
	// Variable to keep a track of total buff elements
	uint16_t gTotalBuffElements;
	// Tracking the head and the tail pointer of the buffer
	uint16_t gHead_ptr;
	uint16_t gTail_ptr;
};
typedef struct buffer buffer_t;

void cbfifo_init(buffer_t *b, uint8_t *buffer, uint16_t sizeof_buf);

/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buffer_t * buffer pointer
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 * 
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */
size_t cbfifo_enqueue(buffer_t *b, void *buf, size_t nbyte);


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buffer_t * buffer pointer
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 * 
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte. 
 * 
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(buffer_t *b, void *buf, size_t nbyte);


/*
 * Returns the number of bytes currently on the FIFO. 
 *
 * Parameters:
 *   buffer_t * buffer pointer
 * 
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(buffer_t *buf);


/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   buffer_t * buffer pointer
 * 
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity(buffer_t *buf);


#endif // _CBFIFO_H_
