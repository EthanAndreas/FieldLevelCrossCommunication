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
File        : SEGGER_SYS_IO_printf.c
Purpose     : Implementation for API functions, using compiler
              provided printf()
Revision    : $Rev: 14082 $
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER_SYS.h"
#include "SEGGER_SYS_IO_ConfDefaults.h"
#include <string.h>

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
#if SYSIO_USE_PRINTF
#include <stdio.h>
void SEGGER_SYS_IO_Init(void) {
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Print()
*
*  Function description
*    Output a string to standard output.
*
*  Parameters
*    sText - Text to output, zero terminated.
*/
void SEGGER_SYS_IO_Print(const char *sText) {
  SEGGER_SYS_IO_Init();
  printf("%s", sText);
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Error()
*
*  Function description
*    Output a string to error output.
*
*  Parameters
*    sText - Text to output, zero terminated.
*/
void SEGGER_SYS_IO_Error(const char *sText) {
  SEGGER_SYS_IO_Init();
#if (defined(__ICCRX__) || defined(__ICCARM__))
  printf("%s", sText);
#else
  fputs(sText, stderr);
#endif
}

/*********************************************************************
*
*       SEGGER_SYS_IO_Gets()
*
*  Function description
*    Read a string from standard input.
*
*  Parameters
*    acText[TextByteCnt] - Buffer that contains text.
*
*  Return value
*     < 0 - Error reading string (end of file on standard input).
*    >= 0 - Number of characters read.
*/
int SEGGER_SYS_IO_Gets(char acText[], unsigned TextByteCnt) {
#if (defined(__ICCRX__) || defined(__ICCARM__))
  (void) acText;
  (void) TextByteCnt;
  return -1;
#else
  char* s;
  SEGGER_SYS_IO_Init();
  s = fgets(acText, TextByteCnt, stdin);
  if (s == NULL) {
    return -1;
  }
  return strlen(s);
#endif
}

int SEGGER_SYS_IO_Getc(void) {
  return getchar();
}
#endif  // SYSIO_USE_PRINTF
/****** End Of File *************************************************/
