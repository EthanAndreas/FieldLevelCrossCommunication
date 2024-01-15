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
File    : UDPCOM.h
Purpose : Header file for embOSView communication using UDP
*/

#ifndef UDPCOM_H
#define UDPCOM_H

void UDP_Process_Init (void);
void UDP_Process_Send1(char c);

#endif  // UDPCOM_H

/*************************** End of file ****************************/
