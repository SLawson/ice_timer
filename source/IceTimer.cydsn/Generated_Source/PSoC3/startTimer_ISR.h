/*******************************************************************************
* File Name: startTimer_ISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ISR_startTimer_ISR_H)
#define CY_ISR_startTimer_ISR_H

#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void startTimer_ISR_Start(void) ;
void startTimer_ISR_StartEx(cyisraddress address) ;
void startTimer_ISR_Stop(void) ;

CY_ISR_PROTO(startTimer_ISR_Interrupt);

void startTimer_ISR_SetVector(cyisraddress address) ;
cyisraddress startTimer_ISR_GetVector(void) ;

void startTimer_ISR_SetPriority(uint8 priority) ;
uint8 startTimer_ISR_GetPriority(void) ;

void startTimer_ISR_Enable(void) ;
uint8 startTimer_ISR_GetState(void) ;
void startTimer_ISR_Disable(void) ;

void startTimer_ISR_SetPending(void) ;
void startTimer_ISR_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the startTimer_ISR ISR. */
#define startTimer_ISR_INTC_VECTOR            ((reg16 *) startTimer_ISR__INTC_VECT)

/* Address of the startTimer_ISR ISR priority. */
#define startTimer_ISR_INTC_PRIOR             ((reg8 *) startTimer_ISR__INTC_PRIOR_REG)

/* Priority of the startTimer_ISR interrupt. */
#define startTimer_ISR_INTC_PRIOR_NUMBER      startTimer_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable startTimer_ISR interrupt. */
#define startTimer_ISR_INTC_SET_EN            ((reg8 *) startTimer_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the startTimer_ISR interrupt. */
#define startTimer_ISR_INTC_CLR_EN            ((reg8 *) startTimer_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the startTimer_ISR interrupt state to pending. */
#define startTimer_ISR_INTC_SET_PD            ((reg8 *) startTimer_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the startTimer_ISR interrupt. */
#define startTimer_ISR_INTC_CLR_PD            ((reg8 *) startTimer_ISR__INTC_CLR_PD_REG)



#endif /* CY_ISR_startTimer_ISR_H */


/* [] END OF FILE */
