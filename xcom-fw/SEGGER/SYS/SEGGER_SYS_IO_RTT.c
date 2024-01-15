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
File        : SEGGER_SYS_IO_RTT.c
Purpose     : Implementation for API functions, debug output to
              RTT terminal #1, regular output to RTT Terminal #0.
Revision    : $Rev: 14031 $
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
#if USE_RTT
#include "SEGGER_RTT.h"
void SEGGER_SYS_IO_Init(void) {
  //
  // Only call SEGGER_RTT_Init() if your system needs it.
  //
#if 0
  static unsigned char Init;
  //
  if (Init == 0) {
    ++Init;
    SEGGER_RTT_Init();
  }
#endif
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
  SEGGER_RTT_WriteString(0, sText);
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
  SEGGER_RTT_TerminalOut(1, sText);
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
  unsigned i;
  //
  SEGGER_SYS_IO_Init();
  i = 0;
  for (;;) {
    int c;
    c = SEGGER_RTT_WaitKey();
    if (c < 0) {
      break;
    } else if (c == '\b') {
      if (i > 0) {
        --i;
        SEGGER_RTT_WriteString(0, " \b");
      }
    } else if (c == '\r' || c == '\n') {
      break;
    } else if (i+1 < TextByteCnt) {
      acText[i++] = c;
    }
  }
  acText[i] = 0;
  SEGGER_RTT_WriteString(0, "\r\n");
  return 0;
}

int SEGGER_SYS_IO_Getc(void) {
  return SEGGER_RTT_WaitKey();
}
#endif  // USE_RTT
/****** End Of File *************************************************/
