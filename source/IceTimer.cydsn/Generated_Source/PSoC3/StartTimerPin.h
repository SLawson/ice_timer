/*******************************************************************************
* File Name: StartTimerPin.h  
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

#if !defined(CY_PINS_StartTimerPin_H) /* Pins StartTimerPin_H */
#define CY_PINS_StartTimerPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "StartTimerPin_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    StartTimerPin_Write(uint8 value) ;
void    StartTimerPin_SetDriveMode(uint8 mode) ;
uint8   StartTimerPin_ReadDataReg(void) ;
uint8   StartTimerPin_Read(void) ;
uint8   StartTimerPin_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define StartTimerPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define StartTimerPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define StartTimerPin_DM_RES_UP          PIN_DM_RES_UP
#define StartTimerPin_DM_RES_DWN         PIN_DM_RES_DWN
#define StartTimerPin_DM_OD_LO           PIN_DM_OD_LO
#define StartTimerPin_DM_OD_HI           PIN_DM_OD_HI
#define StartTimerPin_DM_STRONG          PIN_DM_STRONG
#define StartTimerPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define StartTimerPin_MASK               StartTimerPin__MASK
#define StartTimerPin_SHIFT              StartTimerPin__SHIFT
#define StartTimerPin_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define StartTimerPin_PS                     (* (reg8 *) StartTimerPin__PS)
/* Data Register */
#define StartTimerPin_DR                     (* (reg8 *) StartTimerPin__DR)
/* Port Number */
#define StartTimerPin_PRT_NUM                (* (reg8 *) StartTimerPin__PRT) 
/* Connect to Analog Globals */                                                  
#define StartTimerPin_AG                     (* (reg8 *) StartTimerPin__AG)                       
/* Analog MUX bux enable */
#define StartTimerPin_AMUX                   (* (reg8 *) StartTimerPin__AMUX) 
/* Bidirectional Enable */                                                        
#define StartTimerPin_BIE                    (* (reg8 *) StartTimerPin__BIE)
/* Bit-mask for Aliased Register Access */
#define StartTimerPin_BIT_MASK               (* (reg8 *) StartTimerPin__BIT_MASK)
/* Bypass Enable */
#define StartTimerPin_BYP                    (* (reg8 *) StartTimerPin__BYP)
/* Port wide control signals */                                                   
#define StartTimerPin_CTL                    (* (reg8 *) StartTimerPin__CTL)
/* Drive Modes */
#define StartTimerPin_DM0                    (* (reg8 *) StartTimerPin__DM0) 
#define StartTimerPin_DM1                    (* (reg8 *) StartTimerPin__DM1)
#define StartTimerPin_DM2                    (* (reg8 *) StartTimerPin__DM2) 
/* Input Buffer Disable Override */
#define StartTimerPin_INP_DIS                (* (reg8 *) StartTimerPin__INP_DIS)
/* LCD Common or Segment Drive */
#define StartTimerPin_LCD_COM_SEG            (* (reg8 *) StartTimerPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define StartTimerPin_LCD_EN                 (* (reg8 *) StartTimerPin__LCD_EN)
/* Slew Rate Control */
#define StartTimerPin_SLW                    (* (reg8 *) StartTimerPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define StartTimerPin_PRTDSI__CAPS_SEL       (* (reg8 *) StartTimerPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define StartTimerPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) StartTimerPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define StartTimerPin_PRTDSI__OE_SEL0        (* (reg8 *) StartTimerPin__PRTDSI__OE_SEL0) 
#define StartTimerPin_PRTDSI__OE_SEL1        (* (reg8 *) StartTimerPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define StartTimerPin_PRTDSI__OUT_SEL0       (* (reg8 *) StartTimerPin__PRTDSI__OUT_SEL0) 
#define StartTimerPin_PRTDSI__OUT_SEL1       (* (reg8 *) StartTimerPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define StartTimerPin_PRTDSI__SYNC_OUT       (* (reg8 *) StartTimerPin__PRTDSI__SYNC_OUT) 


#if defined(StartTimerPin__INTSTAT)  /* Interrupt Registers */

    #define StartTimerPin_INTSTAT                (* (reg8 *) StartTimerPin__INTSTAT)
    #define StartTimerPin_SNAP                   (* (reg8 *) StartTimerPin__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins StartTimerPin_H */


/* [] END OF FILE */
