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

#undef _DSO_DEBUG_

#ifdef _DSO_DEBUG_
	#define GPIO_DEBUG_48_KHZ 0
	#define GPIO_DEBUG_96_KHZ 1
	volatile int pin_status = 0;
	volatile int pin_status_96 = 0;
#endif

volatile static int tpm0_1_msec_cntr = 0;

volatile uint8_t one_second_occoured = 0;

void __attribute__((section(".ramfunc"), long_call)) TPM0_IRQHandler()
{
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	if(TPM0->STATUS & TPM_STATUS_CH0F_MASK)
	{
		TPM0->STATUS &= ~TPM_STATUS_CH0F_MASK;
	}
	TPM0->STATUS |= TPM_STATUS_TOF_MASK;

	tpm0_1_msec_cntr++;

	if(tpm0_1_msec_cntr >= 48000)
	{
		one_second_occoured = 1;
		tpm0_1_msec_cntr = 0;
	}

#ifdef _DSO_DEBUG_
	if(pin_status)
	{
		PTB->PCOR |= 1 << GPIO_DEBUG_48_KHZ;//GPIO_PCOR_PTCO(GPIO_DEBUG_48_KHZ);
	}
	else
	{
		PTB->PSOR |= 1 << GPIO_DEBUG_48_KHZ;//GPIO_PSOR_PTSO(GPIO_DEBUG_48_KHZ);
	}
	pin_status = !pin_status;
#endif
}

void TPM1_IRQHandler()
{
	//NVIC_ClearPendingIRQ(TPM1_IRQn);
	if(TPM1->STATUS & TPM_STATUS_CH0F_MASK)
	{
		TPM1->STATUS &= ~TPM_STATUS_CH0F_MASK;
	}
	TPM1->STATUS |= TPM_STATUS_TOF_MASK;

#ifdef _DSO_DEBUG_
	if(pin_status_96)
	{
		PTB->PCOR |= 1 << GPIO_DEBUG_96_KHZ;
	}
	else
	{
		PTB->PSOR |= 1 << GPIO_DEBUG_96_KHZ;
	}
	pin_status_96 = !pin_status_96;
#endif
}

void Tpm_Deinit()
{
	SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK | ~SIM_SCGC6_TPM1_MASK;
	//set clock source for tpm
	SIM->SOPT2 &= ~(SIM_SOPT2_TPMSRC(1) | ~SIM_SOPT2_PLLFLLSEL_MASK);
	NVIC_DisableIRQ(TPM0_IRQn);
	NVIC_DisableIRQ(TPM1_IRQn);
	TPM0->SC = 0;
	TPM1->SC = 0;
}

void Tpm_Init()
{
#ifdef _DSO_DEBUG_
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[GPIO_DEBUG_48_KHZ] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GPIO_DEBUG_48_KHZ] |= PORT_PCR_MUX(1);
	FGPIOB->PDDR |= 1 << (GPIO_DEBUG_48_KHZ);

	PORTB->PCR[GPIO_DEBUG_96_KHZ] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GPIO_DEBUG_96_KHZ] |= PORT_PCR_MUX(1);
	FGPIOB->PDDR |= 1 << (GPIO_DEBUG_96_KHZ);
#endif
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK;
	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	// Set TPM0 for DAC at 48Khz
	TPM0->SC = 0;
	TPM0->SC |= TPM_SC_PS(2) | TPM_SC_TOIE_MASK;
	NVIC_EnableIRQ(TPM0_IRQn);
	TPM0->CNT = 0;
	TPM0->MOD = TPM_MOD_MOD(248);
	TPM0->SC |= TPM_SC_CMOD(1);

	// Set TPM1 for DAC at 48Khz
	TPM1->SC = 0;
	TPM1->SC |= TPM_SC_DMA_MASK | TPM_SC_PS(2);
	TPM1->CNT = 0;
	TPM1->MOD = TPM_MOD_MOD(124);
	TPM1->SC |= TPM_SC_CMOD(1);
}
