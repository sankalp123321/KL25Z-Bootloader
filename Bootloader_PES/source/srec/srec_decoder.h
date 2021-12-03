#ifndef _SREC_DECODER_
#define _SREC_DECODER_

#include <stdint.h>
#define BITS_TO_BYTES(x) ((x)>>1)
#define BYTES_TO_BITS(x) ((x)*2)
int srec_decoder(uint8_t* val, uint16_t nbytes);
int hex_int_equv(char hex);
#endif
