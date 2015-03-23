/*******************************************************************************
* File Name: StartTimer_INT.h
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

#if !defined(CY_ISR_StartTimer_INT_H)
#define CY_ISR_StartTimer_INT_H

#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void StartTimer_INT_Start(void) ;
void StartTimer_INT_StartEx(cyisraddress address) ;
void StartTimer_INT_Stop(void) ;

CY_ISR_PROTO(StartTimer_INT_Interrupt);

void StartTimer_INT_SetVector(cyisraddress address) ;
cyisraddress StartTimer_INT_GetVector(void) ;

void StartTimer_INT_SetPriority(uint8 priority) ;
uint8 StartTimer_INT_GetPriority(void) ;

void StartTimer_INT_Enable(void) ;
uint8 StartTimer_INT_GetState(void) ;
void StartTimer_INT_Disable(void) ;

void StartTimer_INT_SetPending(void) ;
void StartTimer_INT_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the StartTimer_INT ISR. */
#define StartTimer_INT_INTC_VECTOR            ((reg16 *) StartTimer_INT__INTC_VECT)

/* Address of the StartTimer_INT ISR priority. */
#define StartTimer_INT_INTC_PRIOR             ((reg8 *) StartTimer_INT__INTC_PRIOR_REG)

/* Priority of the StartTimer_INT interrupt. */
#define StartTimer_INT_INTC_PRIOR_NUMBER      StartTimer_INT__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable StartTimer_INT interrupt. */
#define StartTimer_INT_INTC_SET_EN            ((reg8 *) StartTimer_INT__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the StartTimer_INT interrupt. */
#define StartTimer_INT_INTC_CLR_EN            ((reg8 *) StartTimer_INT__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the StartTimer_INT interrupt state to pending. */
#define StartTimer_INT_INTC_SET_PD            ((reg8 *) StartTimer_INT__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the StartTimer_INT interrupt. */
#define StartTimer_INT_INTC_CLR_PD            ((reg8 *) StartTimer_INT__INTC_CLR_PD_REG)



#endif /* CY_ISR_StartTimer_INT_H */


/* [] END OF FILE */
