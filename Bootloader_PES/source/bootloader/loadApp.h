/*
 * loadApp.h
 *
 *  Created on: 29-Nov-2021
 *      Author: Sankalp
 */

#ifndef BOOTLOADER_LOADAPP_H_
#define BOOTLOADER_LOADAPP_H_
#include <stdint.h>
#define BOOTLOADER_BOUNDARY 0x7800
/**
 *
 */
void FlashErase();

/**
 *
 * @param byte
 */
void Load_SRECLine(uint8_t byte);

/**
 *
 * @param app_link_location
 */
void start_application(unsigned long app_link_location);

#endif /* BOOTLOADER_LOADAPP_H_ */
