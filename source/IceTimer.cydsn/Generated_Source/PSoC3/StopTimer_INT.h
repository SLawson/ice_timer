/*******************************************************************************
* File Name: StopTimer_INT.h
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

#if !defined(CY_ISR_StopTimer_INT_H)
#define CY_ISR_StopTimer_INT_H

#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void StopTimer_INT_Start(void) ;
void StopTimer_INT_StartEx(cyisraddress address) ;
void StopTimer_INT_Stop(void) ;

CY_ISR_PROTO(StopTimer_INT_Interrupt);

void StopTimer_INT_SetVector(cyisraddress address) ;
cyisraddress StopTimer_INT_GetVector(void) ;

void StopTimer_INT_SetPriority(uint8 priority) ;
uint8 StopTimer_INT_GetPriority(void) ;

void StopTimer_INT_Enable(void) ;
uint8 StopTimer_INT_GetState(void) ;
void StopTimer_INT_Disable(void) ;

void StopTimer_INT_SetPending(void) ;
void StopTimer_INT_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the StopTimer_INT ISR. */
#define StopTimer_INT_INTC_VECTOR            ((reg16 *) StopTimer_INT__INTC_VECT)

/* Address of the StopTimer_INT ISR priority. */
#define StopTimer_INT_INTC_PRIOR             ((reg8 *) StopTimer_INT__INTC_PRIOR_REG)

/* Priority of the StopTimer_INT interrupt. */
#define StopTimer_INT_INTC_PRIOR_NUMBER      StopTimer_INT__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable StopTimer_INT interrupt. */
#define StopTimer_INT_INTC_SET_EN            ((reg8 *) StopTimer_INT__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the StopTimer_INT interrupt. */
#define StopTimer_INT_INTC_CLR_EN            ((reg8 *) StopTimer_INT__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the StopTimer_INT interrupt state to pending. */
#define StopTimer_INT_INTC_SET_PD            ((reg8 *) StopTimer_INT__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the StopTimer_INT interrupt. */
#define StopTimer_INT_INTC_CLR_PD            ((reg8 *) StopTimer_INT__INTC_CLR_PD_REG)



#endif /* CY_ISR_StopTimer_INT_H */


/* [] END OF FILE */
