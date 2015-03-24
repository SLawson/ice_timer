/*******************************************************************************
* File Name: sec_CNTR_PM.c  
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

#include "sec_CNTR.h"

static sec_CNTR_backupStruct sec_CNTR_backup;


/*******************************************************************************
* Function Name: sec_CNTR_SaveConfig
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
*  sec_CNTR_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void sec_CNTR_SaveConfig(void) 
{
    #if (!sec_CNTR_UsingFixedFunction)

        sec_CNTR_backup.CounterUdb = sec_CNTR_ReadCounter();

        #if (CY_UDB_V0)
            sec_CNTR_backup.CounterPeriod = sec_CNTR_ReadPeriod();
            sec_CNTR_backup.CompareValue = sec_CNTR_ReadCompare();
            sec_CNTR_backup.InterruptMaskValue = sec_CNTR_STATUS_MASK;
        #endif /* CY_UDB_V0 */

        #if(!sec_CNTR_ControlRegRemoved)
            sec_CNTR_backup.CounterControlRegister = sec_CNTR_ReadControlRegister();
        #endif /* (!sec_CNTR_ControlRegRemoved) */

    #endif /* (!sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_RestoreConfig
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
*  sec_CNTR_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void sec_CNTR_RestoreConfig(void) 
{      
    #if (!sec_CNTR_UsingFixedFunction)

        #if (CY_UDB_V0)
            uint8 sec_CNTR_interruptState;
        #endif  /* (CY_UDB_V0) */

       sec_CNTR_WriteCounter(sec_CNTR_backup.CounterUdb);

        #if (CY_UDB_V0)
            sec_CNTR_WritePeriod(sec_CNTR_backup.CounterPeriod);
            sec_CNTR_WriteCompare(sec_CNTR_backup.CompareValue);

            sec_CNTR_interruptState = CyEnterCriticalSection();
            sec_CNTR_STATUS_AUX_CTRL |= sec_CNTR_STATUS_ACTL_INT_EN_MASK;
            CyExitCriticalSection(sec_CNTR_interruptState);

            sec_CNTR_STATUS_MASK = sec_CNTR_backup.InterruptMaskValue;
        #endif  /* (CY_UDB_V0) */

        #if(!sec_CNTR_ControlRegRemoved)
            sec_CNTR_WriteControlRegister(sec_CNTR_backup.CounterControlRegister);
        #endif /* (!sec_CNTR_ControlRegRemoved) */

    #endif /* (!sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_Sleep
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
*  sec_CNTR_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void sec_CNTR_Sleep(void) 
{
    #if(!sec_CNTR_ControlRegRemoved)
        /* Save Counter's enable state */
        if(sec_CNTR_CTRL_ENABLE == (sec_CNTR_CONTROL & sec_CNTR_CTRL_ENABLE))
        {
            /* Counter is enabled */
            sec_CNTR_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            sec_CNTR_backup.CounterEnableState = 0u;
        }
    #else
        sec_CNTR_backup.CounterEnableState = 1u;
        if(sec_CNTR_backup.CounterEnableState != 0u)
        {
            sec_CNTR_backup.CounterEnableState = 0u;
        }
    #endif /* (!sec_CNTR_ControlRegRemoved) */
    
    sec_CNTR_Stop();
    sec_CNTR_SaveConfig();
}


/*******************************************************************************
* Function Name: sec_CNTR_Wakeup
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
*  sec_CNTR_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void sec_CNTR_Wakeup(void) 
{
    sec_CNTR_RestoreConfig();
    #if(!sec_CNTR_ControlRegRemoved)
        if(sec_CNTR_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            sec_CNTR_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!sec_CNTR_ControlRegRemoved) */
    
}


/* [] END OF FILE */
