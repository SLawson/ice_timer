/* ========================================
 *
 * File:    main.c
 * Project: Ice Timer
 * Author:  Scott Lawson
 * Version: 2015.3.22
 *
 * ========================================
*/
#include <project.h>

int main()
{
    dbnc_CLK_Start();
    timer_CLK_Start();
    
    LCD_Start();
    LCD_Position(0,0);
    LCD_PrintString("Set arm to upper");
    LCD_Position(1,0);
    LCD_PrintString("position...");
    
    Timer_Start();
    tenthSec_CNTR_Start();
    sec_CNTR_Start();
    min_CNTR_Start();
    hr_CNTR_Start();
    
    startTimer_ISR_Start();
    stopTimer_ISR_Start();
    readyTimer_ISR_Start();
    
    CyGlobalIntEnable;

    for(;;)
    {
    }
}

