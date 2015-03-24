/*******************************************************************************
* File Name: hr_CNTR.c  
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

#include "hr_CNTR.h"

uint8 hr_CNTR_initVar = 0u;


/*******************************************************************************
* Function Name: hr_CNTR_Init
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
void hr_CNTR_Init(void) 
{
        #if (!hr_CNTR_UsingFixedFunction && !hr_CNTR_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!hr_CNTR_UsingFixedFunction && !hr_CNTR_ControlRegRemoved) */
        
        #if(!hr_CNTR_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 hr_CNTR_interruptState;
        #endif /* (!hr_CNTR_UsingFixedFunction) */
        
        #if (hr_CNTR_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            hr_CNTR_CONTROL &= hr_CNTR_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                hr_CNTR_CONTROL2 &= ((uint8)(~hr_CNTR_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                hr_CNTR_CONTROL3 &= ((uint8)(~hr_CNTR_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (hr_CNTR_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                hr_CNTR_CONTROL |= hr_CNTR_ONESHOT;
            #endif /* (hr_CNTR_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            hr_CNTR_CONTROL2 |= hr_CNTR_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            hr_CNTR_RT1 &= ((uint8)(~hr_CNTR_RT1_MASK));
            hr_CNTR_RT1 |= hr_CNTR_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            hr_CNTR_RT1 &= ((uint8)(~hr_CNTR_SYNCDSI_MASK));
            hr_CNTR_RT1 |= hr_CNTR_SYNCDSI_EN;

        #else
            #if(!hr_CNTR_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = hr_CNTR_CONTROL & ((uint8)(~hr_CNTR_CTRL_CMPMODE_MASK));
            hr_CNTR_CONTROL = ctrl | hr_CNTR_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = hr_CNTR_CONTROL & ((uint8)(~hr_CNTR_CTRL_CAPMODE_MASK));
            
            #if( 0 != hr_CNTR_CAPTURE_MODE_CONF)
                hr_CNTR_CONTROL = ctrl | hr_CNTR_DEFAULT_CAPTURE_MODE;
            #else
                hr_CNTR_CONTROL = ctrl;
            #endif /* 0 != hr_CNTR_CAPTURE_MODE */ 
            
            #endif /* (!hr_CNTR_ControlRegRemoved) */
        #endif /* (hr_CNTR_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!hr_CNTR_UsingFixedFunction)
            hr_CNTR_ClearFIFO();
        #endif /* (!hr_CNTR_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        hr_CNTR_WritePeriod(hr_CNTR_INIT_PERIOD_VALUE);
        #if (!(hr_CNTR_UsingFixedFunction && (CY_PSOC5A)))
            hr_CNTR_WriteCounter(hr_CNTR_INIT_COUNTER_VALUE);
        #endif /* (!(hr_CNTR_UsingFixedFunction && (CY_PSOC5A))) */
        hr_CNTR_SetInterruptMode(hr_CNTR_INIT_INTERRUPTS_MASK);
        
        #if (!hr_CNTR_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)hr_CNTR_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            hr_CNTR_WriteCompare(hr_CNTR_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            hr_CNTR_interruptState = CyEnterCriticalSection();
            
            hr_CNTR_STATUS_AUX_CTRL |= hr_CNTR_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(hr_CNTR_interruptState);
            
        #endif /* (!hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_Enable
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
void hr_CNTR_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (hr_CNTR_UsingFixedFunction)
        hr_CNTR_GLOBAL_ENABLE |= hr_CNTR_BLOCK_EN_MASK;
        hr_CNTR_GLOBAL_STBY_ENABLE |= hr_CNTR_BLOCK_STBY_EN_MASK;
    #endif /* (hr_CNTR_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!hr_CNTR_ControlRegRemoved || hr_CNTR_UsingFixedFunction)
        hr_CNTR_CONTROL |= hr_CNTR_CTRL_ENABLE;                
    #endif /* (!hr_CNTR_ControlRegRemoved || hr_CNTR_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: hr_CNTR_Start
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
*  hr_CNTR_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void hr_CNTR_Start(void) 
{
    if(hr_CNTR_initVar == 0u)
    {
        hr_CNTR_Init();
        
        hr_CNTR_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    hr_CNTR_Enable();        
}


/*******************************************************************************
* Function Name: hr_CNTR_Stop
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
void hr_CNTR_Stop(void) 
{
    /* Disable Counter */
    #if(!hr_CNTR_ControlRegRemoved || hr_CNTR_UsingFixedFunction)
        hr_CNTR_CONTROL &= ((uint8)(~hr_CNTR_CTRL_ENABLE));        
    #endif /* (!hr_CNTR_ControlRegRemoved || hr_CNTR_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (hr_CNTR_UsingFixedFunction)
        hr_CNTR_GLOBAL_ENABLE &= ((uint8)(~hr_CNTR_BLOCK_EN_MASK));
        hr_CNTR_GLOBAL_STBY_ENABLE &= ((uint8)(~hr_CNTR_BLOCK_STBY_EN_MASK));
    #endif /* (hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_SetInterruptMode
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
void hr_CNTR_SetInterruptMode(uint8 interruptsMask) 
{
    hr_CNTR_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: hr_CNTR_ReadStatusRegister
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
uint8   hr_CNTR_ReadStatusRegister(void) 
{
    return hr_CNTR_STATUS;
}


#if(!hr_CNTR_ControlRegRemoved)
/*******************************************************************************
* Function Name: hr_CNTR_ReadControlRegister
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
uint8   hr_CNTR_ReadControlRegister(void) 
{
    return hr_CNTR_CONTROL;
}


/*******************************************************************************
* Function Name: hr_CNTR_WriteControlRegister
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
void    hr_CNTR_WriteControlRegister(uint8 control) 
{
    hr_CNTR_CONTROL = control;
}

#endif  /* (!hr_CNTR_ControlRegRemoved) */


#if (!(hr_CNTR_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: hr_CNTR_WriteCounter
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
void hr_CNTR_WriteCounter(uint8 counter) \
                                   
{
    #if(hr_CNTR_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (hr_CNTR_GLOBAL_ENABLE & hr_CNTR_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        hr_CNTR_GLOBAL_ENABLE |= hr_CNTR_BLOCK_EN_MASK;
        CY_SET_REG16(hr_CNTR_COUNTER_LSB_PTR, (uint16)counter);
        hr_CNTR_GLOBAL_ENABLE &= ((uint8)(~hr_CNTR_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(hr_CNTR_COUNTER_LSB_PTR, counter);
    #endif /* (hr_CNTR_UsingFixedFunction) */
}
#endif /* (!(hr_CNTR_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: hr_CNTR_ReadCounter
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
uint8 hr_CNTR_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(hr_CNTR_UsingFixedFunction)
		(void)CY_GET_REG16(hr_CNTR_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(hr_CNTR_COUNTER_LSB_PTR_8BIT);
	#endif/* (hr_CNTR_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(hr_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(hr_CNTR_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(hr_CNTR_STATICCOUNT_LSB_PTR));
    #endif /* (hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_ReadCapture
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
uint8 hr_CNTR_ReadCapture(void) 
{
    #if(hr_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(hr_CNTR_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(hr_CNTR_STATICCOUNT_LSB_PTR));
    #endif /* (hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_WritePeriod
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
void hr_CNTR_WritePeriod(uint8 period) 
{
    #if(hr_CNTR_UsingFixedFunction)
        CY_SET_REG16(hr_CNTR_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(hr_CNTR_PERIOD_LSB_PTR, period);
    #endif /* (hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_ReadPeriod
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
uint8 hr_CNTR_ReadPeriod(void) 
{
    #if(hr_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(hr_CNTR_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(hr_CNTR_PERIOD_LSB_PTR));
    #endif /* (hr_CNTR_UsingFixedFunction) */
}


#if (!hr_CNTR_UsingFixedFunction)
/*******************************************************************************
* Function Name: hr_CNTR_WriteCompare
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
void hr_CNTR_WriteCompare(uint8 compare) \
                                   
{
    #if(hr_CNTR_UsingFixedFunction)
        CY_SET_REG16(hr_CNTR_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(hr_CNTR_COMPARE_LSB_PTR, compare);
    #endif /* (hr_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: hr_CNTR_ReadCompare
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
uint8 hr_CNTR_ReadCompare(void) 
{
    return (CY_GET_REG8(hr_CNTR_COMPARE_LSB_PTR));
}


#if (hr_CNTR_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: hr_CNTR_SetCompareMode
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
void hr_CNTR_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    hr_CNTR_CONTROL &= ((uint8)(~hr_CNTR_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    hr_CNTR_CONTROL |= compareMode;
}
#endif  /* (hr_CNTR_COMPARE_MODE_SOFTWARE) */


#if (hr_CNTR_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: hr_CNTR_SetCaptureMode
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
void hr_CNTR_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    hr_CNTR_CONTROL &= ((uint8)(~hr_CNTR_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    hr_CNTR_CONTROL |= ((uint8)((uint8)captureMode << hr_CNTR_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (hr_CNTR_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: hr_CNTR_ClearFIFO
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
void hr_CNTR_ClearFIFO(void) 
{

    while(0u != (hr_CNTR_ReadStatusRegister() & hr_CNTR_STATUS_FIFONEMP))
    {
        (void)hr_CNTR_ReadCapture();
    }

}
#endif  /* (!hr_CNTR_UsingFixedFunction) */


/* [] END OF FILE */

