/*
 * loadApp.h
 *
 *  Created on: 29-Nov-2021
 *      Author: Sankalp
 */

#ifndef BOOTLOADER_LOADAPP_H_
#define BOOTLOADER_LOADAPP_H_
#include <stdint.h>
void FlashErase();
void LoadApp(char *cmd_new);
void Load_SRECLine(uint8_t byte);
void start_application(unsigned long app_link_location);

#endif /* BOOTLOADER_LOADAPP_H_ */
