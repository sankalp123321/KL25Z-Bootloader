/*
 * bootApp.c
 *
 *  Created on: 29-Nov-2021
 *      Author: Sankalp
 */

#include "bootApp.h"
#include "fsl_common.h"
#include "MKL25Z4.h"
#include "loadApp.h"
#include "../tpm/tpm.h"
#include "../uart/uart.h"

void BootApp()
{
//	DisableGlobalIRQ();
	Tpm_Deinit();
	UART_Deinit();

	SCB->VTOR = 0x00007800 & SCB_VTOR_TBLOFF_Msk;

	start_application(0x7800);
}
