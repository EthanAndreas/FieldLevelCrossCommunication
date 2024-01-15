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
-------------------------- END-OF-HEADER -----------------------------
File    : OS_ThreadSafe.c
Purpose : Thread safe library functions
*/

#include "RTOS.h"
#include "__libc.h"

/**********************************************************************
*
*       __heap_lock()
*/
void __heap_lock(void) {
  OS_HeapLock();
}

/**********************************************************************
*
*       __heap_unlock()
*/
void __heap_unlock(void) {
  OS_HeapUnlock();
}

/**********************************************************************
*
*       __printf_lock()
*/
void __printf_lock(void) {
  OS_PrintfLock();
}

/**********************************************************************
*
*       __printf_unlock()
*/
void __printf_unlock(void) {
  OS_PrintfUnlock();
}

/**********************************************************************
*
*       __scanf_lock()
*/
void __scanf_lock(void) {
  OS_ScanfLock();
}

/**********************************************************************
*
*       __scanf_unlock()
*/
void __scanf_unlock(void) {
  OS_ScanfUnlock();
}

/**********************************************************************
*
*       __debug_io_lock()
*/
void __debug_io_lock(void) {
  OS_DebugIOLock();
}

/**********************************************************************
*
*       __debug_io_unlock()
*/
void __debug_io_unlock(void) {
  OS_DebugIOUnlock();
}

/*************************** End of file ****************************/
