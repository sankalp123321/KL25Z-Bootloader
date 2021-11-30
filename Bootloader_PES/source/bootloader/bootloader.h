#ifndef BOOTLOADER_H
#define BOOTLOADER_H

typedef enum bootmodes
{
    eCHECKFORINPUT = 0,
    eERASE = 1,
    ePROGRAMFLASH = 2,
    eBOOTAPPL = 3,
}bootstates_t;


void Bootloader_StateMachine(void);
void Bootloader_SetState(bootstates_t newstate);
void Bootloader_OneSecondCounter();

#endif
