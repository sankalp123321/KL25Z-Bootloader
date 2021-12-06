#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint8_t srec_line[256];
uint8_t cntr = 0;

#define PRINT_DEBUG

#define BITS_TO_BYTES(x) ((x)>>1)
#define BYTES_TO_BITS(x) ((x)*2)

int hex_int_equv(char hex)
{
    if(hex <= '9')
    {
        return (hex - '0');
    }
    return (hex - 'A' + 10);
}

uint32_t get_hex_equiv(uint8_t* val, uint16_t input_string_size, uint16_t pos, uint16_t num_bytes_required)
{
	if(num_bytes_required > input_string_size)
	{
		return 0;
	}
    uint32_t num = 0;
    char str[100] = " ";
    strncpy(str, (char* )&val[pos], num_bytes_required);
    sscanf(str, "%X", &num);
    return num;
}

void Load_SRECLine(uint8_t byte)
{
//	int a = DisableGlobalIRQ();
//	printf("%c", byte);

	srec_line[cntr++] = byte;

	if(byte == '\n')
	{
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
    		// uint8_t data = get_hex_equiv(srec_line, cntr, pos + address_siz + itr, 2);
//    	    Flash_write(address+i, data);
    	    itr += 2;
    	}
#ifdef PRINT_DEBUG
    	printf("string_size[%d]bytes payload_size[0x%02X]bytes crc[0x%02X] s_type[%c]  address[0x%X] address_siz[%d]bytes\r\nData: ", cntr, payload_size, crc, srec_line[1], address, BITS_TO_BYTES(address_siz));
    	for (uint8_t i = 0; i < data_size; i++)
    	{
    	    printf("%02X ", data[i]);
    	}
    	printf("\r\n");
#endif
//    	for(uint8_t val = address; val < data_size; val++)
//    	{
//    		Flash_write(val, 30);
//    	}
    	cntr = 0;
	}
//	EnableGlobalIRQ(a);
}

int main()
{
    FILE *fp = NULL;
    fp = fopen("btldr_img.s19", "rb");

    while (1)
    {
        char ch = fgetc(fp);
        if(feof(fp)) break;
        Load_SRECLine(ch);
    }
    fclose(fp);
}