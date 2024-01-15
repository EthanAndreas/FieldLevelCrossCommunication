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

File    : SEGGER_SYSVIEW_Conf.h
Purpose : SEGGER SystemView configuration file.
          Set defines which deviate from the defaults (see SEGGER_SYSVIEW_ConfDefaults.h) here.
Revision: $Rev: 21292 $

Additional information:
  Required defines which must be set are:
    SEGGER_SYSVIEW_GET_TIMESTAMP
    SEGGER_SYSVIEW_GET_INTERRUPT_ID
  For known compilers and cores, these might be set to good defaults
  in SEGGER_SYSVIEW_ConfDefaults.h.

  SystemView needs a (nestable) locking mechanism.
  If not defined, the RTT locking mechanism is used,
  which then needs to be properly configured.
*/

#ifndef SEGGER_SYSVIEW_CONF_H
#define SEGGER_SYSVIEW_CONF_H

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
extern unsigned int SystemCoreClock;

#define SEGGER_SYSVIEW_APP_NAME        "embOS start project"
#define SEGGER_SYSVIEW_DEVICE_NAME     "STM32F429ZI"
#define SEGGER_SYSVIEW_ID_BASE         0x10000000
#define SEGGER_SYSVIEW_TIMESTAMP_FREQ  SystemCoreClock
#define SEGGER_SYSVIEW_CPU_FREQ        SystemCoreClock
#define SEGGER_SYSVIEW_SYSDESC0        "I#15=SysTick"

#endif  // SEGGER_SYSVIEW_CONF_H

/*************************** End of file ****************************/
