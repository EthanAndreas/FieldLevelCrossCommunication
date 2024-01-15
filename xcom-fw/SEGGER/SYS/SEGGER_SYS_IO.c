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
File    : SEGGER_SYS_IO.c
Purpose : Implementation of system API functions, system independent.
Revision: $Rev: 11360 $
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER_SYS.h"
#include <stdarg.h>
#include <stdio.h>

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       SEGGER_SYS_IO_Printf()
*
*  Function description
*    Output formatted string to standard output.
*
*  Parameters
*    sFormat - Format control string.
*/
void SEGGER_SYS_IO_Printf(const char *sFormat, ...) {
  va_list ap;
  va_start(ap, sFormat);
  SEGGER_SYS_IO_Printvf(sFormat, ap);
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Printvf()
*
*  Function description
*    Output formatted string to standard output.
*
*  Parameters
*    sFormat - Format control string.
*    Params  - Variable argument parameter list.
*/
void SEGGER_SYS_IO_Printvf(const char *sFormat, va_list Params) {
  char acBuf[1025];
  vsnprintf(acBuf, sizeof(acBuf)-1, sFormat, Params);
  SEGGER_SYS_IO_Print(acBuf);
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Errorf()
*
*  Function description
*    Output formatted string to error output.
*
*  Parameters
*    sFormat - Format control string.
*/
void SEGGER_SYS_IO_Errorf(const char *sFormat, ...) {
  va_list ap;
  va_start(ap, sFormat);
  SEGGER_SYS_IO_Errorvf(sFormat, ap);
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Errorvf()
*
*  Function description
*    Output formatted string to error output.
*
*  Parameters
*    sFormat - Format control string.
*    Params  - Variable argument parameter list.
*/
void SEGGER_SYS_IO_Errorvf(const char *sFormat, va_list Params) {
  char acBuf[128];
  vsnprintf(acBuf, sizeof(acBuf)-1, sFormat, Params);
  SEGGER_SYS_IO_Error(acBuf);
}

#if 0
/*********************************************************************
*
*       SEGGER_SYS_IO_Debugf()
*
*  Function description
*    Output formatted string to debug output.
*
*  Parameters
*    sFormat - Format control string.
*/
void SEGGER_SYS_IO_Debugf(const char *sFormat, ...) {
  va_list ap;
  va_start(ap, sFormat);
  SEGGER_SYS_IO_Debugvf(sFormat, ap);
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Debugvf()
*
*  Function description
*    Output formatted string to error output.
*
*  Parameters
*    sFormat - Format control string.
*    Params  - Variable argument parameter list.
*/
void SEGGER_SYS_IO_Debugvf(const char *sFormat, va_list Params) {
  char acBuf[128];
  vsnprintf(acBuf, sizeof(acBuf)-1, sFormat, Params);
  SEGGER_SYS_IO_Debug(acBuf);
}
#endif

/****** End Of File *************************************************/
