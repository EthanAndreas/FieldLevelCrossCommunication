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
File        : SEGGER_MEM_Conf.h
Purpose     : Configuration file for SEGGER memory allocators.
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SEGGER_MEM_CONF_H
#define SEGGER_MEM_CONF_H

//
// Define SEGGER_MEM_DEBUG: Debug level for SSL product
//                  0: No checks                      (Smallest and fastest code)
//                  1: Warnings & Panic checks
//                  2: Warnings, logs, & panic checks (Bigger code)
//
#ifndef   DEBUG
  #define DEBUG            0
#endif

#if DEBUG
  #ifndef   SEGGER_MEM_DEBUG
    #define SEGGER_MEM_DEBUG      2      // Default for debug builds
  #endif
  #ifndef   SEGGER_MEM_STATS
    #define SEGGER_MEM_STATS      1      // Default for debug builds, include statistics
  #endif
#else
  #ifndef   SEGGER_MEM_DEBUG
    #define SEGGER_MEM_DEBUG      0      // Default for release builds
  #endif
  #ifndef   SEGGER_MEM_STATS
    #define SEGGER_MEM_STATS      0      // Default for release builds, don't include statistics
  #endif
#endif

#endif

/****** End Of File *************************************************/
