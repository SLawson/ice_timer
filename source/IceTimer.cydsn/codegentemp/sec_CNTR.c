/*******************************************************************************
* File Name: sec_CNTR.c  
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

#include "sec_CNTR.h"

uint8 sec_CNTR_initVar = 0u;


/*******************************************************************************
* Function Name: sec_CNTR_Init
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
void sec_CNTR_Init(void) 
{
        #if (!sec_CNTR_UsingFixedFunction && !sec_CNTR_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!sec_CNTR_UsingFixedFunction && !sec_CNTR_ControlRegRemoved) */
        
        #if(!sec_CNTR_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 sec_CNTR_interruptState;
        #endif /* (!sec_CNTR_UsingFixedFunction) */
        
        #if (sec_CNTR_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            sec_CNTR_CONTROL &= sec_CNTR_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                sec_CNTR_CONTROL2 &= ((uint8)(~sec_CNTR_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                sec_CNTR_CONTROL3 &= ((uint8)(~sec_CNTR_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (sec_CNTR_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                sec_CNTR_CONTROL |= sec_CNTR_ONESHOT;
            #endif /* (sec_CNTR_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            sec_CNTR_CONTROL2 |= sec_CNTR_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            sec_CNTR_RT1 &= ((uint8)(~sec_CNTR_RT1_MASK));
            sec_CNTR_RT1 |= sec_CNTR_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            sec_CNTR_RT1 &= ((uint8)(~sec_CNTR_SYNCDSI_MASK));
            sec_CNTR_RT1 |= sec_CNTR_SYNCDSI_EN;

        #else
            #if(!sec_CNTR_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = sec_CNTR_CONTROL & ((uint8)(~sec_CNTR_CTRL_CMPMODE_MASK));
            sec_CNTR_CONTROL = ctrl | sec_CNTR_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = sec_CNTR_CONTROL & ((uint8)(~sec_CNTR_CTRL_CAPMODE_MASK));
            
            #if( 0 != sec_CNTR_CAPTURE_MODE_CONF)
                sec_CNTR_CONTROL = ctrl | sec_CNTR_DEFAULT_CAPTURE_MODE;
            #else
                sec_CNTR_CONTROL = ctrl;
            #endif /* 0 != sec_CNTR_CAPTURE_MODE */ 
            
            #endif /* (!sec_CNTR_ControlRegRemoved) */
        #endif /* (sec_CNTR_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!sec_CNTR_UsingFixedFunction)
            sec_CNTR_ClearFIFO();
        #endif /* (!sec_CNTR_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        sec_CNTR_WritePeriod(sec_CNTR_INIT_PERIOD_VALUE);
        #if (!(sec_CNTR_UsingFixedFunction && (CY_PSOC5A)))
            sec_CNTR_WriteCounter(sec_CNTR_INIT_COUNTER_VALUE);
        #endif /* (!(sec_CNTR_UsingFixedFunction && (CY_PSOC5A))) */
        sec_CNTR_SetInterruptMode(sec_CNTR_INIT_INTERRUPTS_MASK);
        
        #if (!sec_CNTR_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)sec_CNTR_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            sec_CNTR_WriteCompare(sec_CNTR_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            sec_CNTR_interruptState = CyEnterCriticalSection();
            
            sec_CNTR_STATUS_AUX_CTRL |= sec_CNTR_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(sec_CNTR_interruptState);
            
        #endif /* (!sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_Enable
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
void sec_CNTR_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (sec_CNTR_UsingFixedFunction)
        sec_CNTR_GLOBAL_ENABLE |= sec_CNTR_BLOCK_EN_MASK;
        sec_CNTR_GLOBAL_STBY_ENABLE |= sec_CNTR_BLOCK_STBY_EN_MASK;
    #endif /* (sec_CNTR_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!sec_CNTR_ControlRegRemoved || sec_CNTR_UsingFixedFunction)
        sec_CNTR_CONTROL |= sec_CNTR_CTRL_ENABLE;                
    #endif /* (!sec_CNTR_ControlRegRemoved || sec_CNTR_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: sec_CNTR_Start
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
*  sec_CNTR_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void sec_CNTR_Start(void) 
{
    if(sec_CNTR_initVar == 0u)
    {
        sec_CNTR_Init();
        
        sec_CNTR_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    sec_CNTR_Enable();        
}


/*******************************************************************************
* Function Name: sec_CNTR_Stop
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
void sec_CNTR_Stop(void) 
{
    /* Disable Counter */
    #if(!sec_CNTR_ControlRegRemoved || sec_CNTR_UsingFixedFunction)
        sec_CNTR_CONTROL &= ((uint8)(~sec_CNTR_CTRL_ENABLE));        
    #endif /* (!sec_CNTR_ControlRegRemoved || sec_CNTR_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (sec_CNTR_UsingFixedFunction)
        sec_CNTR_GLOBAL_ENABLE &= ((uint8)(~sec_CNTR_BLOCK_EN_MASK));
        sec_CNTR_GLOBAL_STBY_ENABLE &= ((uint8)(~sec_CNTR_BLOCK_STBY_EN_MASK));
    #endif /* (sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_SetInterruptMode
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
void sec_CNTR_SetInterruptMode(uint8 interruptsMask) 
{
    sec_CNTR_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: sec_CNTR_ReadStatusRegister
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
uint8   sec_CNTR_ReadStatusRegister(void) 
{
    return sec_CNTR_STATUS;
}


#if(!sec_CNTR_ControlRegRemoved)
/*******************************************************************************
* Function Name: sec_CNTR_ReadControlRegister
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
uint8   sec_CNTR_ReadControlRegister(void) 
{
    return sec_CNTR_CONTROL;
}


/*******************************************************************************
* Function Name: sec_CNTR_WriteControlRegister
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
void    sec_CNTR_WriteControlRegister(uint8 control) 
{
    sec_CNTR_CONTROL = control;
}

#endif  /* (!sec_CNTR_ControlRegRemoved) */


#if (!(sec_CNTR_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: sec_CNTR_WriteCounter
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
void sec_CNTR_WriteCounter(uint8 counter) \
                                   
{
    #if(sec_CNTR_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (sec_CNTR_GLOBAL_ENABLE & sec_CNTR_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        sec_CNTR_GLOBAL_ENABLE |= sec_CNTR_BLOCK_EN_MASK;
        CY_SET_REG16(sec_CNTR_COUNTER_LSB_PTR, (uint16)counter);
        sec_CNTR_GLOBAL_ENABLE &= ((uint8)(~sec_CNTR_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(sec_CNTR_COUNTER_LSB_PTR, counter);
    #endif /* (sec_CNTR_UsingFixedFunction) */
}
#endif /* (!(sec_CNTR_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: sec_CNTR_ReadCounter
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
uint8 sec_CNTR_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(sec_CNTR_UsingFixedFunction)
		(void)CY_GET_REG16(sec_CNTR_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(sec_CNTR_COUNTER_LSB_PTR_8BIT);
	#endif/* (sec_CNTR_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(sec_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(sec_CNTR_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(sec_CNTR_STATICCOUNT_LSB_PTR));
    #endif /* (sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_ReadCapture
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
uint8 sec_CNTR_ReadCapture(void) 
{
    #if(sec_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(sec_CNTR_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(sec_CNTR_STATICCOUNT_LSB_PTR));
    #endif /* (sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_WritePeriod
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
void sec_CNTR_WritePeriod(uint8 period) 
{
    #if(sec_CNTR_UsingFixedFunction)
        CY_SET_REG16(sec_CNTR_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(sec_CNTR_PERIOD_LSB_PTR, period);
    #endif /* (sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_ReadPeriod
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
uint8 sec_CNTR_ReadPeriod(void) 
{
    #if(sec_CNTR_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(sec_CNTR_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(sec_CNTR_PERIOD_LSB_PTR));
    #endif /* (sec_CNTR_UsingFixedFunction) */
}


#if (!sec_CNTR_UsingFixedFunction)
/*******************************************************************************
* Function Name: sec_CNTR_WriteCompare
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
void sec_CNTR_WriteCompare(uint8 compare) \
                                   
{
    #if(sec_CNTR_UsingFixedFunction)
        CY_SET_REG16(sec_CNTR_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(sec_CNTR_COMPARE_LSB_PTR, compare);
    #endif /* (sec_CNTR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: sec_CNTR_ReadCompare
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
uint8 sec_CNTR_ReadCompare(void) 
{
    return (CY_GET_REG8(sec_CNTR_COMPARE_LSB_PTR));
}


#if (sec_CNTR_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: sec_CNTR_SetCompareMode
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
void sec_CNTR_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    sec_CNTR_CONTROL &= ((uint8)(~sec_CNTR_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    sec_CNTR_CONTROL |= compareMode;
}
#endif  /* (sec_CNTR_COMPARE_MODE_SOFTWARE) */


#if (sec_CNTR_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: sec_CNTR_SetCaptureMode
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
void sec_CNTR_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    sec_CNTR_CONTROL &= ((uint8)(~sec_CNTR_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    sec_CNTR_CONTROL |= ((uint8)((uint8)captureMode << sec_CNTR_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (sec_CNTR_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: sec_CNTR_ClearFIFO
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
void sec_CNTR_ClearFIFO(void) 
{

    while(0u != (sec_CNTR_ReadStatusRegister() & sec_CNTR_STATUS_FIFONEMP))
    {
        (void)sec_CNTR_ReadCapture();
    }

}
#endif  /* (!sec_CNTR_UsingFixedFunction) */


/* [] END OF FILE */

