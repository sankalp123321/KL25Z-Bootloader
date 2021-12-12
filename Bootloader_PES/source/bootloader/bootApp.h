/**
 * @file bootApp.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief This file provides functions to boot the firmware image
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef BOOTLOADER_BOOTAPP_H_
#define BOOTLOADER_BOOTAPP_H_

#define BOOTLOADER_BOUNDARY 0x7800
#define FIRMWARE_VERIFICATION 0x00007400
/**
 *  @brief This function is used to boot the application
 *         from the 0x7800 location.
 */
void BootApp();

/**
 * @brief This function writes the verification signature
 * 	      onto the fash memory location.
 */
void BootApp_WriteFirmwareVerificationArea();

/**
 * @brief This function checks for the presence
 * 		  of the verification signature
 * @return 1, if present
 *         0, if not present
 */
unsigned char BootApp_IsFirmwareVerificationPresent();

#endif /* BOOTLOADER_BOOTAPP_H_ */
