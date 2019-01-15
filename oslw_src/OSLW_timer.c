/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_timer.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"

#if OSLW_OS_ENABLE

void OSlwTimerClockCallBack(OSlwTimerSTU *pt)
{
    if(pt->TimerEnableFlag)
    {

        if(!(-- pt->Count) && pt->AlarmCBFun)
        {
            switch(pt->Mode)
            {
            case OSlwTimerMode_Once:
                pt->TimerEnableFlag=0;
                break;

            case OSlwTimerMode_Continue:
                pt->Count=pt->CountMax;
                break;

            default:

                break;
            }
            pt->AlarmCBFun(pt);
        }

    }

    return ;

}

#endif

