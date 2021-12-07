/*
 * flash.h
 *
 *  Created on: 16-Nov-2021
 *      Author: Sankalp
 */

#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_

/**
 *
 * @param sector
 * @return
 */
int Flash_erase(int sector);

/**
 *
 * @param address
 * @param buffer
 * @param bytes
 * @return
 */
int Flash_read(int address, int* buffer, int bytes);

/**
 *
 * @param sector
 * @param value
 * @return
 */
int Flash_write(int sector, int value);

#endif /* FLASH_FLASH_H_ */
