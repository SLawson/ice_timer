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
    LCD_PrintString("Ice Timer");

    startTimer_ISR_Start();
    stopTimer_ISR_Start();
    CyGlobalIntEnable;

    for(;;)
    {
    }
}

