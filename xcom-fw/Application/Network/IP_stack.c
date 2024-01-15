/************************* include section *************************/

#include "IP_stack.h"

#include "Configuration.h"

/************************* definition section *************************/

// IP TCB and stack
static OS_STACKPTR int ipStack[TASK_STACK_SIZE_IP_TASK / sizeof(int) + MAX_MSG_SIZE];
static OS_TASK         ipTCB;

// Webserver TCBs and stacks
static OS_TASK         WebParentTask;
static OS_STACKPTR int WebParentStack[TASK_STACK_SIZE_WEB_TASK];
static OS_TASK         WebChildTasks[MAX_CONNECTIONS];
static OS_STACKPTR int WebChildStacks[MAX_CONNECTIONS][TASK_STACK_SIZE_WEB_TASK / sizeof(int)];

// Other variables
static IP_HOOK_ON_STATE_CHANGE stateChangeHook;
static int                     faceId;
static int                     connectCnt;

// Memory pool for the Web server child tasks
static uint32_t memPool[(CHILD_ALLOC_SIZE * MAX_CONNECTIONS) / sizeof(int)];

// File system info
static const IP_FS_API *fsAPI;

/*********************************************************************
 *
 *       OnStateChange()
 *
 * Function description
 *   Callback that will be notified once the state of an interface
 *   changes.
 *
 * Parameters
 *   faceId    : Zero-based interface index.
 *   adminState: Is this interface enabled ?
 *   hwState   : Is this interface physically ready ?
 */
static void OnStateChange(unsigned faceId, uint8_t adminState, uint8_t hwState)
{
  // Check if this is a disconnect from the peer or a link down
  // In this case call IP_Disconnect() to get into a known state
  if (((adminState == IP_ADMIN_STATE_DOWN) && (hwState == 1)) ||  // Typical for dial-up connection e.g. PPP when closed from peer. Link up but app. closed
      ((adminState == IP_ADMIN_STATE_UP) && (hwState == 0)))      // Typical for any Ethernet connection e.g. PPPoE. App. opened but link down
  {
    IP_Disconnect(faceId);
  }
}

/*********************************************************************
 *
 *       OnRx()
 *
 * Parameters
 *   packet: Pointer to incoming packet.
 *   context : Context set during callback registration.
 *
 * Return value
 *   IP_RX_ERROR: Packet content is invalid for some reason.
 *   IP_OK      : Packet content is valid.
 *
 * Notes
 *   (1) Freeing pInPacket:
 *       With either return value, the IN-packet is freed by the stack
 *       and therefore can not be re-used nor has it to be freed by
 *       this routine.
 */
static int OnRx(IP_PACKET *packet, void *context)
{
  MailboxMessage_t msg;

  IP_USE_PARA(context);

  IP_Log("Packet received");

  // address returned in network uint32_t
  IP_UDP_GetSrcAddr(packet, &(msg.ip), sizeof(msg.ip));
  msg.fport  = IP_UDP_GetFPort(packet);
  msg.length = (uint32_t)IP_UDP_GetDataSize(packet);
  msg.data   = IP_UDP_GetDataPtr(packet);

  Add_to_queue(&msg);

  Set_taskEvent(EVENT_DATARECEIVED);

  return IP_OK;
}

/*********************************************************************
 *
 *       CloseSocket()
 *
 *  Function description
 *    Wrapper for closesocket()
 */
static int CloseSocket(long pConnectionInfo)
{
  int           r;
  struct linger Linger;

  // Enable linger for this socket to verify that all data is send
  Linger.l_onoff = 1;
  // Linger timeout in seconds
  Linger.l_linger = 1;
  setsockopt((long)pConnectionInfo, SOL_SOCKET, SO_LINGER, &Linger, sizeof(Linger));
  r = closesocket((long)pConnectionInfo);

  return r;
}

/*********************************************************************
 *
 *       Recv()
 *
 *  Function description
 *    Wrapper for recv()
 */
static int Recv(unsigned char *buf, int len, void *pConnectionInfo)
{
  int r;

  r = recv((long)pConnectionInfo, (char *)buf, len, 0);

  return r;
}

/*********************************************************************
 *
 *       Send()
 *
 *  Function description
 *    Wrapper for send()
 */
static int Send(const unsigned char *buf, int len, void *pConnectionInfo)
{
  int r;

  r = send((long)pConnectionInfo, (const char *)buf, len, 0);

  return r;
}

/*********************************************************************
 *
 *       WEBS_IP_API
 *
 *  Description
 *   IP related function table
 */
static const WEBS_IP_API Webs_IP_API = {Send, Recv};

/*********************************************************************
 *
 *       Alloc()
 *
 *  Function description
 *    Wrapper for Alloc(). (emNet: IP_MEMAlloc())
 */
static void *Alloc(uint32_t numBytesReq)
{
  return IP_AllocEx(memPool, numBytesReq);
}

/*********************************************************************
 *
 *       Free()
 *
 *  Function description
 *    Wrapper for Alloc(). (emNet: IP_MEMAlloc())
 */
static void Free(void *p)
{
  IP_Free(p);
}

/*********************************************************************
 *
 *       WEBS_SYS_API
 *
 *  Description
 *   System related function table
 */
static const WEBS_SYS_API Webs_SYS_API = {Alloc, Free};

/*********************************************************************
 *
 *       AddToConnectCnt
 */
static void AddToConnectCnt(int delta)
{
  OS_IncDI();
  connectCnt += delta;
  OS_DecRI();
}

/*********************************************************************
 *
 *       WebServerChildTask
 */
static void WebServerChildTask(void *context)
{
  WEBS_CONTEXT childContext;
  long         sock;
  int          Opt;
  int          r;

  sock = (long)context;
  Opt  = 1;

  setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &Opt, sizeof(Opt));
  // Initialize the context of the child task
  IP_WEBS_Init(&childContext, &Webs_IP_API, &Webs_SYS_API, fsAPI, &WebsSample_Application);

  if (connectCnt < MAX_CONNECTIONS)
  {
    r = IP_WEBS_ProcessEx(&childContext, context, NULL);
  }
  else
  {
    r = IP_WEBS_ProcessLastEx(&childContext, context, NULL);
  }

  if (r != WEBS_CONNECTION_DETACHED)
  {
    // Only close the socket if it is still in web server context and has
    // not been detached to another handler like a WebSocket handler
    CloseSocket(sock);
  }

  OS_EnterRegion();
  AddToConnectCnt(-1);
  OS_Terminate(0);
  OS_LeaveRegion();
}

/*********************************************************************
 *
 *       WebServerParentTask
 */
static void WebServerParentTask(void)
{
  struct sockaddr_in inAddr;
  uint32_t           timeout, numBytes;
  long               sockListen, sock;
  int                i, t, t0, r;
  WEBS_BUFFER_SIZES  bufferSizes;

  timeout = IDLE_TIMEOUT;

  // Assign file system
  fsAPI = &IP_FS_ReadOnly;

  // Configure buffer size.
  IP_MEMSET(&bufferSizes, 0, sizeof(bufferSizes));
  bufferSizes.NumBytesInBuf = WEBS_IN_BUFFER_SIZE;

  // Use max. MTU configured for the last interface added minus worst
  // case IPv4/TCP/VLAN headers. Calculation for the memory pool is
  // done under assumption of the best case headers with - 40 bytes.
  bufferSizes.NumBytesOutBuf      = IP_TCP_GetMTU(faceId) - 72;
  bufferSizes.NumBytesParaBuf     = WEBS_PARA_BUFFER_SIZE;
  bufferSizes.NumBytesFilenameBuf = WEBS_FILENAME_BUFFER_SIZE;
  bufferSizes.MaxRootPathLen      = WEBS_MAX_ROOT_PATH_LEN;

  // Configure the size of the buffers used by the Webserver child tasks.
  IP_WEBS_ConfigBufSizes(&bufferSizes);

  // Check memory pool size
  // Get numBytes for internals of one child thread
  numBytes = IP_WEBS_CountRequiredMem(NULL);
  // Calculate the total amount for x connections (+ some bytes for managing a memory pool)
  numBytes = (numBytes + 64) * MAX_CONNECTIONS;

  IP_Logf_Application("WEBS: Using a memory pool of %lu bytes for %lu connections.",
                      sizeof(memPool), MAX_CONNECTIONS);

  if (numBytes > sizeof(memPool))
  {
    IP_Warnf_Application("WEBS: Memory pool should be at least %lu bytes.",
                         numBytes);
  }

  // Give the stack some more memory to enable the dynamical memory allocation
  // for the Web server child tasks
  IP_AddMemory(memPool, sizeof(memPool));

  // Get a socket into listening state
  sockListen = socket(AF_INET, SOCK_STREAM, 0);
  if (sockListen == SOCKET_ERROR)
  {
    // This should never happen
    OS_Terminate(0);
  }

  IP_MEMSET(&inAddr, 0, sizeof(inAddr));
  inAddr.sin_family      = AF_INET;
  inAddr.sin_port        = htons(TCP_PORT);
  inAddr.sin_addr.s_addr = INADDR_ANY;
  bind(sockListen, (struct sockaddr *)&inAddr, sizeof(inAddr));
  listen(sockListen, BACK_LOG);

  // Loop once per client and create a thread for the actual server
  while (1)
  {
  Next:

    // Wait for an incoming connection
    sock = 0;
    if ((sock = accept(sockListen, NULL, NULL)) == SOCKET_ERROR)
    {
      continue;  // Error
    }

    // Block configuration webpage if configuration is allready set
    if (Get_isSetConf())
    {
      IP_WEBS_OnConnectionLimit(Send, Recv, (void *)sock);
      CloseSocket(sock);
      goto Next;
    }

    // Create server thread to handle connection.
    // If connection limit is reached, keep trying for some time before
    // giving up and outputting an error message
    t0 = OS_GetTime32() + 1000;

    while (1)
    {
      if (connectCnt < MAX_CONNECTIONS)
      {
        for (i = 0; i < MAX_CONNECTIONS; i++)
        {
          r = OS_IsTask(&WebChildTasks[i]);

          if (r == 0)
          {
            // Set receive timeout for the client
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            OS_CREATETASK_EX(&WebChildTasks[i], "Webserver Child",
                             WebServerChildTask, TASKPRIO_WEBSCHILD,
                             WebChildStacks[i], (void *)sock);

            AddToConnectCnt(1);
            goto Next;
          }
        }
      }

      // Check time out
      t = OS_GetTime32();
      if ((t - t0) > 0)
      {
        IP_WEBS_OnConnectionLimit(Send, Recv, (void *)sock);
        CloseSocket(sock);
        break;
      }

      OS_Delay(10);
    }
  }
}

/*********************************************************************
 *
 *       IP_InitTask
 */

void IP_InitTask(void)
{
  IP_Init();

  // Get the last registered interface ID
  faceId = IP_INFO_GetNumInterfaces() - 1;

  OS_CREATETASK(&ipTCB, "IP_Task", IP_Task, TASKPRIO_IP, ipStack);

  // Register hook to be notified on disconnects
  IP_AddStateChangeHook(&stateChangeHook, OnStateChange);

  // Register UDP callback
  IP_UDP_Open(0L /* any foreign host */, 0L, UDP_PORT, OnRx, 0);

  // Connect the interface if necessary
  IP_Connect(faceId);

  // Wait for the specified interface to be ready for usage
  while (IP_IFaceIsReadyEx(faceId) == 0)
  {
    OS_Delay(50);
  }

  // Create Webserver parent task which creates one child task per connection for the configuration webpage
  OS_CREATETASK(&WebParentTask, "Webserver Parent", WebServerParentTask, TASKPRIO_WEBSPARENT, WebParentStack);
}

/************************* end of file *************************/