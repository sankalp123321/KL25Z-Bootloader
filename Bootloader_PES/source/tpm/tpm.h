/**
 * @file tpm.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief Interface for timer and GPIO
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TPM_TPM_H_
#define TPM_TPM_H_

/**
 * @brief This method initialises the TPM to run at 48KHZ.
 * 
 */
void Tpm_Init();

/**
 * @brief This method is used to de initialise the TPM registers.
 * 
 */
void Tpm_Deinit();

#endif /* TPM_TPM_H_ */
