/**
 * @file bootApp.c
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief This file provdes defition to the functions that 
 *        allow the program to boot the new firmware from the flash.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
// Application specific lib
#include "bootApp.h"
#include "MKL25Z4.h"
#include "loadApp.h"
#include "../flash/flash.h"
#include "../tpm/tpm.h"
#include "../uart/uart.h"

#define FIRMWARE_VERIFICATION_SIGNATURE 0xAA55

#define REVBYTE(X)((((((X)<<8)|((X)>>8)))&0xFFFF) << 16)

void BootApp_WriteFirmwareVerificationArea()
{
	Flash_erase(FIRMWARE_VERIFICATION);
	Flash_write(FIRMWARE_VERIFICATION, FIRMWARE_VERIFICATION_SIGNATURE);
}

uint8_t BootApp_IsFirmwareVerificationPresent()
{
	uint32_t val = *((unsigned int*)FIRMWARE_VERIFICATION);
	if(val == REVBYTE(FIRMWARE_VERIFICATION_SIGNATURE))
	{
		return 1;
	}
	else return 0;
}


/**
 * @brief This method boots the firmware image present on
 *        the bootloader bounday location.
 *
 * @param app_link_location
 */
static void start_application(unsigned long app_link_location)
{
	// get the stack pointer value from the program's reset vector
    asm(" ldr r1, [r0,#0]");
    // copy the value to the stack pointer
    asm(" mov sp, r1");
    // get the program counter value from the program's reset vector
    asm(" ldr r0, [r0,#4]");
    // jump to the start address
    asm(" blx r0");
}

void BootApp()
{
	// Deinitialise the TPM registers
	Tpm_Deinit();
	// Deinitialise the UART register
	UART_Deinit();
	// Redirect the vector table register
	SCB->VTOR = BOOTLOADER_BOUNDARY & SCB_VTOR_TBLOFF_Msk;
	// Start the New application from the bootloader boundary
	start_application(BOOTLOADER_BOUNDARY);
}
