/*******************************************************************************
* File Name: min_CNTR_PM.c  
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

#include "min_CNTR.h"

static min_CNTR_backupStruct min_CNTR_backup;


/*******************************************************************************
* Function Name: min_CNTR_SaveConfig
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
*  min_CNTR_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void min_CNTR_SaveConfig(void) 
{
    #if (!min_CNTR_UsingFixedFunction)

        min_CNTR_backup.CounterUdb = min_CNTR_ReadCounter();

        #if (CY_UDB_V0)
            min_CNTR_backup.CounterPeriod = min_CNTR_ReadPeriod();
            min_CNTR_backup.CompareValue = min_CNTR_ReadCompare();
            min_CNTR_backup.InterruptMaskValue = min_CNTR_STATUS_MASK;
        #endif /* CY_UDB_V0 */

        #if(!min_CNTR_ControlRegRemoved)
            min_CNTR_backup.CounterControlRegister = min_CNTR_ReadControlRegister();
        #endif /* (!min_CNTR_ControlRegRemoved) */

    #endif /* (!min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_RestoreConfig
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
*  min_CNTR_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void min_CNTR_RestoreConfig(void) 
{      
    #if (!min_CNTR_UsingFixedFunction)

        #if (CY_UDB_V0)
            uint8 min_CNTR_interruptState;
        #endif  /* (CY_UDB_V0) */

       min_CNTR_WriteCounter(min_CNTR_backup.CounterUdb);

        #if (CY_UDB_V0)
            min_CNTR_WritePeriod(min_CNTR_backup.CounterPeriod);
            min_CNTR_WriteCompare(min_CNTR_backup.CompareValue);

            min_CNTR_interruptState = CyEnterCriticalSection();
            min_CNTR_STATUS_AUX_CTRL |= min_CNTR_STATUS_ACTL_INT_EN_MASK;
            CyExitCriticalSection(min_CNTR_interruptState);

            min_CNTR_STATUS_MASK = min_CNTR_backup.InterruptMaskValue;
        #endif  /* (CY_UDB_V0) */

        #if(!min_CNTR_ControlRegRemoved)
            min_CNTR_WriteControlRegister(min_CNTR_backup.CounterControlRegister);
        #endif /* (!min_CNTR_ControlRegRemoved) */

    #endif /* (!min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_Sleep
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
*  min_CNTR_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void min_CNTR_Sleep(void) 
{
    #if(!min_CNTR_ControlRegRemoved)
        /* Save Counter's enable state */
        if(min_CNTR_CTRL_ENABLE == (min_CNTR_CONTROL & min_CNTR_CTRL_ENABLE))
        {
            /* Counter is enabled */
            min_CNTR_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            min_CNTR_backup.CounterEnableState = 0u;
        }
    #else
        min_CNTR_backup.CounterEnableState = 1u;
        if(min_CNTR_backup.CounterEnableState != 0u)
        {
            min_CNTR_backup.CounterEnableState = 0u;
        }
    #endif /* (!min_CNTR_ControlRegRemoved) */
    
    min_CNTR_Stop();
    min_CNTR_SaveConfig();
}


/*******************************************************************************
* Function Name: min_CNTR_Wakeup
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
*  min_CNTR_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void min_CNTR_Wakeup(void) 
{
    min_CNTR_RestoreConfig();
    #if(!min_CNTR_ControlRegRemoved)
        if(min_CNTR_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            min_CNTR_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!min_CNTR_ControlRegRemoved) */
    
}


/* [] END OF FILE */
