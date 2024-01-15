#ifndef __TIME_SYNCHRO_H
#define __TIME_SYNCHRO_H

/************************* include section *************************/

#include "Timer.h"
#include "XCom_free.h"

/************************* define section *************************/

#define CYCLE 10

#define MULTIPLIER_WAIT_TIME 4

/************************* definition section *************************/

void Init_varTS(void);

void Set_ts(void);

void TS_Init(void);

#endif

/************************* end of file *************************/