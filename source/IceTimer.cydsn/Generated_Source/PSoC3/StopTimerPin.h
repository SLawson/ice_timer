/*******************************************************************************
* File Name: StopTimerPin.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_StopTimerPin_H) /* Pins StopTimerPin_H */
#define CY_PINS_StopTimerPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "StopTimerPin_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    StopTimerPin_Write(uint8 value) ;
void    StopTimerPin_SetDriveMode(uint8 mode) ;
uint8   StopTimerPin_ReadDataReg(void) ;
uint8   StopTimerPin_Read(void) ;
uint8   StopTimerPin_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define StopTimerPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define StopTimerPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define StopTimerPin_DM_RES_UP          PIN_DM_RES_UP
#define StopTimerPin_DM_RES_DWN         PIN_DM_RES_DWN
#define StopTimerPin_DM_OD_LO           PIN_DM_OD_LO
#define StopTimerPin_DM_OD_HI           PIN_DM_OD_HI
#define StopTimerPin_DM_STRONG          PIN_DM_STRONG
#define StopTimerPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define StopTimerPin_MASK               StopTimerPin__MASK
#define StopTimerPin_SHIFT              StopTimerPin__SHIFT
#define StopTimerPin_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define StopTimerPin_PS                     (* (reg8 *) StopTimerPin__PS)
/* Data Register */
#define StopTimerPin_DR                     (* (reg8 *) StopTimerPin__DR)
/* Port Number */
#define StopTimerPin_PRT_NUM                (* (reg8 *) StopTimerPin__PRT) 
/* Connect to Analog Globals */                                                  
#define StopTimerPin_AG                     (* (reg8 *) StopTimerPin__AG)                       
/* Analog MUX bux enable */
#define StopTimerPin_AMUX                   (* (reg8 *) StopTimerPin__AMUX) 
/* Bidirectional Enable */                                                        
#define StopTimerPin_BIE                    (* (reg8 *) StopTimerPin__BIE)
/* Bit-mask for Aliased Register Access */
#define StopTimerPin_BIT_MASK               (* (reg8 *) StopTimerPin__BIT_MASK)
/* Bypass Enable */
#define StopTimerPin_BYP                    (* (reg8 *) StopTimerPin__BYP)
/* Port wide control signals */                                                   
#define StopTimerPin_CTL                    (* (reg8 *) StopTimerPin__CTL)
/* Drive Modes */
#define StopTimerPin_DM0                    (* (reg8 *) StopTimerPin__DM0) 
#define StopTimerPin_DM1                    (* (reg8 *) StopTimerPin__DM1)
#define StopTimerPin_DM2                    (* (reg8 *) StopTimerPin__DM2) 
/* Input Buffer Disable Override */
#define StopTimerPin_INP_DIS                (* (reg8 *) StopTimerPin__INP_DIS)
/* LCD Common or Segment Drive */
#define StopTimerPin_LCD_COM_SEG            (* (reg8 *) StopTimerPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define StopTimerPin_LCD_EN                 (* (reg8 *) StopTimerPin__LCD_EN)
/* Slew Rate Control */
#define StopTimerPin_SLW                    (* (reg8 *) StopTimerPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define StopTimerPin_PRTDSI__CAPS_SEL       (* (reg8 *) StopTimerPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define StopTimerPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) StopTimerPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define StopTimerPin_PRTDSI__OE_SEL0        (* (reg8 *) StopTimerPin__PRTDSI__OE_SEL0) 
#define StopTimerPin_PRTDSI__OE_SEL1        (* (reg8 *) StopTimerPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define StopTimerPin_PRTDSI__OUT_SEL0       (* (reg8 *) StopTimerPin__PRTDSI__OUT_SEL0) 
#define StopTimerPin_PRTDSI__OUT_SEL1       (* (reg8 *) StopTimerPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define StopTimerPin_PRTDSI__SYNC_OUT       (* (reg8 *) StopTimerPin__PRTDSI__SYNC_OUT) 


#if defined(StopTimerPin__INTSTAT)  /* Interrupt Registers */

    #define StopTimerPin_INTSTAT                (* (reg8 *) StopTimerPin__INTSTAT)
    #define StopTimerPin_SNAP                   (* (reg8 *) StopTimerPin__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins StopTimerPin_H */


/* [] END OF FILE */
