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
File    : IP_SNMP_AGENT_Conf.h
Purpose : SNMP agent add-on configuration file.
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef IP_SNMP_AGENT_CONF_H
#define IP_SNMP_AGENT_CONF_H

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#ifndef   DEBUG
  #define DEBUG  0
#endif

//
// Logging
//
#if (DEBUG || (defined(WIN32) && DEBUG))
  #if   (defined(__ICCARM__) || defined(__ICCRX__) || defined(__GNUC__) || defined(__SEGGER_CC__))
    //
    // - IAR ARM
    // - IAR RX
    // - GCC based
    // - SEGGER
    //
    #include "IP.h"
    #define IP_SNMP_AGENT_WARN(p)  IP_Warnf_Application p
    #define IP_SNMP_AGENT_LOG(p)   IP_Logf_Application  p
  #elif defined(WIN32)
    //
    // - Microsoft Visual Studio
    //
    #if (_MSC_VER < 1310)  // Before 2003 .NET ?
      #define IP_SNMP_AGENT_FUNCTION_NAME  ""  // Not available in Visual Studio below 2003 .NET.
    #else
      #define IP_SNMP_AGENT_FUNCTION_NAME  __FUNCTION__
    #endif
    void WIN32_OutputDebugStringf(const char * sFormat, ...);
    #define IP_SNMP_AGENT_WARN(p)  WIN32_OutputDebugStringf p
    #define IP_SNMP_AGENT_LOG(p)   WIN32_OutputDebugStringf p
  #else
    //
    // Other toolchains
    //
    #define IP_SNMP_AGENT_WARN(p)
    #define IP_SNMP_AGENT_LOG(p)
  #endif
#else
  //
  // Release builds
  //
  #define   IP_SNMP_AGENT_WARN(p)
  #define   IP_SNMP_AGENT_LOG(p)
#endif

//
// Panic check.
//
#ifndef     IP_SNMP_AGENT_SUPPORT_PANIC_CHECK
  #if DEBUG
    #define IP_SNMP_AGENT_SUPPORT_PANIC_CHECK  1
  #endif
#endif

//
// Other configurations.
// Can be disabled to support compiling on older toolchains (not C99 compliant).
//
#ifndef   IP_SNMP_AGENT_SUPPORT_64_BIT_TYPES
  #ifdef U64
    #define IP_SNMP_AGENT_SUPPORT_64_BIT_TYPES  1
  #else
    #define IP_SNMP_AGENT_SUPPORT_64_BIT_TYPES  0
  #endif
#endif


#endif     // Avoid multiple inclusion

/*************************** End of file ****************************/
