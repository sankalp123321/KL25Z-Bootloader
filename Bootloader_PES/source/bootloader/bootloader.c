#include "bootloader.h"
#include "../flash/flash.h"
#include <stdio.h>
#include "loadApp.h"
#include "bootApp.h"

static bootstates_t g_state = eCHECKFORINPUT;
static int g_bootApplTimer = 20;
static int g_enableTimer = 0;

#define MEM_LOC 0xF000

void Bootloader_OneSecondCounter()
{
	if(!g_enableTimer) return;
    g_bootApplTimer--;
    if(!g_bootApplTimer)
    {
        printf("Boot application\r\n");
        g_state = eBOOTAPPL;
    }
    else
    {
    	printf("Timing out %d\r\n", g_bootApplTimer);
    }
}

void Bootloader_StateMachine()
{
    switch (g_state)
    {
    case eERASE:
    	g_enableTimer = 0;
//        // erase the flash
//    	Flash_write(MEM_LOC, 30);
//    	int *a = (int*)(MEM_LOC);
//		printf("%08X\r\n", *a);
////    	for(int i = 0; i < 10000000; i++){for(int j = 0; j < 100; j++){}}
//    	Flash_erase(MEM_LOC);
//    	printf("%08X\r\n", *a);
////    	for(int i = 0; i < 10000000; i++){for(int j = 0; j < 100; j++){}}
    	FlashErase();
    	g_state = 0xFF;
        break;
    case ePROGRAMFLASH:
    	g_enableTimer = 0;
    	printf("Data coming in...\r\n");
        // wait for the .s19
    	g_state = eWAITFORS19;
        break;
    case eBOOTAPPL:
        printf("Entering boot application mode\r\n");
        BootApp();
        g_enableTimer = 0;
        g_state = 0xFF;
        break;
    case eCHECKFORINPUT:
        printf("Entering check for input mode\r\n");
        g_enableTimer = 1;
        // check for 20 seconds for UART command, if the user has not pressed anything
        // then boot the application
        g_bootApplTimer = 20;
        g_state = 0xFF;
        // Enable the 20 second time
        break;
    default:
        break;
    }
}

void Bootloader_SetState(bootstates_t newstate)
{
    g_state = newstate;
}

bootstates_t Bootloader_GetState()
{
    return g_state;
}
