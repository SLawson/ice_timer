/*******************************************************************************
* File Name: hr_CNTR_PM.c  
* Version 2.40
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "hr_CNTR.h"

static hr_CNTR_backupStruct hr_CNTR_backup;


/*******************************************************************************
* Function Name: hr_CNTR_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  hr_CNTR_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void hr_CNTR_SaveConfig(void) 
{
    #if (!hr_CNTR_UsingFixedFunction)

        hr_CNTR_backup.CounterUdb = hr_CNTR_ReadCounter();

        #if (CY_UDB_V0)
            hr_CNTR_backup.CounterPeriod = hr_CNTR_ReadPeriod();
            hr_CNTR_backup.CompareValue = hr_CNTR_ReadCompare();
            hr_CNTR_backup.InterruptMaskValue = hr_CNTR_STATUS_MASK;
        #endif /* CY_UDB_V0 */

        #if(!hr_CNTR_ControlRegRemoved)
            hr_CNTR_backup.CounterControlRegister = hr_CNTR_ReadControlRegister();
        #endif /* (!hr_CNTR_ControlRegRemoved) */

    #endif /* (!hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  hr_CNTR_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void hr_CNTR_RestoreConfig(void) 
{      
    #if (!hr_CNTR_UsingFixedFunction)

        #if (CY_UDB_V0)
            uint8 hr_CNTR_interruptState;
        #endif  /* (CY_UDB_V0) */

       hr_CNTR_WriteCounter(hr_CNTR_backup.CounterUdb);

        #if (CY_UDB_V0)
            hr_CNTR_WritePeriod(hr_CNTR_backup.CounterPeriod);
            hr_CNTR_WriteCompare(hr_CNTR_backup.CompareValue);

            hr_CNTR_interruptState = CyEnterCriticalSection();
            hr_CNTR_STATUS_AUX_CTRL |= hr_CNTR_STATUS_ACTL_INT_EN_MASK;
            CyExitCriticalSection(hr_CNTR_interruptState);

            hr_CNTR_STATUS_MASK = hr_CNTR_backup.InterruptMaskValue;
        #endif  /* (CY_UDB_V0) */

        #if(!hr_CNTR_ControlRegRemoved)
            hr_CNTR_WriteControlRegister(hr_CNTR_backup.CounterControlRegister);
        #endif /* (!hr_CNTR_ControlRegRemoved) */

    #endif /* (!hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  hr_CNTR_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void hr_CNTR_Sleep(void) 
{
    #if(!hr_CNTR_ControlRegRemoved)
        /* Save Counter's enable state */
        if(hr_CNTR_CTRL_ENABLE == (hr_CNTR_CONTROL & hr_CNTR_CTRL_ENABLE))
        {
            /* Counter is enabled */
            hr_CNTR_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            hr_CNTR_backup.CounterEnableState = 0u;
        }
    #else
        hr_CNTR_backup.CounterEnableState = 1u;
        if(hr_CNTR_backup.CounterEnableState != 0u)
        {
            hr_CNTR_backup.CounterEnableState = 0u;
        }
    #endif /* (!hr_CNTR_ControlRegRemoved) */
    
    hr_CNTR_Stop();
    hr_CNTR_SaveConfig();
}


/*******************************************************************************
* Function Name: hr_CNTR_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  hr_CNTR_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void hr_CNTR_Wakeup(void) 
{
    hr_CNTR_RestoreConfig();
    #if(!hr_CNTR_ControlRegRemoved)
        if(hr_CNTR_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            hr_CNTR_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!hr_CNTR_ControlRegRemoved) */
    
}


/* [] END OF FILE */
