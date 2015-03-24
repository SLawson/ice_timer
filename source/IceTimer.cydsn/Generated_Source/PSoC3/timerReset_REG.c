/*******************************************************************************
* File Name: timerReset_REG.c  
* Version 1.70
*
* Description:
*  This file contains API to enable firmware control of a Control Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "timerReset_REG.h"

#if !defined(timerReset_REG_Sync_ctrl_reg__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
* Function Name: timerReset_REG_Write
********************************************************************************
*
* Summary:
*  Write a byte to the Control Register.
*
* Parameters:
*  control:  The value to be assigned to the Control Register.
*
* Return:
*  None.
*
*******************************************************************************/
void timerReset_REG_Write(uint8 control) 
{
    timerReset_REG_Control = control;
}


/*******************************************************************************
* Function Name: timerReset_REG_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Control Register.
*
* Parameters:
*  None.
*
* Return:
*  Returns the current value in the Control Register.
*
*******************************************************************************/
uint8 timerReset_REG_Read(void) 
{
    return timerReset_REG_Control;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
