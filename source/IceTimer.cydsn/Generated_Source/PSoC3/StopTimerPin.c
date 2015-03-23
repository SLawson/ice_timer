/*******************************************************************************
* File Name: StopTimerPin.c  
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
#include "StopTimerPin.h"


/*******************************************************************************
* Function Name: StopTimerPin_Write
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
void StopTimerPin_Write(uint8 value) 
{
    uint8 staticBits = (StopTimerPin_DR & (uint8)(~StopTimerPin_MASK));
    StopTimerPin_DR = staticBits | ((uint8)(value << StopTimerPin_SHIFT) & StopTimerPin_MASK);
}


/*******************************************************************************
* Function Name: StopTimerPin_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  StopTimerPin_DM_STRONG     Strong Drive 
*  StopTimerPin_DM_OD_HI      Open Drain, Drives High 
*  StopTimerPin_DM_OD_LO      Open Drain, Drives Low 
*  StopTimerPin_DM_RES_UP     Resistive Pull Up 
*  StopTimerPin_DM_RES_DWN    Resistive Pull Down 
*  StopTimerPin_DM_RES_UPDWN  Resistive Pull Up/Down 
*  StopTimerPin_DM_DIG_HIZ    High Impedance Digital 
*  StopTimerPin_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void StopTimerPin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(StopTimerPin_0, mode);
}


/*******************************************************************************
* Function Name: StopTimerPin_Read
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
*  Macro StopTimerPin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 StopTimerPin_Read(void) 
{
    return (StopTimerPin_PS & StopTimerPin_MASK) >> StopTimerPin_SHIFT;
}


/*******************************************************************************
* Function Name: StopTimerPin_ReadDataReg
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
uint8 StopTimerPin_ReadDataReg(void) 
{
    return (StopTimerPin_DR & StopTimerPin_MASK) >> StopTimerPin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(StopTimerPin_INTSTAT) 

    /*******************************************************************************
    * Function Name: StopTimerPin_ClearInterrupt
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
    uint8 StopTimerPin_ClearInterrupt(void) 
    {
        return (StopTimerPin_INTSTAT & StopTimerPin_MASK) >> StopTimerPin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
