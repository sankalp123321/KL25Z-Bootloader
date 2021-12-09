/**
 * @file flash.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief This file provides function that allow for flash operation
 *        to be possible.
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_

/**
 * @brief This fucntion erases a sector worth of memory
 * 
 * @param sector Sector Address
 * @return int Success or fail.
 */
int Flash_erase(int sector);

/**
 * @brief This method writes the data on the address specified.
 * 
 * @param address Address on which the data is to be written
 * @param value Data
 * @return int Success or Failure
 */
int Flash_write(int address, int value);

#endif /* FLASH_FLASH_H_ */
