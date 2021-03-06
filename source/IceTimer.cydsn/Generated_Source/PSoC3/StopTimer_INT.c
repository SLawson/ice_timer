/*******************************************************************************
* File Name: StopTimer_INT.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include <cydevice_trm.h>
#include <CyLib.h>
#include <StopTimer_INT.h>

#if !defined(StopTimer_INT__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START StopTimer_INT_intc` */
#include <project.h>
/* `#END` */


/*******************************************************************************
* Function Name: StopTimer_INT_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_Start(void) 
{
    /* For all we know the interrupt is active. */
    StopTimer_INT_Disable();

    /* Set the ISR to point to the StopTimer_INT Interrupt. */
    StopTimer_INT_SetVector(&StopTimer_INT_Interrupt);

    /* Set the priority. */
    StopTimer_INT_SetPriority((uint8)StopTimer_INT_INTC_PRIOR_NUMBER);

    /* Enable it. */
    StopTimer_INT_Enable();
}


/*******************************************************************************
* Function Name: StopTimer_INT_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_StartEx(cyisraddress address) 
{
    /* For all we know the interrupt is active. */
    StopTimer_INT_Disable();

    /* Set the ISR to point to the StopTimer_INT Interrupt. */
    StopTimer_INT_SetVector(address);

    /* Set the priority. */
    StopTimer_INT_SetPriority((uint8)StopTimer_INT_INTC_PRIOR_NUMBER);

    /* Enable it. */
    StopTimer_INT_Enable();
}


/*******************************************************************************
* Function Name: StopTimer_INT_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_Stop(void) 
{
    /* Disable this interrupt. */
    StopTimer_INT_Disable();
}


/*******************************************************************************
* Function Name: StopTimer_INT_Interrupt
********************************************************************************
* Summary:
*   The default Interrupt Service Routine for StopTimer_INT.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(StopTimer_INT_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START StopTimer_INT_Interrupt` */
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Stop Timer");
    stopTimer_ISR_ClearPending();
    
    /* `#END` */

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)
        #if((CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2) && (StopTimer_INT__ES2_PATCH ))      
            StopTimer_INT_ISR_PATCH();
        #endif /* CYDEV_CHIP_REVISION_USED */
    #endif /* (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */
}


/*******************************************************************************
* Function Name: StopTimer_INT_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling StopTimer_INT_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use StopTimer_INT_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_SetVector(cyisraddress address) 
{
    CY_SET_REG16(StopTimer_INT_INTC_VECTOR, (uint16) address);
}


/*******************************************************************************
* Function Name: StopTimer_INT_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress StopTimer_INT_GetVector(void) 
{
    return (cyisraddress) CY_GET_REG16(StopTimer_INT_INTC_VECTOR);
}


/*******************************************************************************
* Function Name: StopTimer_INT_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling StopTimer_INT_Start
*   or StopTimer_INT_StartEx will override any effect this method would 
*   have had. This method should only be called after StopTimer_INT_Start or 
*   StopTimer_INT_StartEx has been called. To set the initial
*   priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_SetPriority(uint8 priority) 
{
    *StopTimer_INT_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: StopTimer_INT_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*******************************************************************************/
uint8 StopTimer_INT_GetPriority(void) 
{
    uint8 priority;


    priority = *StopTimer_INT_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: StopTimer_INT_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_Enable(void) 
{
    /* Enable the general interrupt. */
    *StopTimer_INT_INTC_SET_EN = StopTimer_INT__INTC_MASK;
}


/*******************************************************************************
* Function Name: StopTimer_INT_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 StopTimer_INT_GetState(void) 
{
    /* Get the state of the general interrupt. */
    return ((*StopTimer_INT_INTC_SET_EN & (uint8)StopTimer_INT__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: StopTimer_INT_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_Disable(void) 
{
    /* Disable the general interrupt. */
    *StopTimer_INT_INTC_CLR_EN = StopTimer_INT__INTC_MASK;
}


/*******************************************************************************
* Function Name: StopTimer_INT_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_SetPending(void) 
{
    *StopTimer_INT_INTC_SET_PD = StopTimer_INT__INTC_MASK;
}


/*******************************************************************************
* Function Name: StopTimer_INT_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void StopTimer_INT_ClearPending(void) 
{
    *StopTimer_INT_INTC_CLR_PD = StopTimer_INT__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
