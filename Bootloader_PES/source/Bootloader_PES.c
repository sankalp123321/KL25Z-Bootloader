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
    UART_Init(115200);
    Tpm_Init();
    printf("Hello World\r\n");
    CmdCentre_CommandEngine("Help");
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        Bootloader_StateMachine();
        if(one_second_occoured)
        {
            Bootloader_OneSecondCounter();
            one_second_occoured = false;
        }
        char cmd[256];
		if(CmdCentre_WordEngine(cmd))
		{
		  CmdCentre_CommandEngine(cmd);
		  memset(cmd, 0, sizeof(cmd));
		}
    }
    return 0 ;
}
