/*******************************************************************************
* File Name: min_CNTR.c  
* Version 2.40
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 min_CNTR_initVar = 0u;


/*******************************************************************************
* Function Name: min_CNTR_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void min_CNTR_Init(void) 
{
        #if (!min_CNTR_UsingFixedFunction && !min_CNTR_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!min_CNTR_UsingFixedFunction && !min_CNTR_ControlRegRemoved) */
        
        #if(!min_CNTR_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 min_CNTR_interruptState;
        #endif /* (!min_CNTR_UsingFixedFunction) */
        
        #if (min_CNTR_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            min_CNTR_CONTROL &= min_CNTR_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                min_CNTR_CONTROL2 &= ((uint8)(~min_CNTR_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                min_CNTR_CONTROL3 &= ((uint8)(~min_CNTR_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (min_CNTR_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                min_CNTR_CONTROL |= min_CNTR_ONESHOT;
            #endif /* (min_CNTR_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            min_CNTR_CONTROL2 |= min_CNTR_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            min_CNTR_RT1 &= ((uint8)(~min_CNTR_RT1_MASK));
            min_CNTR_RT1 |= min_CNTR_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            min_CNTR_RT1 &= ((uint8)(~min_CNTR_SYNCDSI_MASK));
            min_CNTR_RT1 |= min_CNTR_SYNCDSI_EN;

        #else
            #if(!min_CNTR_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = min_CNTR_CONTROL & ((uint8)(~min_CNTR_CTRL_CMPMODE_MASK));
            min_CNTR_CONTROL = ctrl | min_CNTR_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = min_CNTR_CONTROL & ((uint8)(~min_CNTR_CTRL_CAPMODE_MASK));
            
            #if( 0 != min_CNTR_CAPTURE_MODE_CONF)
                min_CNTR_CONTROL = ctrl | min_CNTR_DEFAULT_CAPTURE_MODE;
            #else
                min_CNTR_CONTROL = ctrl;
            #endif /* 0 != min_CNTR_CAPTURE_MODE */ 
            
            #endif /* (!min_CNTR_ControlRegRemoved) */
        #endif /* (min_CNTR_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!min_CNTR_UsingFixedFunction)
            min_CNTR_ClearFIFO();
        #endif /* (!min_CNTR_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        min_CNTR_WritePeriod(min_CNTR_INIT_PERIOD_VALUE);
        #if (!(min_CNTR_UsingFixedFunction && (CY_PSOC5A)))
            min_CNTR_WriteCounter(min_CNTR_INIT_COUNTER_VALUE);
        #endif /* (!(min_CNTR_UsingFixedFunction && (CY_PSOC5A))) */
        min_CNTR_SetInterruptMode(min_CNTR_INIT_INTERRUPTS_MASK);
        
        #if (!min_CNTR_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)min_CNTR_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            min_CNTR_WriteCompare(min_CNTR_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            min_CNTR_interruptState = CyEnterCriticalSection();
            
            min_CNTR_STATUS_AUX_CTRL |= min_CNTR_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(min_CNTR_interruptState);
            
        #endif /* (!min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void min_CNTR_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (min_CNTR_UsingFixedFunction)
        min_CNTR_GLOBAL_ENABLE |= min_CNTR_BLOCK_EN_MASK;
        min_CNTR_GLOBAL_STBY_ENABLE |= min_CNTR_BLOCK_STBY_EN_MASK;
    #endif /* (min_CNTR_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!min_CNTR_ControlRegRemoved || min_CNTR_UsingFixedFunction)
        min_CNTR_CONTROL |= min_CNTR_CTRL_ENABLE;                
    #endif /* (!min_CNTR_ControlRegRemoved || min_CNTR_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: min_CNTR_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  min_CNTR_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void min_CNTR_Start(void) 
{
    if(min_CNTR_initVar == 0u)
    {
        min_CNTR_Init();
        
        min_CNTR_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    min_CNTR_Enable();        
}


/*******************************************************************************
* Function Name: min_CNTR_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void min_CNTR_Stop(void) 
{
    /* Disable Counter */
    #if(!min_CNTR_ControlRegRemoved || min_CNTR_UsingFixedFunction)
        min_CNTR_CONTROL &= ((uint8)(~min_CNTR_CTRL_ENABLE));        
    #endif /* (!min_CNTR_ControlRegRemoved || min_CNTR_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (min_CNTR_UsingFixedFunction)
        min_CNTR_GLOBAL_ENABLE &= ((uint8)(~min_CNTR_BLOCK_EN_MASK));
        min_CNTR_GLOBAL_STBY_ENABLE &= ((uint8)(~min_CNTR_BLOCK_STBY_EN_MASK));
    #endif /* (min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void min_CNTR_SetInterruptMode(uint8 interruptsMask) 
{
    min_CNTR_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: min_CNTR_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   min_CNTR_ReadStatusRegister(void) 
{
    return min_CNTR_STATUS;
}


#if(!min_CNTR_ControlRegRemoved)
/*******************************************************************************
* Function Name: min_CNTR_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   min_CNTR_ReadControlRegister(void) 
{
    return min_CNTR_CONTROL;
}


/*******************************************************************************
* Function Name: min_CNTR_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    min_CNTR_WriteControlRegister(uint8 control) 
{
    min_CNTR_CONTROL = control;
}

#endif  /* (!min_CNTR_ControlRegRemoved) */


#if (!(min_CNTR_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: min_CNTR_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void min_CNTR_WriteCounter(uint8 counter) \
                                   
{
    #if(min_CNTR_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (min_CNTR_GLOBAL_ENABLE & min_CNTR_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        min_CNTR_GLOBAL_ENABLE |= min_CNTR_BLOCK_EN_MASK;
        CY_SET_REG16(min_CNTR_COUNTER_LSB_PTR, (uint16)counter);
        min_CNTR_GLOBAL_ENABLE &= ((uint8)(~min_CNTR_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(min_CNTR_COUNTER_LSB_PTR, counter);
    #endif /* (min_CNTR_UsingFixedFunction) */
}
#endif /* (!(min_CNTR_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: min_CNTR_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 min_CNTR_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(min_CNTR_UsingFixedFunction)
		(void)CY_GET_REG16(min_CNTR_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(min_CNTR_COUNTER_LSB_PTR_8BIT);
	#endif/* (min_CNTR_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(min_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(min_CNTR_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(min_CNTR_STATICCOUNT_LSB_PTR));
    #endif /* (min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 min_CNTR_ReadCapture(void) 
{
    #if(min_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(min_CNTR_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(min_CNTR_STATICCOUNT_LSB_PTR));
    #endif /* (min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void min_CNTR_WritePeriod(uint8 period) 
{
    #if(min_CNTR_UsingFixedFunction)
        CY_SET_REG16(min_CNTR_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(min_CNTR_PERIOD_LSB_PTR, period);
    #endif /* (min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 min_CNTR_ReadPeriod(void) 
{
    #if(min_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(min_CNTR_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(min_CNTR_PERIOD_LSB_PTR));
    #endif /* (min_CNTR_UsingFixedFunction) */
}


#if (!min_CNTR_UsingFixedFunction)
/*******************************************************************************
* Function Name: min_CNTR_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void min_CNTR_WriteCompare(uint8 compare) \
                                   
{
    #if(min_CNTR_UsingFixedFunction)
        CY_SET_REG16(min_CNTR_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(min_CNTR_COMPARE_LSB_PTR, compare);
    #endif /* (min_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: min_CNTR_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 min_CNTR_ReadCompare(void) 
{
    return (CY_GET_REG8(min_CNTR_COMPARE_LSB_PTR));
}


#if (min_CNTR_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: min_CNTR_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void min_CNTR_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    min_CNTR_CONTROL &= ((uint8)(~min_CNTR_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    min_CNTR_CONTROL |= compareMode;
}
#endif  /* (min_CNTR_COMPARE_MODE_SOFTWARE) */


#if (min_CNTR_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: min_CNTR_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void min_CNTR_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    min_CNTR_CONTROL &= ((uint8)(~min_CNTR_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    min_CNTR_CONTROL |= ((uint8)((uint8)captureMode << min_CNTR_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (min_CNTR_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: min_CNTR_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void min_CNTR_ClearFIFO(void) 
{

    while(0u != (min_CNTR_ReadStatusRegister() & min_CNTR_STATUS_FIFONEMP))
    {
        (void)min_CNTR_ReadCapture();
    }

}
#endif  /* (!min_CNTR_UsingFixedFunction) */


/* [] END OF FILE */

