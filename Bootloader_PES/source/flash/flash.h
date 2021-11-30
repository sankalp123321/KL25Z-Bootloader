/*
 * flash.h
 *
 *  Created on: 16-Nov-2021
 *      Author: Sankalp
 */

#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_

int Flash_erase(int sector);
int Flash_read(int sector, int* value);
int Flash_write(int sector, int value);

#endif /* FLASH_FLASH_H_ */
