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

File    : IP_COAP_Conf.h
Purpose : COAP configuration file.
*/

#ifndef IP_COAP_CONF_H  // Avoid multiple inclusion.
#define IP_COAP_CONF_H

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs.
#endif

//
// Includes for memset(), memcpy() and strlen().
//
#include <stdio.h>
#include <string.h>

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/


#ifndef   IP_COAP_NSTART
  #define IP_COAP_NSTART           1u     // Number of simultaneous activities between a client and a server.
#endif

#ifndef   IP_COAP_MAX_RETRANSMIT
  #define IP_COAP_MAX_RETRANSMIT   4u     // Number of CON retry.
#endif

#ifndef   IP_COAP_ACK_TIMEOUT
  #define IP_COAP_ACK_TIMEOUT      2000u  // Number of milliseconds to wait for an ACK.
#endif

#ifndef   IP_COAP_DEFAULT_LEISURE
  #define IP_COAP_DEFAULT_LEISURE  5000u  // Default leisure time in milliseconds to randomize the time to reply to a multicast NON request.
#endif

#ifndef   DEBUG
  #define DEBUG                    0
#endif

#ifndef   IP_COAP_MEMSET
  #define IP_COAP_MEMSET           memset
#endif

#ifndef   IP_COAP_MEMCPY
  #define IP_COAP_MEMCPY           memcpy
#endif

#ifndef   IP_COAP_MEMCMP
  #define IP_COAP_MEMCMP           memcmp
#endif

#ifndef   IP_COAP_MEMMOVE
  #define IP_COAP_MEMMOVE          memmove
#endif

#ifndef   IP_COAP_STRLEN
  #define IP_COAP_STRLEN           strlen
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
    #define IP_COAP_WARN(p)  IP_Warnf_Application p
    #define IP_COAP_LOG(p)   IP_Logf_Application  p
  #elif defined(WIN32)
    //
    // - Microsoft Visual Studio
    //
    void WIN32_OutputDebugStringf(const char * sFormat, ...);
    #define IP_COAP_WARN(p)  WIN32_OutputDebugStringf p
    #define IP_COAP_LOG(p)   WIN32_OutputDebugStringf p
  #else
    //
    // Other toolchains
    //
    #define IP_COAP_WARN(p)
    #define IP_COAP_LOG(p)
  #endif
#else
  //
  // Release builds
  //
  #define   IP_COAP_WARN(p)
  #define   IP_COAP_LOG(p)
#endif


#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

#endif                        // Avoid multiple inclusion.

/*************************** End of file ****************************/
