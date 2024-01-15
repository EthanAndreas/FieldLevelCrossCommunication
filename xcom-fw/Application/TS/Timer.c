/************************* include section *************************/

#include "Timer.h"

/************************* definition section *************************/

#define MAX_PERIOD 0x7FFF
#define TS_PERIOD  10000  // 30s

static OS_TIMER clockTimer, initMeasTimer, measTimer, calcTimer, tsTimer;

static latency_t latency[MAX_SENSORS - 1] = {0};
static bool_t    started                  = false;

void cb_clock(void)
{
  OS_TIMER_Restart(&clockTimer);
}

void Init_clock()
{
  TS_Log("Init clock");

  OS_TIMER_Create(&clockTimer, cb_clock, MAX_PERIOD);
  OS_TIMER_Start(&clockTimer);
}

uint64_t Get_clock()
{
  return OS_TIMER_GetPeriod(&clockTimer) - OS_TIMER_GetRemainingPeriod(&clockTimer);
}

void Stop_clock(void)
{
  OS_TIMER_Stop(&clockTimer);
}


void cb_latency1(void)
{
  Set_pressure(Get_confSensorIndex(latency[0].ip), 0, 0, QUALITY_CODE_ERROR);

  if ((latency[0].count++) >= Get_confCycle().frameLoss)
  {
    Interrupt_Log("Connection lost with a sensor");

    Set_confSensorLessOne(latency[0].ip);
    OS_TIMER_Stop(&(latency[0].timer));
  }
  else
  {
    Interrupt_Log("Packet not received in time");
    
    latency[0].count++;
    OS_TIMER_Restart(&(latency[0].timer));
  }
}

void cb_latency2(void)
{
  Set_pressure(Get_confSensorIndex(latency[1].ip), 0, 0, QUALITY_CODE_ERROR);

  if ((latency[1].count++) >= Get_confCycle().frameLoss)
  {
    Interrupt_Log("Connection lost with a sensor");

    Set_confSensorLessOne(latency[1].ip);
    OS_TIMER_Stop(&(latency[1].timer));
  }
  else
  {
    Interrupt_Log("Packet not received in time");

    latency[1].count++;
    OS_TIMER_Restart(&(latency[1].timer));
  }
}

void Init_latencyTimer(uint64_t period, uint32_t latency1, uint32_t latency2)
{
  TS_Log("Init latency timer");

  if (latency1 != 0)
  {
    latency[0].ip = latency1;

    if (started == false)
    {
      OS_TIMER_Create(&(latency[0].timer), cb_latency1, period);
      OS_TIMER_Start(&(latency[0].timer));
    }
    else 
    {
      OS_TIMER_Restart(&(latency[0].timer));
    }
  }

  if (latency2 != 0)
  {
    latency[1].ip = latency2;

    if (started == false)
    {
      OS_TIMER_Create(&(latency[1].timer), cb_latency2, period);
      OS_TIMER_Start(&(latency[1].timer));
    }
    else
    {
      OS_TIMER_Restart(&(latency[1].timer));
    }
  }
}

void Restart_latencyOneTimer(uint32_t ip)
{
  if (ip == latency[0].ip)
  {
    latency[0].count = 0;
    OS_TIMER_Restart(&(latency[0].timer));
  }
  else if (ip == latency[1].ip)
  {
    latency[1].count = 0;
    OS_TIMER_Restart(&(latency[1].timer));
  }
}

void Restart_latencyAllTimer(void)
{
  if (latency[0].ip != 0)
  {
    latency[0].count = 0;
    OS_TIMER_Restart(&(latency[0].timer));
  }

  if (latency[1].ip != 0)
  {
    latency[1].count = 0;
    OS_TIMER_Restart(&(latency[1].timer));
  }
}

void Stop_latencyTimer(void)
{
  if (latency[0].ip != 0)
  {
    latency[0].count = 0;
    OS_TIMER_Stop(&(latency[0].timer));
  }

  if (latency[1].ip != 0)
  {
    latency[1].count = 0;
    OS_TIMER_Stop(&(latency[1].timer));
  }
}


void cb_calculation(void)
{
  Set_taskEvent(EVENT_CALCULATION);

  OS_TIMER_Restart(&calcTimer);
}

void Init_calcTimer(uint64_t period)
{
  TS_Log("Init calculation timer");

  if (started == false)
  {
    OS_TIMER_Create(&calcTimer, cb_calculation, period);
    OS_TIMER_Start(&calcTimer);
  }
  else
  {
    OS_TIMER_Restart(&calcTimer);
  }
}

void Stop_calcTimer(void)
{
  OS_TIMER_Stop(&calcTimer);
}


void cb_measurement(void)
{
  Set_taskEvent(EVENT_MEASUREMENT);

  OS_TIMER_Restart(&measTimer);
}

void cb_init_measurement(void)
{
  Interrupt_Log("Init measurement timer");

  OS_TIMER_Restart(&measTimer);

  OS_TIMER_Stop(&initMeasTimer);
}

void Init_measTimer(uint64_t wait, uint64_t period)
{
  TS_Log("Wait before launching measurement");

  if (wait < 1)
  {
    // In the case of the latency is near 0, wait one tick to
    // place the measurement timer after the calculation one
    wait = 1;
  }

  if (started == false)
  {
    OS_TIMER_Create(&initMeasTimer, cb_init_measurement, wait);
    OS_TIMER_Start(&initMeasTimer);

    OS_TIMER_Create(&measTimer, cb_measurement, period);
  }
  else
  {
    OS_TIMER_Restart(&initMeasTimer);
  }
}

void Stop_measTimer(void)
{
  OS_TIMER_Stop(&measTimer);
}


void cb_ts(void)
{
  Stop_latencyTimer();
  Stop_calcTimer();
  Stop_measTimer();

  Set_taskEvent(EVENT_TS);
}

void Init_tsTimer(void)
{
  TS_Log("Init TS timer");

  OS_Delay(1);

  if (started == false)
  {
    OS_TIMER_Create(&tsTimer, cb_ts, OS_TIME_Convertms2Ticks(TS_PERIOD));
    OS_TIMER_Start(&tsTimer);
  }
  else
  {
    OS_TIMER_Restart(&tsTimer);
  }
}

void Stop_tsTimer(void)
{
  OS_TIMER_Stop(&tsTimer);
}


void End_timerInit(void)
{
  // wait for last timer (measurement timer) to be running
  while (OS_TIMER_GetStatus(&measTimer) == 0)
    ;

  started = true;
}

/************************* end of file *************************/