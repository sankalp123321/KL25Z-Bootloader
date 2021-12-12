/**
 * @file loadApp.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief The file provides interfaces to erase and load the new 
 *        firmware image into the flash.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef BOOTLOADER_LOADAPP_H_
#define BOOTLOADER_LOADAPP_H_
#include <stdint.h>

/**
 * @brief THis function recursively erases the flash
 *        sectors starting 0x7800
 */
void FlashErase();

/**
 * @brief This method decodes the recvd s19 record 
 *        and programs the data into the specified address location.
 * 
 * @param byte Data byte
 */
void Load_SRECLine(uint8_t byte);

#endif /* BOOTLOADER_LOADAPP_H_ */
