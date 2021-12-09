/**
 * @file    Bootloader_PES.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
/* TODO: insert other include files here. */
#include "uart/uart.h"
#include "bootloader/bootloader.h"
#include "tpm/tpm.h"
#include "cmdcentre/cmdcentre.h"
/* TODO: insert other definitions and declarations here. */
extern uint8_t one_second_occoured;
/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    // Initialise the UART to 115200
    UART_Init(115200);
    // Intialise the TPM to produce 1 second notifs
    Tpm_Init();
    // Mode print
    printf("Bootloader Mode\r\n");
    // Print the initial set of commands
    CmdCentre_CommandEngine("Help");
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        // Loop through the state machine.
        Bootloader_StateMachine();
        // Toggle 1 second timer
        if(one_second_occoured)
        {
            // Bootloader timeout mechanism
            Bootloader_OneSecondCounter();
            // Reset the one second timer
            one_second_occoured = false;
        }
        // Form a command
        char cmd[256];
		if(CmdCentre_WordEngine(cmd))
		{
            // Process the command
		    CmdCentre_CommandEngine(cmd);
            // Reset
		    memset(cmd, 0, sizeof(cmd));
		}
    }
    return 0 ;
}
