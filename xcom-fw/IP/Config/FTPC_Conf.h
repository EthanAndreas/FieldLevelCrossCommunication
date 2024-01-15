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
File    : FTPC_Conf.h
Purpose : FTP client add-on configuration file
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _FTPS_CONF_H_
#define _FTPS_CONF_H_ 1

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
    #define FTPC_WARN(p)  IP_Warnf_Application p
    #define FTPC_LOG(p)   IP_Logf_Application  p
  #elif defined(WIN32)
    //
    // - Microsoft Visual Studio
    //
    void WIN32_OutputDebugStringf(const char * sFormat, ...);
    #define FTPC_WARN(p)  WIN32_OutputDebugStringf p
    #define FTPC_LOG(p)   WIN32_OutputDebugStringf p
  #else
    //
    // Other toolchains
    //
    #define FTPC_WARN(p)
    #define FTPC_LOG(p)
  #endif
#else
  //
  // Release builds
  //
  #define   FTPC_WARN(p)
  #define   FTPC_LOG(p)
#endif

#define FTPC_BUFFER_SIZE               512
#define FTPC_CTRL_BUFFER_SIZE          256
#define FTPC_SERVER_REPLY_BUFFER_SIZE  128  // Only required in debug builds with enabled logging.

#endif     // Avoid multiple inclusion

/*************************** End of file ****************************/
