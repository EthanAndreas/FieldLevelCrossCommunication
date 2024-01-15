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
File    : JLINKMEM.h
Purpose : Header file for J-Link ARM communication using memory
*/

#ifndef JLINKMEM_H
#define JLINKMEM_H

#ifdef __cplusplus
extern "C" {
#endif

void JLINKMEM_Process(void);
void JLINKMEM_SetpfOnRx(void (* pf)(unsigned char Data));
void JLINKMEM_SetpfOnTx(unsigned char (* pf)(void));
void JLINKMEM_SetpfGetNextChar(OS_INT (* pf)(void));
void JLINKMEM_SendChar(unsigned char Data);

#ifdef __cplusplus
}
#endif

#endif  // JLINKMEM_H

/*************************** End of file ****************************/
