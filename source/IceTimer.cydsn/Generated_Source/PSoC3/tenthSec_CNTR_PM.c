/*******************************************************************************
* File Name: tenthSec_CNTR_PM.c  
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

#include "tenthSec_CNTR.h"

static tenthSec_CNTR_backupStruct tenthSec_CNTR_backup;


/*******************************************************************************
* Function Name: tenthSec_CNTR_SaveConfig
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
*  tenthSec_CNTR_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void tenthSec_CNTR_SaveConfig(void) 
{
    #if (!tenthSec_CNTR_UsingFixedFunction)

        tenthSec_CNTR_backup.CounterUdb = tenthSec_CNTR_ReadCounter();

        #if (CY_UDB_V0)
            tenthSec_CNTR_backup.CounterPeriod = tenthSec_CNTR_ReadPeriod();
            tenthSec_CNTR_backup.CompareValue = tenthSec_CNTR_ReadCompare();
            tenthSec_CNTR_backup.InterruptMaskValue = tenthSec_CNTR_STATUS_MASK;
        #endif /* CY_UDB_V0 */

        #if(!tenthSec_CNTR_ControlRegRemoved)
            tenthSec_CNTR_backup.CounterControlRegister = tenthSec_CNTR_ReadControlRegister();
        #endif /* (!tenthSec_CNTR_ControlRegRemoved) */

    #endif /* (!tenthSec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: tenthSec_CNTR_RestoreConfig
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
*  tenthSec_CNTR_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void tenthSec_CNTR_RestoreConfig(void) 
{      
    #if (!tenthSec_CNTR_UsingFixedFunction)

        #if (CY_UDB_V0)
            uint8 tenthSec_CNTR_interruptState;
        #endif  /* (CY_UDB_V0) */

       tenthSec_CNTR_WriteCounter(tenthSec_CNTR_backup.CounterUdb);

        #if (CY_UDB_V0)
            tenthSec_CNTR_WritePeriod(tenthSec_CNTR_backup.CounterPeriod);
            tenthSec_CNTR_WriteCompare(tenthSec_CNTR_backup.CompareValue);

            tenthSec_CNTR_interruptState = CyEnterCriticalSection();
            tenthSec_CNTR_STATUS_AUX_CTRL |= tenthSec_CNTR_STATUS_ACTL_INT_EN_MASK;
            CyExitCriticalSection(tenthSec_CNTR_interruptState);

            tenthSec_CNTR_STATUS_MASK = tenthSec_CNTR_backup.InterruptMaskValue;
        #endif  /* (CY_UDB_V0) */

        #if(!tenthSec_CNTR_ControlRegRemoved)
            tenthSec_CNTR_WriteControlRegister(tenthSec_CNTR_backup.CounterControlRegister);
        #endif /* (!tenthSec_CNTR_ControlRegRemoved) */

    #endif /* (!tenthSec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: tenthSec_CNTR_Sleep
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
*  tenthSec_CNTR_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void tenthSec_CNTR_Sleep(void) 
{
    #if(!tenthSec_CNTR_ControlRegRemoved)
        /* Save Counter's enable state */
        if(tenthSec_CNTR_CTRL_ENABLE == (tenthSec_CNTR_CONTROL & tenthSec_CNTR_CTRL_ENABLE))
        {
            /* Counter is enabled */
            tenthSec_CNTR_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            tenthSec_CNTR_backup.CounterEnableState = 0u;
        }
    #else
        tenthSec_CNTR_backup.CounterEnableState = 1u;
        if(tenthSec_CNTR_backup.CounterEnableState != 0u)
        {
            tenthSec_CNTR_backup.CounterEnableState = 0u;
        }
    #endif /* (!tenthSec_CNTR_ControlRegRemoved) */
    
    tenthSec_CNTR_Stop();
    tenthSec_CNTR_SaveConfig();
}


/*******************************************************************************
* Function Name: tenthSec_CNTR_Wakeup
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
*  tenthSec_CNTR_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void tenthSec_CNTR_Wakeup(void) 
{
    tenthSec_CNTR_RestoreConfig();
    #if(!tenthSec_CNTR_ControlRegRemoved)
        if(tenthSec_CNTR_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            tenthSec_CNTR_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!tenthSec_CNTR_ControlRegRemoved) */
    
}


/* [] END OF FILE */
