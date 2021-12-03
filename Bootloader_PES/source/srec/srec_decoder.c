
#include <stdio.h>
#include <string.h>
#include "srec_decoder.h"



int hex_int_equv(char hex)
{
    if((hex >= '0') && (hex <= '9'))
    {
        return (hex - '0');
    }
    else if((hex >= 'A') && (hex <= 'F'))
    {
        return (hex - 'A' + 10);
    }
    else if((hex >= 'a') && (hex <= 'f'))
    {
        return (hex - 'a' + 10);
    }
    return '-';
}

uint32_t get_hex_value(uint8_t* val, uint16_t input_string_size, uint16_t pos, uint16_t num_bytes_required)
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

int srec_decoder(uint8_t* val, uint16_t nbytes)
{
    uint16_t pos = 0;
    char s_type[3];
    memset(s_type, 0, sizeof(s_type));
    uint32_t itr = 0;
    uint8_t address_siz = 0; // in bits
    uint32_t address = 0;
    uint16_t data_size = 0;
    uint8_t data[100];
    uint16_t payload_size = 0;
    uint8_t crc = 0;

    strncpy(s_type, (char*)&val[pos], 2);

    pos = pos + 2;

    payload_size = get_hex_value(val, nbytes, pos, 2);
    crc = get_hex_value(val, nbytes, nbytes-3, 2);

    if(!strcmp(s_type, "S0")) { address_siz = 4; return 0;}
    else if(!strcmp(s_type, "S1")) { address_siz = 4; }
    else if(!strcmp(s_type, "S2")) { address_siz = 6; }
    else if(!strcmp(s_type, "S3")) { address_siz = 8; }
    else if(!strcmp(s_type, "S5")) { address_siz = 0; }
    else if(!strcmp(s_type, "S7")) { address_siz = 8; }
    else if(!strcmp(s_type, "S8")) { address_siz = 6; }
    else if(!strcmp(s_type, "S9")) { address_siz = 4; }
    else {}
    pos += 2;
    address = get_hex_value(val, nbytes, pos, address_siz);
    data_size = payload_size - BITS_TO_BYTES(address_siz) - sizeof(crc);
    for (uint8_t i = 0; i < data_size; i++)
    {
        data[i] = get_hex_value(val, nbytes, pos + address_siz + itr, 2);
        itr += 2;
    }

    printf("string_size[%d]bytes payload_size[0x%02X]bytes crc[0x%02X] s_type[%s]  address[0x%X] address_siz[%d]bytes\r\nData: ", nbytes, payload_size, crc, s_type, address, BITS_TO_BYTES(address_siz));
    for (uint8_t i = 0; i < data_size; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\r\n");
    memset(s_type, 0, sizeof(s_type));

    return 0;
}
