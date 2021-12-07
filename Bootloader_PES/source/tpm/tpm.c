/*
 * tpm.c
 *
 *  Created on: 12-Nov-2021
 *      Author: Sankalp
 */
#include <stdio.h>
#include "tpm.h"
#include "MKL25Z4.h"
#include "../commons.h"

#define INTERRUPT_DELAY 48000
#define PRESCALAR_DIV_FACTOR 4

#define GREEN_HEARTBEAT_LED 19

volatile static int tpm0_1_msec_cntr = 0;

volatile uint8_t one_second_occoured = 0;

void TPM0_IRQHandler()
{
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	if(TPM0->STATUS & TPM_STATUS_CH0F_MASK)
	{
		TPM0->STATUS &= ~TPM_STATUS_CH0F_MASK;
	}
	TPM0->STATUS |= TPM_STATUS_TOF_MASK;

	tpm0_1_msec_cntr++;

	if(tpm0_1_msec_cntr >= INTERRUPT_DELAY)
	{
		one_second_occoured = 1;
		PTB->PTOR |= 1 << GREEN_HEARTBEAT_LED;
		tpm0_1_msec_cntr = 0;
	}
}

void Tpm_Deinit()
{
	SIM->SCGC5 &= ~SIM_SCGC5_PORTB_MASK;
	SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK | ~SIM_SCGC6_TPM1_MASK;
	//set clock source for tpm
	SIM->SOPT2 &= ~(SIM_SOPT2_TPMSRC(1) | ~SIM_SOPT2_PLLFLLSEL_MASK);
	NVIC_DisableIRQ(TPM0_IRQn);
	TPM0->SC = 0;
}

void Tpm_Init()
{

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[GREEN_HEARTBEAT_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_HEARTBEAT_LED] |= PORT_PCR_MUX(1);
	FGPIOB->PDDR |= 1 << (GREEN_HEARTBEAT_LED);

	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	// Set TPM0 for DAC at 48Khz
	TPM0->SC = 0;
	TPM0->SC |= TPM_SC_PS(2) | TPM_SC_TOIE_MASK;
	NVIC_EnableIRQ(TPM0_IRQn);
	TPM0->CNT = 0;
	TPM0->MOD = TPM_MOD_MOD((SystemCoreClock/PRESCALAR_DIV_FACTOR)/INTERRUPT_DELAY);
	TPM0->SC |= TPM_SC_CMOD(1);
}
