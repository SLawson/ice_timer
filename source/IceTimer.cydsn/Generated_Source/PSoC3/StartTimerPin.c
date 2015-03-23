/*******************************************************************************
* File Name: StartTimerPin.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "StartTimerPin.h"


/*******************************************************************************
* Function Name: StartTimerPin_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void StartTimerPin_Write(uint8 value) 
{
    uint8 staticBits = (StartTimerPin_DR & (uint8)(~StartTimerPin_MASK));
    StartTimerPin_DR = staticBits | ((uint8)(value << StartTimerPin_SHIFT) & StartTimerPin_MASK);
}


/*******************************************************************************
* Function Name: StartTimerPin_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  StartTimerPin_DM_STRONG     Strong Drive 
*  StartTimerPin_DM_OD_HI      Open Drain, Drives High 
*  StartTimerPin_DM_OD_LO      Open Drain, Drives Low 
*  StartTimerPin_DM_RES_UP     Resistive Pull Up 
*  StartTimerPin_DM_RES_DWN    Resistive Pull Down 
*  StartTimerPin_DM_RES_UPDWN  Resistive Pull Up/Down 
*  StartTimerPin_DM_DIG_HIZ    High Impedance Digital 
*  StartTimerPin_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void StartTimerPin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(StartTimerPin_0, mode);
}


/*******************************************************************************
* Function Name: StartTimerPin_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro StartTimerPin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 StartTimerPin_Read(void) 
{
    return (StartTimerPin_PS & StartTimerPin_MASK) >> StartTimerPin_SHIFT;
}


/*******************************************************************************
* Function Name: StartTimerPin_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 StartTimerPin_ReadDataReg(void) 
{
    return (StartTimerPin_DR & StartTimerPin_MASK) >> StartTimerPin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(StartTimerPin_INTSTAT) 

    /*******************************************************************************
    * Function Name: StartTimerPin_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 StartTimerPin_ClearInterrupt(void) 
    {
        return (StartTimerPin_INTSTAT & StartTimerPin_MASK) >> StartTimerPin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
