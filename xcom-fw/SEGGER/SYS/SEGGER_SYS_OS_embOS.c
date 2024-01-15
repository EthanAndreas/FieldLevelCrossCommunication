/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2021  SEGGER Microcontroller GmbH                 *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : SEGGER_SYS_OS_embOS.c
Purpose : SEGGER OS abstraction layer, embOS implementation.
Revision: $Rev: 16688 $
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "SEGGER_SYS.h"
#include "Global.h"
#include "RTOS.h"

#ifndef   SEGGER_SYS_CONFIG_NUM_MUTEX
  #define SEGGER_SYS_CONFIG_NUM_MUTEX   3
#endif

// Read millisecond timer.
U32 SEGGER_SYS_OS_GetTime(void) {
  return OS_GetTime();
}

// Read timer with highest precision/resolution available (unit implementation defined)
U64 SEGGER_SYS_OS_GetTimer(void) {
  return OS_GetTime_us64();
}

// Get the frequency of the timer with highest precision/resolution available
U64 SEGGER_SYS_OS_GetFrequency(void) {
  return 1000000;
}

U32 SEGGER_SYS_GetProcessorSpeed(void) {
  extern U32 SystemCoreClock;
  //
  return SystemCoreClock;
}

// Convert MaxRes Timer value to us
U64 SEGGER_SYS_OS_ConvertTicksToMicros(U64 Ticks) {
  return Ticks * 1000000ULL / SEGGER_SYS_OS_GetFrequency();
}

U64 SEGGER_SYS_OS_ConvertMicrosToTicks(U64 Microseconds) {
  return SEGGER_SYS_OS_GetFrequency() * Microseconds / 1000000UL;
}

void SEGGER_SYS_OS_PauseBeforeHalt(void) {
  /* Nothing to do */
}

void SEGGER_SYS_OS_Halt(int Status) {
  //
  // SEGGER's unit test and benchmarking requires a "STOP" message.
  //
  SEGGER_SYS_IO_Printf("\nSTOP.\n\n");
  for (;;) {
    (void)Status;
  }
}

void SEGGER_SYS_OS_EnterCritical(void) {
  OS_IncDI();
}

void SEGGER_SYS_OS_LeaveCritical(void) {
  OS_DecRI();
}

U32 SEGGER_SYS_TIME_GetUnixTime(void) {
  return 0;
}

/*********************************************************************
*
*       SEGGER_SYS_OS_Delay()
*
*  Function description
*    Delay for at least a certain number of milliseconds.
*/
void SEGGER_SYS_OS_Delay(unsigned Milliseconds) {
  U64 Ticks;
#if (OS_VERSION_GENERIC < 50800u)
  Ticks = Milliseconds;
#else
  //
  // Ticks = Milliseconds is not always correct, just a common default.
  // OS_TIME_Convertms2Ticks() has been introduced with embos 5.08
  //
  Ticks = OS_TIME_Convertms2Ticks(Milliseconds);
#endif
  OS_Delay(Ticks);
}
