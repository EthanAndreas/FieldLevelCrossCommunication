#ifndef __TIMER_H
#define __TIMER_H

/************************* include section *************************/

#include "Calculation.h"
#include "Configuration.h"
#include "Measurement.h"
#include "Task.h"
#include "UDP_send.h"
#include "XCom_decode.h"
#include "XCom_helper.h"
#include "XCom_protocol.h"

/************************* define section *************************/

#define TS_Log IP_Log

/************************* definition section *************************/

typedef struct 
{
  OS_TIMER timer;
  uint32_t ip;
  uint8_t count;
} latency_t;


void Init_clock(void);
uint64_t Get_clock(void);
void Stop_clock(void);

void Init_latencyTimer(uint64_t period, uint32_t latency1, uint32_t latency2);
void Restart_latencyOneTimer(uint32_t ip);
void Restart_latencyAllTimer(void);
void Stop_latencyTimer(void);

void Init_calcTimer(uint64_t period);
void Stop_calcTimer(void);

void Init_measTimer(uint64_t wait, uint64_t period);
void Stop_measTimer(void);

void Init_tsTimer(void);
void Stop_tsTimer(void);

void End_timerInit(void);

#endif

/************************* end of file *************************/