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
    LCD_Start();
    LCD_Position(0,0);
    LCD_PrintString("Ready to begin");
    LCD_Position(1,0);
    LCD_PrintString("timing...");

    startTimer_ISR_Start();
    stopTimer_ISR_Start();
    readyTimer_ISR_Start();
    
    CyGlobalIntEnable;

    for(;;)
    {
    }
}

