#include "bootloader.h"
#include <stdio.h>

static bootstates_t g_state = eCHECKFORINPUT;
static int g_bootApplTimer = 20;
static int g_enableTimer = 0;

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
        // erase the flash
        break;
    case ePROGRAMFLASH:
        // wait for the .s19
        break;
    case eBOOTAPPL:
        printf("Entering boot applicaton mode\r\n");
        g_enableTimer = 0;
        g_state = 0xFF;
        break;
    case eCHECKFORINPUT:
        printf("Entering check for input mode\r\n");
        g_enableTimer = 1;
        // check for 20 seconds for uart command, if the user has not pressed anything
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
