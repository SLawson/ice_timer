/*******************************************************************************
* File Name: readyTimer_ISR.h
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

#if !defined(CY_ISR_readyTimer_ISR_H)
#define CY_ISR_readyTimer_ISR_H

#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void readyTimer_ISR_Start(void) ;
void readyTimer_ISR_StartEx(cyisraddress address) ;
void readyTimer_ISR_Stop(void) ;

CY_ISR_PROTO(readyTimer_ISR_Interrupt);

void readyTimer_ISR_SetVector(cyisraddress address) ;
cyisraddress readyTimer_ISR_GetVector(void) ;

void readyTimer_ISR_SetPriority(uint8 priority) ;
uint8 readyTimer_ISR_GetPriority(void) ;

void readyTimer_ISR_Enable(void) ;
uint8 readyTimer_ISR_GetState(void) ;
void readyTimer_ISR_Disable(void) ;

void readyTimer_ISR_SetPending(void) ;
void readyTimer_ISR_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the readyTimer_ISR ISR. */
#define readyTimer_ISR_INTC_VECTOR            ((reg16 *) readyTimer_ISR__INTC_VECT)

/* Address of the readyTimer_ISR ISR priority. */
#define readyTimer_ISR_INTC_PRIOR             ((reg8 *) readyTimer_ISR__INTC_PRIOR_REG)

/* Priority of the readyTimer_ISR interrupt. */
#define readyTimer_ISR_INTC_PRIOR_NUMBER      readyTimer_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable readyTimer_ISR interrupt. */
#define readyTimer_ISR_INTC_SET_EN            ((reg8 *) readyTimer_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the readyTimer_ISR interrupt. */
#define readyTimer_ISR_INTC_CLR_EN            ((reg8 *) readyTimer_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the readyTimer_ISR interrupt state to pending. */
#define readyTimer_ISR_INTC_SET_PD            ((reg8 *) readyTimer_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the readyTimer_ISR interrupt. */
#define readyTimer_ISR_INTC_CLR_PD            ((reg8 *) readyTimer_ISR__INTC_CLR_PD_REG)



#endif /* CY_ISR_readyTimer_ISR_H */


/* [] END OF FILE */
