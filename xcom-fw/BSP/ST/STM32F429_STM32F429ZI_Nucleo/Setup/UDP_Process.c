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
File    : UDP_Process.c
Purpose : UPD communication for embOSView
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"
#include "IP.h"
#include "UDPCOM.h"

/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/
#ifndef   EMBOSVIEW_UDP_PORT            // This is the default UDP port for embOSView communication.
  #define EMBOSVIEW_UDP_PORT   50021    // If you change it please modify it also in embOSView.
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PROTOCOL_SIZE  5                // Size of embOSView protocol bytes (SD0, SD1, Size, Checksum, ED)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static IP_HOOK_AFTER_INIT Hook;
static OS_U32             PeerAddr;
static OS_U16             PeerPort;

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _OnRx
*
* Function descrition
*   Client UDP callback. Called from stack
*   whenever we get a UDP packet from embOSView
*
* Notes
*   (1) Freeing In packet
*       With the return value, the IN-packet is freed by the stack
*/
static int _OnRx(IP_PACKET *pInPacket, void *pContext) {
  OS_U32 i;

  OS_USEPARA(pContext);
  //
  // Get IP address from incoming packet
  //
  IP_UDP_GetSrcAddr(pInPacket, &PeerAddr, sizeof(PeerAddr));
  PeerPort = IP_UDP_GetFPort(pInPacket);
  //
  // Handle received embOSView protocol
  //
  for (i = 0u; i < pInPacket->NumBytes; i++) {
    OS_OnRx(pInPacket->pData[i]);
  }

  return IP_OK;
}

/*********************************************************************
*
*       _SetupCallback
*
* Function descrition
*   Setup the UDP callback function for incoming UDP packets
*
* Notes
*   (1) This function is automatically called at the end of IP_Init()
*/
static void _SetupCallback(void) {
  IP_UDP_Open(0u, 0u, EMBOSVIEW_UDP_PORT, _OnRx, 0u);
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       UDP_Process_Send1()
*/
void UDP_Process_Send1(char c) {
  IP_PACKET *pOutPacket;
  OS_INT    SD1;
  OS_INT    NumBytes;
  OS_INT    i;

  //
  // Send UDP packet only if we already got a target address
  //
  if (PeerAddr != 0u) {
    SD1      = OS_COM_GetNextChar(); // Get SD1
    NumBytes = OS_COM_GetNextChar(); // Get NumBytes
    //
    // Alloc packet, we need a packet for the amount of data bytes plus protocol bytes
    //
    pOutPacket = IP_UDP_Alloc(NumBytes + PROTOCOL_SIZE);
    if (pOutPacket) {
      //
      // Copy protocol bytes and data in out UDP packet
      //
      pOutPacket->pData[0] = c;
      pOutPacket->pData[1] = SD1;
      pOutPacket->pData[2] = NumBytes;

      for (i = 0; i < NumBytes + 2; i++) {
        pOutPacket->pData[i + 3] = OS_COM_GetNextChar();
      }

      IP_UDP_SendAndFree(0, PeerAddr, PeerPort, EMBOSVIEW_UDP_PORT, pOutPacket);

      do {
        i = OS_COM_GetNextChar(); // Call the state machine until there are no more characters to send
      } while (i >= 0);
    } else {
      //
      // No buffer available, discard response
      //
      OS_COM_ClearTxActive();
    }
  } else {
    //
    // No target address so far, discard response
    //
    OS_COM_ClearTxActive();
  }
}

/*********************************************************************
*
*       UDP_Process_Init()
*
* Function description
*   Initializes the UDP communication for embOSView
*/
void UDP_Process_Init(void) {
  IP_AddAfterInitHook(&Hook, _SetupCallback);
  PeerAddr = 0;
  PeerPort = 0;
}

/****** End Of File *************************************************/
