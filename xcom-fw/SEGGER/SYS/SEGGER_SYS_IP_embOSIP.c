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

File        : SEGGER_SYS_IP_embOSIP.c
Purpose     : Socket interface for embOS/IP.
Revision    : $Rev: 13631 $
*/


/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "RTOS.h"
#include "SEGGER_SYS.h"
#include "IP.h"

/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/

#define USE_RX_TASK 0

//
// Task priorities
//
enum {
  TASK_PRIO_IP_APP = 150,
  TASK_PRIO_IP_TASK,         // Priority should be higher as all TCP/IP application tasks
  TASK_PRIO_IP_RX_TASK       // Must be the highest priority of all TCP/IP related tasks, comment out to read packets in ISR
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static OS_STACKPTR int _IPStack[2048/sizeof(int)];       // Define the stack of the IP_Task to 768 bytes
static OS_TASK         _IPTCB;                            // Task-control-block
static int             _IPInited;

#if USE_RX_TASK
static OS_STACKPTR int _IPRxStack[1024/sizeof(int)];     // Define the stack of the IP_RxTask to 512 bytes
static OS_TASK         _IPRxTCB;
#endif


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       SEGGER_SYS_IP_Send()
*
*  Function description
*    Send data to socket.
*
*  Parameters
*    Socket  - Socket to write to.
*    pData   - Pointer to octet string to send.
*    DataLen - Octet length of the octet string to send.
*    Flags   - Socket send flags.
*
*  Return value
*    >= 0 - Number of bytes sent.
*    <  0 - Error.
*
*  Additional information
*    The number of bytes sent can be less than the number
*    of bytes that were requested to be written.
*/
int SEGGER_SYS_IP_Send(int Socket, const char *pData, int DataLen, int Flags) {
  int Status;
  //
  Status = send(Socket, pData, DataLen, Flags);
  //
  return Status < 0 ? -1 : Status;
}

/*********************************************************************
*
*       SEGGER_SYS_IP_Recv()
*
*  Function description
*    Receive data from socket.
*
*  Parameters
*    Socket  - Socket to read from.
*    pData   - Pointer to object that receives an octet string.
*    DataLen - Octet length of the octet string to receive.
*
*  Return value
*    >= 0 - Number of bytes received.
*    <  0 - Error.
*
*  Additional information
*    The number of bytes received can be less than the number
*    of bytes requested.
*/
int SEGGER_SYS_IP_Recv(int Socket, char *pData, int DataLen, int Flags) {
  int Status;
  //
  Status = recv(Socket, pData, DataLen, Flags);
  //
  return Status < 0 ? -1 : Status;
}

/*********************************************************************
*
*       SEGGER_SYS_IP_Open()
*
*  Function description
*    Open a socket.
*
*  Parameters
*    sHost - Host name to be resolved.
*    Port  - Port to open.
*
*  Return value
*    >= 0 - Success, socket handle.
*    <  0 - Error.
*/
int SEGGER_SYS_IP_Open(const char *sHost, int Port) {
  struct sockaddr_in sa;
  int    Socket;
  int    Status;
  U32    IPAddr;
  //
  // Decode host.
  //
  Status = IP_ResolveHost(sHost, &IPAddr, 1000);
  if (Status < 0) {
    return -1;
  }
  //
  Socket = socket(AF_INET, SOCK_STREAM, 0);
  if (Socket < 0) {
    Status = Socket;
  } else {
    sa.sin_family = AF_INET;
    sa.sin_port = htons(Port);
    sa.sin_addr.s_addr = IPAddr;
    Status = connect(Socket, (struct sockaddr *)&sa, sizeof(sa));
    if (Status < 0) {
      closesocket(Socket);
    }
  }
  return Status < 0 ? -1 : Socket;
}

/*********************************************************************
*
*       SEGGER_SYS_IP_Close()
*
*  Function description
*    Close a socket.
*
*  Parameters
*    Socket - Socket to close.
*
*  Return value
*    >= 0 - Success.
*    <  0 - Error.
*/
int SEGGER_SYS_IP_Close(int Socket) {
  if ((int)Socket <= 0) {
    return 0;
  } else {
    return closesocket(Socket);
  }
}

int SEGGER_SYS_IP_CloseWait(int Socket) {
  if (Socket >= 0) {
    (void)shutdown(Socket, 1);
    for (;;) {
      int res;
      char acBuf[128];
      res = recv(Socket, acBuf, sizeof(acBuf), 0);
      if (res < 0) {
        break;
      }
      if (!res) {
        break;
      }
    }
    return closesocket(Socket);
  } else {
    return 0;
  }
}

/*********************************************************************
*
*       SEGGER_SYS_IP_Bind()
*
*  Function description
*    Bind a TCP port.
*
*  Parameters
*    Port - Bound port.
*
*  Return value
*    >= 0 - Bound socket handle.
*    <  0 - Error.
*/
int SEGGER_SYS_IP_Bind(int Port) {
  int Socket;
  int Status;
  int Enable;
  struct sockaddr_in local_addr;

  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(Port);
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_LOOPBACK

  Socket = Status = socket(PF_INET, SOCK_STREAM, 0);
  if (Status >= 0) {
    Enable = 1;
    Status = setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&Enable, sizeof(Enable));
  }
  if (Status >= 0) {
    Status = bind(Socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
  }
  if (Status >= 0) {
    Status = listen(Socket, 10);
  }
  return Status >= 0 ? Socket : Status;
}

/*********************************************************************
*
*       SEGGER_SYS_IP_Accept()
*
*  Function description
*    Accept an incoming connection.
*
*  Parameters
*    Port - Bound port.
*
*  Return value
*    >= 0 - Socket handle.
*    <  0 - Error.
*/
int SEGGER_SYS_IP_Accept(int Socket) {
  struct sockaddr_in client_addr;
  int                client_addr_len;
  //
  client_addr_len = sizeof(client_addr);
  Socket = accept(Socket, (struct sockaddr *) &client_addr, &client_addr_len);
  if (Socket < 0) {
    return -1;
  } else {
    return Socket;
  }
}

int SEGGER_SYS_IP_QueryCanRead(int Socket) {
  IP_fd_set readset;
  int       Status;
  //
  IP_FD_ZERO(&readset);
  IP_FD_SET(Socket, &readset);
  Status = select(&readset, 0, 0, 0);
  //
  return Status > 0;
}

void SEGGER_SYS_IP_Init(void) {
  if (!_IPInited) {
    int IFaceId;
    _IPInited = 1;
    IP_Init();
    IFaceId = IP_INFO_GetNumInterfaces() - 1;  // Get the last registered interface ID as this is most likely the interface we want to use in this sample.
    OS_CREATETASK(&_IPTCB  , "IP_Task"  , IP_Task   , TASK_PRIO_IP_TASK   , _IPStack);    // Start the IP task
#if USE_RX_TASK 
    OS_CREATETASK(&_IPRxTCB, "IP_RxTask", IP_RxTask , TASK_PRIO_IP_RX_TASK, _IPRxStack);  // Start the IP_RxTask, optional.
#endif
    IP_Connect(IFaceId);
    while (IP_IFaceIsReadyEx(IFaceId) == 0) {
      OS_Delay(50);
    }
    OS_SetPriority(OS_GetTaskID(), TASK_PRIO_IP_APP);   // Set application priority
    OS_SetTaskName(OS_GetTaskID(), "MainTask");         // Set task name
  }
}

void SEGGER_SYS_IP_Exit(void) {
  OS_TerminateTask(&_IPTCB);
#if USE_RX_TASK 
  OS_TerminateTask(&_IPRxTCB);
#endif
}

/*************************** End of file ****************************/
