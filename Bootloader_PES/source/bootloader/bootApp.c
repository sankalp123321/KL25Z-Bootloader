/*
 * bootApp.c
 *
 *  Created on: 29-Nov-2021
 *      Author: Sankalp
 */

#include "bootApp.h"
#include "MKL25Z4.h"
#include "loadApp.h"
#include "../tpm/tpm.h"
#include "../uart/uart.h"

void BootApp()
{
	Tpm_Deinit();
	UART_Deinit();

	SCB->VTOR = BOOTLOADER_BOUNDARY & SCB_VTOR_TBLOFF_Msk;
	start_application(BOOTLOADER_BOUNDARY);
}
