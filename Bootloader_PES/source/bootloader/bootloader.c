/**
 * @file bootloader.c
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief This file describes the state machine for driving the
 *        bootloader interface.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// C standard lib
#include <stdio.h>
// Application specific lib
#include "bootloader.h"
#include "../flash/flash.h"
#include "tpm/tpm.h"
#include "loadApp.h"
#include "bootApp.h"

#define BOOT_TIMEOUT 20
// Init the satte machine state varibale
static bootstates_t g_state = eCHECKFORINPUT;
// Set the boot timeout delay to the macro value
static int g_bootApplTimer = BOOT_TIMEOUT;
// Flag for enabling and disabling the timer
static int g_enableTimer = 0;

// Called every 1 second
void Bootloader_OneSecondCounter()
{
    // If the timer flag is disabled, return.
	if(!g_enableTimer) return;
    // Count down timer
    g_bootApplTimer--;
    // If the timer reaches 0, set the state of the statemachine to boot the application
    if(!g_bootApplTimer)
    {
        // Boot state
        g_state = eBOOTAPPL;
    }
    else
    {
        // Else print the timer values
    	printf("Timing out %d\r\n", g_bootApplTimer);
    }
}

void Bootloader_StateMachine()
{
    switch (g_state)
    {
    case eERASE:
        /**
         * This state is set when the user enters the "erase" commans in the terminal.
         * The objective of this state is to erase the image location.
         */
    	g_enableTimer = 0;
    	FlashErase();
    	g_state = eUnknown;
        break;
    case ePROGRAMFLASH:
        /**
         * This mode is set when the user enters the "prog" command in the terminal.
         * In this state, the program is continuously waiting for the new s19 files
         * from the UART.
         */
    	g_enableTimer = 0;
    	printf("Data coming in...\r\n");
        // wait for the .s19
    	g_state = eWAITFORS19;
        break;
    case eBOOTAPPL:
        /**
         * This mode is set when the:
         * 1. The user activity timer expires and then the bootloader has to boot the application.
         * 2. The user explicity enters the "boot" command in the terminal in the bootloader mode.
         * 3. The s19 file tranfer is complete and the s9 command is recvd in the end denoting
         *    the end of record.
         * 
         * The state calls the bootApp function to boot the image from the bootlaoder boundary.
         */
        if(BootApp_IsFirmwareVerificationPresent())
        {
        	BootApp();
        }
        else
        {
        	printf("No Firmware present.\r\n");
        	g_state = eCHECKFORINPUT;
        }
        g_enableTimer = 0;
        break;
    case eCHECKFORINPUT:
        /**
         * This is the default state of the statemachine and the very first state
         * the state machine starts from.
         * 
         */
        printf("Entering check for input mode\r\n");
        g_enableTimer = 1;
        // check for 20 seconds for UART command, if the user has not pressed anything
        // then boot the application
        g_bootApplTimer = BOOT_TIMEOUT;
        g_state = eUnknown;
        // Enable the 20 second time
        break;
    case eADDRESSCORRUPTERROR:
    	printf("Error: ATTEMPTED TO WRITE TO PROTECTED REGION.\r\n");
    	printf("Reboot the system.\r\n");
    	Tpm_SetLedColour(0);
    	g_state = eUnknown;
    	break;
    default:
        break;
    }
}

void Bootloader_SetState(bootstates_t newstate)
{
    // Sets the new state for the state machine
    g_state = newstate;
}

bootstates_t Bootloader_GetState()
{
    // Returns the current state of the state machine.
    return g_state;
}
