/**
 * @file bootloader.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief This file provides interfaces which allow the state machine
 *        to drive the whole bootloader interface.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

typedef enum bootmodes
{
    eCHECKFORINPUT = 0,
    eERASE = 1,
    ePROGRAMFLASH = 2,
    eBOOTAPPL = 3,
	eWAITFORS19 = 4,
	eUnknown = 0xFF,
}bootstates_t;

/**
 * @brief This function is where the statmachine resides.
 *        Should be called as often as possible in the super loop.
 */
void Bootloader_StateMachine(void);

/**
 * @brief This method sets the state of the machine
 * 
 * @param newstate The new state of the state machine
 */
void Bootloader_SetState(bootstates_t newstate);

/**
 * @brief One second counter for the timeout mechanism.
 * 
 */
void Bootloader_OneSecondCounter();

/**
 * @brief Function returns the current state of the statemachine.
 * 
 * @return bootstates_t statemachine state.
 */
bootstates_t Bootloader_GetState();

#endif
