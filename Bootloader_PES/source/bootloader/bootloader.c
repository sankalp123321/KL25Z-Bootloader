#include <stdio.h>
#include "bootloader.h"
#include "../flash/flash.h"

#include "loadApp.h"
#include "bootApp.h"

#define BOOT_TIMEOUT 20

static bootstates_t g_state = eCHECKFORINPUT;
static int g_bootApplTimer = BOOT_TIMEOUT;
static int g_enableTimer = 0;

void Bootloader_OneSecondCounter()
{
	if(!g_enableTimer) return;
    g_bootApplTimer--;
    if(!g_bootApplTimer)
    {
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
    	FlashErase();
    	g_state = eUnknown;
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
        g_state = eUnknown;
        break;
    case eCHECKFORINPUT:
        printf("Entering check for input mode\r\n");
        g_enableTimer = 1;
        // check for 20 seconds for UART command, if the user has not pressed anything
        // then boot the application
        g_bootApplTimer = BOOT_TIMEOUT;
        g_state = eUnknown;
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
