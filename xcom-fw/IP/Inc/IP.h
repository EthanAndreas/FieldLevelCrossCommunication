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
File        : IP.h
Purpose     : API of the TCP/IP stack
---------------------------END-OF-HEADER------------------------------
*/

#ifndef IP_H_
#define IP_H_

#ifdef __ICCARM__  // IAR
  #pragma diag_suppress=Pa029  // No warning for unknown pragmas in earlier verions of EWARM
  #pragma diag_suppress=Pa137  // No warning for C-Style-Casts with C++
#endif

#include "SEGGER.h"            // Some segger-specific, global defines.
#include "SEGGER_UTIL.h"       // Utility functions related to multi-byte load and store.
#include "IP_ConfDefaults.h"
#include "IP_Socket.h"
#include "BSP_IP.h"            // IP_BSP_ types have been outsourced to elimate BSP_IP.c requiring IP.h .

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define IP_VERSION   34004   // Format: Mmmrr. Example: 34004 is 3.40d

/*********************************************************************
*
*       IP_MTYPE
*
*  Ids to distinguish different message types
*/
#define IP_MTYPE_INIT         (1UL << 0)
#define IP_MTYPE_CORE         (1UL << 1)
#define IP_MTYPE_ALLOC        (1UL << 2)
#define IP_MTYPE_DRIVER       (1UL << 3)
#define IP_MTYPE_ARP          (1UL << 4)
#define IP_MTYPE_IP           (1UL << 5)
#define IP_MTYPE_TCP_CLOSE    (1UL << 6)
#define IP_MTYPE_TCP_OPEN     (1UL << 7)
#define IP_MTYPE_TCP_IN       (1UL << 8)
#define IP_MTYPE_TCP_OUT      (1UL << 9)
#define IP_MTYPE_TCP_RTT      (1UL << 10)
#define IP_MTYPE_TCP_RXWIN    (1UL << 11)
#define IP_MTYPE_TCP          (IP_MTYPE_TCP_OPEN | IP_MTYPE_TCP_CLOSE | IP_MTYPE_TCP_IN | IP_MTYPE_TCP_OUT | IP_MTYPE_TCP_RTT)
#define IP_MTYPE_UDP_IN       (1UL << 12)
#define IP_MTYPE_UDP_OUT      (1UL << 13)
#define IP_MTYPE_UDP          (IP_MTYPE_UDP_IN | IP_MTYPE_UDP_OUT)
#define IP_MTYPE_LINK_CHANGE  (1UL << 14)
#define IP_MTYPE_AUTOIP       (1UL << 15)
#define IP_MTYPE_DHCP         (1UL << 17)
#define IP_MTYPE_DHCP_EXT     (1UL << 18)
#define IP_MTYPE_APPLICATION  (1UL << 19)
#define IP_MTYPE_ICMP         (1UL << 20)
#define IP_MTYPE_NET_IN       (1UL << 21)
#define IP_MTYPE_NET_OUT      (1UL << 22)
#define IP_MTYPE_NET          (IP_MTYPE_NET_IN | IP_MTYPE_NET_OUT)
#define IP_MTYPE_DNS          (1UL << 24)
#define IP_MTYPE_PPP          (1UL << 25)
#define IP_MTYPE_SOCKET_STATE (1UL << 26)
#define IP_MTYPE_SOCKET_READ  (1UL << 27)
#define IP_MTYPE_SOCKET_WRITE (1UL << 28)
#define IP_MTYPE_SOCKET       (IP_MTYPE_SOCKET_STATE | IP_MTYPE_SOCKET_READ | IP_MTYPE_SOCKET_WRITE)
#define IP_MTYPE_DNSC         (1UL << 29)
#define IP_MTYPE_ACD          (1UL << 30)

#define IP_MTYPE_IPV6         (1UL << 15)  // Temporary message type. Reuse IP_MTYPE_AUTOIP, since it is normally not used.

/*********************************************************************
*
*       IP_ERR_
*
* Error codes.
* In general, these are negative numbers.
*/

//
// Positive values are used for conditions where the result depends on
// a reply which is not checked in the function which returns the value.
//
#define     IP_ERR_SEND_PENDING     1

#define     IP_ERR_MISC            -1    // Any error, unspecified
#define     IP_ERR_TIMEDOUT        -2
#define     IP_ERR_ISCONN          -3
#define     IP_ERR_OP_NOT_SUPP     -4
#define     IP_ERR_CONN_ABORTED    -5
#define     IP_ERR_WOULD_BLOCK     -6
#define     IP_ERR_CONN_REFUSED    -7
#define     IP_ERR_CONN_RESET      -8
#define     IP_ERR_NOT_CONN        -9
#define     IP_ERR_ALREADY         -10
#define     IP_ERR_IN_VAL          -11
#define     IP_ERR_MSG_SIZE        -12
#define     IP_ERR_PIPE            -13
#define     IP_ERR_DEST_ADDR_REQ   -14
#define     IP_ERR_SHUTDOWN        -15
#define     IP_ERR_NO_PROTO_OPT    -16
#define     IP_ERR_NO_MEM          -18
#define     IP_ERR_ADDR_NOT_AVAIL  -19
#define     IP_ERR_ADDR_IN_USE     -20
#define     IP_ERR_IN_PROGRESS     -22
#define     IP_ERR_NO_BUF          -23
#define     IP_ERR_NOT_SOCK        -24
#define     IP_ERR_FAULT           -25
#define     IP_ERR_NET_UNREACH     -26
#define     IP_ERR_PARAM           -27
#define     IP_ERR_LOGIC           -28
#define     IP_ERR_NOMEM           -29   // System error
#define     IP_ERR_NOBUFFER        -30   // System error
#define     IP_ERR_RESOURCE        -31   // System error
#define     IP_ERR_BAD_STATE       -32   // System error
#define     IP_ERR_TIMEOUT         -33   // System error
#define     IP_ERR_NO_ROUTE        -36   // Net error
#define     IP_ERR_QUEUE_FULL      -37   // Typically returned when waiting for an ARP reply and no more packets can be queued for sending once the reply comes in.

#define     IP_ERR_TRIAL_LIMIT     -128  // Trial library limit reached

const char * IP_Err2Str(int x);

/*********************************************************************
*
*  Convert little/big endian - these should be efficient,
*  inline code or MACROs
*/
#if IP_IS_BIG_ENDIAN
  #define htonl(l) (l)
  #define htons(s) (s)
  #define IP_HTONL_FAST(l) (l)
#else
  #define htonl(l) IP_SwapU32(l)
  #define htons(s) ((U16)((U16)(s) >> 8) | (U16)((U16)(/*lint --e(778) */(s) & 0xFFu) << 8))  // Amazingly, some compilers really need all these U16 casts and even the masking is required to avoid a warning.
  #define IP_HTONL_FAST(v) (             \
      (((U32)((v) << 0)  >> 24) << 0)  | \
      (((U32)((v) << 8)  >> 24) << 8)  | \
      (((U32)((v) << 16) >> 24) << 16) | \
      ((v) << 24)                        \
      )
#endif

#define ntohl(l) htonl(l)
#define ntohs(s) htons(s)

#define IPV6_ADDR_LEN                (16)
#define IPV4_ADDR_LEN                 (4)


/*********************************************************************
*
*  Helper MACROs
*/
#define IP_BYTES2ADDR(a, b, c, d)  (        \
          ((((U32)a << 24) >> 24) << 24) |  \
          ((((U32)b << 24) >> 24) << 16) |  \
          ((((U32)c << 24) >> 24) << 8 ) |  \
          ((((U32)d << 24) >> 24) << 0 )    \
        )

/*********************************************************************
*
*       IP_DLIST
*
*  Some very old compilers like the NEC 78K0R seem to have a problem
*  with using structs from other files if these are included after
*  using a pointer to the structure. The typical two-step declaration
*  and using it as a pointer does not work if the struct itself comes
*  from another file. As the struct IP_DLIST_ITEM is simple enough
*  and does not include any internals we keep it here. This allows
*  us to keep the structures IP_RAW_CONNECTION and IP_UDP_CONNECTION
*  in this file as well which is the real reason for the compiler
*  problem.
*/
typedef struct IP_DLIST_ITEM_STRUCT IP_DLIST_ITEM;
struct IP_DLIST_ITEM_STRUCT {
  IP_DLIST_ITEM* pNext;
  IP_DLIST_ITEM* pPrev;
};

#if IP_ALLOW_DEINIT
/*********************************************************************
*
*       IP_ON_EXIT_CB
*
*  Typically only used by the stack internally. Made public only for
*  usage in IP_ConfigIO.c to be able to use embOWView via UDP for
*  log messages.
*/
typedef struct IP_ON_EXIT_CB_STRUCT IP_ON_EXIT_CB;
struct IP_ON_EXIT_CB_STRUCT {
  IP_ON_EXIT_CB* pNext;
  void (*pfOnExit)(void);
};

void IP_AddOnExitHandler(IP_ON_EXIT_CB* pCB, void (*pfOnExit)(void));
#endif

/*********************************************************************
*
*       Delayed execution
*
*  Delayed execution is an internal API workaround to be able to
*  execute chosen API from an interrupt context that otherwise could
*  not be executed due to IP_OS_LOCK() being unable to work from
*  interrupt due to the underlying semaphore used.
*  Delayed execution means that API to execute will be queued to be
*  executed the next time executing IP_Task/IP_Exec. The IP task will
*  get signaled whenever a command is added to the queue, making sure
*  that it executes right after interrupt has finished.
*/
typedef struct IP_EXEC_DELAYED_STRUCT IP_EXEC_DELAYED;
struct IP_EXEC_DELAYED_STRUCT {
  IP_EXEC_DELAYED* pNext;
  void (*pfExec)(const void* pPara);
  void*            pPara;
  void*            pContext;
  void (*pfDoneCB)(IP_EXEC_DELAYED* pED, void* pContext);
};

//
// Use if delayed execution call has 2 parameters.
//
typedef struct {
  PTR_ADDR Para0;
  PTR_ADDR Para1;
} IP_EXEC_DELAYED_PARA_2;

//
// Use if delayed execution call has 3 parameters.
//
typedef struct {
  PTR_ADDR Para0;
  PTR_ADDR Para1;
  PTR_ADDR Para2;
} IP_EXEC_DELAYED_PARA_3;

//
// Use if delayed execution call has 4 parameters.
//
typedef struct {
  PTR_ADDR Para0;
  PTR_ADDR Para1;
  PTR_ADDR Para2;
  PTR_ADDR Para3;
} IP_EXEC_DELAYED_PARA_4;

void IP_ExecDelayed(IP_EXEC_DELAYED* pED, void (*pfExec)(const void* pPara), void* pPara, void* pContext, void (*pfDoneCB)(IP_EXEC_DELAYED* pED, void* pContext));

//
// Available delayed API calls.
//
void IP_DHCPS_ConfigDNSAddr_Delayed(const void* pPara);
void IP_DHCPS_ConfigPool_Delayed   (const void* pPara);
void IP_DHCPS_Halt_Delayed         (const void* pPara);
void IP_DHCPS_Start_Delayed        (const void* pPara);

void IP_DNS_SERVER_Start_Delayed   (const void* pPara);
void IP_DNS_SERVER_Stop_Delayed    (const void* pPara);
void IP_MDNS_SERVER_Start_Delayed  (const void* pPara);
void IP_MDNS_SERVER_Stop_Delayed   (const void* pPara);

void IP_SetAddrMaskEx_Delayed      (const void* pPara);

/*********************************************************************
*
*       Internal API
*
*  Internal API that is made public so glue layers like the OS layer
*  can be shipped in source without the need for internal header
*  files when using object shipments.
*/
void IP_SignalIfPacketFreeUsed(void);

/*********************************************************************
*
*  IP_BSP_
*/
void IP_BSP_SetAPI(unsigned IFaceId, const BSP_IP_API* pAPI);

/*********************************************************************
*
*  IP_OS_
*/
      void   IP_OS_Delay           (unsigned ms);
      void   IP_OS_DisableInterrupt(void);
      void   IP_OS_EnableInterrupt (void);
      void   IP_OS_Init            (void);
      void   IP_OS_Unlock          (void);
      void   IP_OS_AssertLock      (void);
      void   IP_OS_Lock            (void);
      U32    IP_OS_GetTime32       (void);
      void   IP_OS_AddTickHook     (void (*pfHook)(void));
const char * IP_OS_GetTaskName     (void *pTask);
//
// Wait and signal for Net task
//
      void   IP_OS_WaitNetEvent    (unsigned ms);
      void   IP_OS_SignalNetEvent  (void);
//
// Wait and signal for the optional Rx task
//
      void   IP_OS_WaitRxEvent     (void);
      void   IP_OS_SignalRxEvent   (void);
//
// Wait and signal for the optional WiFi task
//
      void   IP_OS_WaitWiFiEventTimed(unsigned Timeout);
      void   IP_OS_SignalWiFiEvent   (void);
//
// Wait and signal for application tasks
//
      void   IP_OS_WaitItem        (void *pWaitItem);
      void   IP_OS_WaitItemTimed   (void *pWaitItem, unsigned Timeout);
      void   IP_OS_SignalItem      (void *pWaitItem);

/*********************************************************************
*
*       IP_PACKET
*
*/
#define IP_PACKET_FLAG_PTP_MASK          (U8)(1u << 0)
#define IP_PACKET_FLAG_ZERO_SRC_IP_MASK  (U8)(1u << 1)  // Use a zero source IP in IP header. For the moment this only applies to IPv4. This is to fulfill the DHCP RFC 2131 4.1 .

typedef   U32 ip_addr;
typedef   U32 IP_ADDR;

typedef struct {
  U16 PortSrc;
  U16 PortDest;
  U32 SeqNo;
  U32 AckNo;
  U8 HeaderLen;
  U8 Flags;
  U16 WindowSize;
  U16 CheckSum;
} IP_TCP4;

typedef struct {
  U16 Dummy;
  U8  aMACDest[6];
  U8  aMACSrc [6];
  U16 Type;
  // IP
  U8  VersionHeaderLen;
  U8  Service;
  U16 Len;
  U16 PacketId;
  U16 FragmentOff;
  U8  TTL;
  U8  Proto;
  U16 HeaderChecksum;
  U32 IPSrc;
  U32 IPDest;
  IP_TCP4 Tcp;
} IP_ETH_IP4_TCP;

typedef struct {
  U8  DefaultTTL;
  U8  LocalMcTTL;
  U8  GlobalMcTTL;
} IP_TTL_CONFIG;

typedef struct IP_PACKET_STRUCT IP_PACKET;
struct IP_PACKET_STRUCT {
  IP_PACKET*        pNext;
  struct IFace*     pIFace;            // The interface (net) it came in on
  U8*               pBuffer;           // Beginning of raw buffer
  U8*               pData;             // Beginning of protocol/data. This is always >= pBuffer.
  IP_TTL_CONFIG*    pTTLConfig;        // Overrides the default TTL values of the system. If NULL the configured defaults are used.
#if IP_SUPPORT_ON_PACKET_FREE_CB
  void (*pfOnFreeCB)(IP_PACKET* pPacket, void* pContext);
  void*             pOnFreeContext;
#endif
  U16               NumBytes;          // Number of bytes in buffer
  U16               BufferSize;        // Length of raw buffer
  I16               UseCnt;            // Use count, for cloning buffer
  U8                ToS;               // Type of Service (QoS) byte in IPv4 header. Reused as DSCP byte for IPv4/IPv6 in newer implementations.
  U8                Flag;              // Bitmap for various indications (PTP request, ...)
#if (defined(U64) && ((IP_SUPPORT_PACKET_TIMESTAMP != 0) || (IP_SUPPORT_PTP != 0)))
  U64               OSTimestamp_ns;    // Time stamp (microseconds) based on OS time.
#endif
#if IP_SUPPORT_PTP
  U32               HWTimestampSec;    // Seconds read from PTP HW clock.
  U32               HWTimestampNSec;   // Nanoseconds read from PTP HW clock.
#endif
#if ((IP_SUPPORT_FRAGMENTATION_IPV4 != 0) || (IP_SUPPORT_FRAGMENTATION_IPV6 != 0))
  IP_PACKET*        pNextFrag;
#endif
#if IP_DEBUG != 0
  IP_PACKET*        pNextPacketInUse;
#endif
};

/*********************************************************************
*
*       PTP driver
*/
typedef struct IP_PTP_DRIVER_STRUCT IP_PTP_DRIVER;
struct IP_PTP_DRIVER_STRUCT {
  int  (*pfAttach)     (struct IFace* pIFace, const IP_PTP_DRIVER* pPTPDriver);
  int  (*pfInit)       (unsigned IFaceId, void* pContext);
  void (*pfCorrectTime)(unsigned IFaceId, U32 Flags, U32 Sec, U32 NSec);  // Callback used by IP_PTP to give a time correction to the driver.
  int  (*pfGetTime)    (unsigned IFaceId, U32 *pSec, U32 *pNSec);         // Callback used by IP_PTP to get the precise time from the driver.
  int  (*pfControl)    (unsigned IFaceId, int Cmd, void* p);
};

/*********************************************************************
*
*       NI driver
*/
typedef struct {
  int  (*pfInit)             (unsigned IFaceId);
  int  (*pfSendPacket)       (unsigned IFaceId);                                 // Sends a packet on this interface. What exactly the driver does depends very much on the hardware.
                                                                                 // Typically it does nothing if it is already sending and starts sending the first packet in the fifo if transmitter is idle.
                                                                                 // Return value: 0: OK, < 0 Error
                                                                                 // Packet is owned by driver from this point on.
  int  (*pfGetPacketSize)    (unsigned IFaceId);                                 // Return the number of bytes in next packet, <= 0 if there is no more packet.
  int  (*pfReadPacket)       (unsigned IFaceId, U8 * pDest, unsigned NumBytes);  // Read (if pDest is valid) and discard packet.
  void (*pfTimer)            (unsigned IFaceId);                                 // Routine is called periodically
  int  (*pfControl)          (unsigned IFaceId, int Cmd, void * p);              // Various control functions
  void (*pfEnDisableRxInt)   (unsigned IFaceId, unsigned OnOff);                 // Masks or unmasks Rx Interrupt. OnOff = 1: Enable, OnOff = 0: Disable
#if IP_SUPPORT_PTP
  int  (*pfGetRxTimestamp)   (unsigned IFaceId, U32* pSec, U32* pNSec);          // Callback used by the stack to get the timestamp of the received packet.
#endif
} IP_HW_DRIVER;

/*********************************************************************
*
*       IP_WIFI_...
*
*  WiFi functions for WiFi interfaces
*/
typedef struct {
  const char* sSSID;
        I32   Rssi;  // Receive Signal Strength Indicator. Typically given as -<value>. Higher (towards 0) means better.
        U16   BeaconInterval;
        U8    abBSSID[6];
        U8    Channel;
        U8    Security;
        U8    Mode;  // IP_WIFI_MODE_UNKNOWN or IP_WIFI_MODE_INFRASTRUCTURE or IP_WIFI_MODE_ADHOC .
} IP_WIFI_SCAN_RESULT;

typedef struct {
  U8 acKey[13];  // 5 (64 bit) or 13 (128 bit) byte long WEP key.
  U8 Len;        // Length of the WEP key, 5 or 13 bytes.
  U8 Index;      // Unique key index 0..3 .
} IP_WIFI_WEP_KEY;

typedef struct {
  const char*            sSSID;
  const char*            sEnterpriseUser;    // WPA/WPA2 enterprise username.
  const char*            sWPAPass;           // WPA/WPA2 enterprise/PSK passphrase.
  const IP_WIFI_WEP_KEY* paWEPKey;
  U8                     abBSSID[6];         // HW address of Access Point.
  U8                     NumWEPKeys;         // Number of WEP keys configured.
  U8                     WEPActiveKeyIndex;  // 0..3: Index of WEP key to be used for sending, typically index 0 .
  U8                     Mode;               // IP_WIFI_MODE_INFRASTRUCTURE or IP_WIFI_MODE_ADHOC .
  U8                     Security;
  U8                     Channel;
} IP_WIFI_CONNECT_PARAMS;

typedef struct {
  U8 abBSSID[6];
  U8 Channel;
} IP_WIFI_ASSOCIATE_INFO;

typedef struct {
  U8 Domain24GHz;
  U8 Domain5GHzMask;
} IP_WIFI_DOMAIN_CONFIG;

typedef struct {
  I32 Rssi;
} IP_WIFI_SIGNAL_INFO;

typedef struct {
  U8* pMacAddress;  // Pointer is not valid after the hook call.
} IP_WIFI_CLIENT_INFO;

typedef void (*IP_WIFI_pfScanResult)          (unsigned IFaceId, const IP_WIFI_SCAN_RESULT* pResult, int Status);
typedef void (*IP_WIFI_pfOnAssociateChange)   (unsigned IFaceId, const IP_WIFI_ASSOCIATE_INFO* pInfo, U8 State);
typedef void (*IP_WIFI_pfOnSignalChange)      (unsigned IFaceId, IP_WIFI_SIGNAL_INFO* pInfo);
typedef void (*IP_WIFI_pfOnIsrExecDone)       (unsigned IFaceId);

#define IP_WIFI_CLIENT_DISCONNECT  (0u)
#define IP_WIFI_CLIENT_CONNECT     (1u)
typedef void (*IP_WIFI_pfOnClientNotification)(unsigned IFaceId, const IP_WIFI_CLIENT_INFO* pInfo, unsigned IsConnected);

typedef struct IP_HOOK_ON_WIFI_ASSOCIATE_CHANGE {
  struct IP_HOOK_ON_WIFI_ASSOCIATE_CHANGE* pNext;  // Pointer to the next hook.
         IP_WIFI_pfOnAssociateChange       pf;     // Pointer to the function to be called from the hook.
} IP_HOOK_ON_WIFI_ASSOCIATE_CHANGE;

typedef struct IP_HOOK_ON_WIFI_SIGNAL_CHANGE {
  struct IP_HOOK_ON_WIFI_SIGNAL_CHANGE* pNext;     // Pointer to the next hook.
         IP_WIFI_pfOnSignalChange       pf;        // Pointer to the function to be called from the hook.
} IP_HOOK_ON_WIFI_SIGNAL_CHANGE;

typedef struct IP_HOOK_ON_WIFI_ISR_EXEC_DONE_STRUCT IP_HOOK_ON_WIFI_ISR_EXEC_DONE;
struct IP_HOOK_ON_WIFI_ISR_EXEC_DONE_STRUCT {
  IP_HOOK_ON_WIFI_ISR_EXEC_DONE* pNext;    // Pointer to the next hook.
  IP_WIFI_pfOnIsrExecDone        pf;       // Callback to execute once the WiFi ISR exec has been executed.
  unsigned                       IFaceId;  // Zero-based interface index for which the callback will be executed.
};

typedef struct IP_HOOK_ON_WIFI_CLIENT_NOTIFICATION_STRUCT IP_HOOK_ON_WIFI_CLIENT_NOTIFICATION;
struct IP_HOOK_ON_WIFI_CLIENT_NOTIFICATION_STRUCT {
  IP_HOOK_ON_WIFI_CLIENT_NOTIFICATION* pNext;
  IP_WIFI_pfOnClientNotification       pf;
};

#define IP_WIFI_MODE_UNKNOWN         0
#define IP_WIFI_MODE_INFRASTRUCTURE  1
#define IP_WIFI_MODE_ADHOC           2
#define IP_WIFI_MODE_ACCESS_POINT    3

#define IP_WIFI_CHANNEL_ALL          0

//
// 2.4 GHz channels by regulation (one at a time can be used) together with mutliple 5 GHz channel defines.
//
#define IP_WIFI_24GHZ_FCC_WORLD  0x00  // Set to 0 to be the default.
#define IP_WIFI_24GHZ_NONE       0x01
#define IP_WIFI_24GHZ_ETSI       0x02
#define IP_WIFI_24GHZ_TELEC      0x03

//
// 5 GHz channels as defined by U-NII (can be orr-ed to use a wider channel selection).
// Can be orr-ed with 2.4 GHz channel setup.
//
#define IP_WIFI_5GHZ_UNII1_MASK   0x10
#define IP_WIFI_5GHZ_UNII2_MASK   0x20
#define IP_WIFI_5GHZ_UNII2E_MASK  0x40
#define IP_WIFI_5GHZ_UNII3_MASK   0x80

#define IP_WIFI_SECURITY_OPEN                       (0)
#define IP_WIFI_SECURITY_WEP_OPEN                   (1)
#define IP_WIFI_SECURITY_WEP_SHARED                 (2)
#define IP_WIFI_SECURITY_WPA_TKIP                   (3)
#define IP_WIFI_SECURITY_WPA_AES                    (4)
#define IP_WIFI_SECURITY_WPA2_AES                   (5)
#define IP_WIFI_SECURITY_WPA_WPA2_MIXED             (6)                              //lint !e621 Ignore identifier clash for 31 unique chars in name.
#define IP_WIFI_SECURITY_WPA_ENTERPRISE             (7)
#define IP_WIFI_SECURITY_WPA2_ENTERPRISE            IP_WIFI_SECURITY_WPA_ENTERPRISE  //                                                                All supported modules use WPA/WPA2 combined, so everything is the same.
#define IP_WIFI_SECURITY_WPA_WPA2_MIXED_ENTERPRISE  IP_WIFI_SECURITY_WPA_ENTERPRISE  //lint !e621 Ignore identifier clash for 31 unique chars in name. All supported modules use WPA/WPA2 combined, so everything is the same.

      void  IP_WIFI_AddAssociateChangeHook     (IP_HOOK_ON_WIFI_ASSOCIATE_CHANGE* pHook, IP_WIFI_pfOnAssociateChange pf);
      void  IP_WIFI_AddClientNotificationHook  (IP_HOOK_ON_WIFI_CLIENT_NOTIFICATION* pHook, IP_WIFI_pfOnClientNotification pf);
      int   IP_WIFI_AddInterface               (const IP_HW_DRIVER* pDriver);
      void  IP_WIFI_AddIsrExecDoneHook         (unsigned IFaceId, IP_HOOK_ON_WIFI_ISR_EXEC_DONE* pHook, IP_WIFI_pfOnIsrExecDone pf);
      void  IP_WIFI_AddSignalChangeHook        (IP_HOOK_ON_WIFI_SIGNAL_CHANGE* pHook, IP_WIFI_pfOnSignalChange pf);
      int   IP_WIFI_Connect                    (unsigned IFaceId, const IP_WIFI_CONNECT_PARAMS* pParams, U32 Timeout);
      int   IP_WIFI_ConfigAllowedChannels      (unsigned IFaceId, const U8* paChannel, U8 NumChannels);
      int   IP_WIFI_ConfigRegDomain            (unsigned IFaceId, const IP_WIFI_DOMAIN_CONFIG* pDomainConfig);
      int   IP_WIFI_Disconnect                 (unsigned IFaceId, U32 Timeout);
      void  IP_WIFI_IsrExec                    (unsigned IFaceId);
      void  IP_WIFI_IsrTask                    (void);
      int   IP_WIFI_Scan                       (unsigned IFaceId, U32 Timeout, IP_WIFI_pfScanResult pf, const char* sSSID, U8 Channel);
const char* IP_WIFI_Security2String            (U8 Security);
      void  IP_WIFI_ConfigIsrTaskAlwaysSignaled(unsigned IFaceId, char OnOff);
      void  IP_WIFI_ConfigIsrTaskTimeout       (unsigned IFaceId, U32 ms);
      void  IP_WIFI_SignalIsrTask              (unsigned IFaceId);

//
// Prototypes that make our life easier providing generic samples
// for WiFi callbacks such as connect/disconnect used in the
// configuration files.
//
int  WIFI_CB_Connect             (unsigned IFaceId);
int  WIFI_CB_Connect_StartAP     (unsigned IFaceId);
int  WIFI_CB_Disconnect          (unsigned IFaceId);
void WIFI_CB_OnAssociateChange   (unsigned IFaceId, const IP_WIFI_ASSOCIATE_INFO* pInfo, U8 State);
void WIFI_CB_OnLinkChange        (unsigned IFaceId, U32 Duplex, U32 Speed);
void WIFI_CB_OnSignalChange      (unsigned IFaceId, IP_WIFI_SIGNAL_INFO* pInfo);
void WIFI_CB_OnClientNotification(unsigned IFaceId, const IP_WIFI_CLIENT_INFO* pInfo, unsigned IsConnected);

/*********************************************************************
*
*       Driver capabilities
*/
#define IP_NI_CAPS_WRITE_IP_CHKSUM     (1u << 0)    // Driver capable of inserting the IP-checksum into an outgoing packet ?
#define IP_NI_CAPS_WRITE_UDP_CHKSUM    (1u << 1)    // Driver capable of inserting the UDP-checksum into an outgoing packet ?
#define IP_NI_CAPS_WRITE_TCP_CHKSUM    (1u << 2)    // Driver capable of inserting the TCP-checksum into an outgoing packet ?
#define IP_NI_CAPS_WRITE_ICMP_CHKSUM   (1u << 3)    // Driver capable of inserting the ICMP-checksum into an outgoing packet ?
#define IP_NI_CAPS_CHECK_IP_CHKSUM     (1u << 4)    // Driver capable of computing and comparing the IP-checksum of an incoming packet ?
#define IP_NI_CAPS_CHECK_UDP_CHKSUM    (1u << 5)    // Driver capable of computing and comparing the UDP-checksum of an incoming packet ?
#define IP_NI_CAPS_CHECK_TCP_CHKSUM    (1u << 6)    // Driver capable of computing and comparing the TCP-checksum of an incoming packet ?
#define IP_NI_CAPS_CHECK_ICMP_CHKSUM   (1u << 7)    // Driver capable of computing and comparing the ICMP-checksum of an incoming packet ?
#define IP_NI_CAPS_WRITE_ICMPV6_CHKSUM (1u << 8)    // Driver capable of inserting the ICMP-checksum into an outgoing packet ?

/*********************************************************************
*
*       PHY configuration
*/
#define IP_PHY_MODE_MII       0u
#define IP_PHY_MODE_RMII      1u

#define IP_PHY_ADDR_ANY       0xFF                          // IP_PHY_ADDR_ANY is used as PHY addr to initiate automatic scan for PHY
#define IP_PHY_ADDR_INTERNAL  0xFE                          // IP_PHY_ADDR_INTERNAL is used as PHY addr to select internal PHY

#define IP_ADMIN_STATE_DOWN   0
#define IP_ADMIN_STATE_UP     1

/*********************************************************************
*
*       Ethernet PHY
*/
typedef struct IP_PHY_CONTEXT  IP_PHY_CONTEXT;

typedef struct {
  unsigned (*pfRead) (IP_PHY_CONTEXT* pContext, unsigned RegIndex);
  void     (*pfWrite)(IP_PHY_CONTEXT* pContext, unsigned RegIndex, unsigned  val);
} IP_PHY_ACCESS;

#define IP_PHY_MODE_10_HALF    ((U16)1u <<  5)  // Real bit in register.
#define IP_PHY_MODE_10_FULL    ((U16)1u <<  6)  // Real bit in register.
#define IP_PHY_MODE_100_HALF   ((U16)1u <<  7)  // Real bit in register.
#define IP_PHY_MODE_100_FULL   ((U16)1u <<  8)  // Real bit in register.
#define IP_PHY_MODE_1000_HALF  ((U16)1u <<  9)  // Pseudo bit. Bit is not consecutive to the rest as other registers and bits are used.
#define IP_PHY_MODE_1000_FULL  ((U16)1u << 10)  // Pseudo bit. Bit is not consecutive to the rest as other registers and bits are used.

struct IP_PHY_CONTEXT {
  const IP_PHY_ACCESS *pAccess;
        void          *pContext;        // Context needed for low level functions
        U32            Speed;
        U32            SpeedStableCnt;
        U16            SupportedModes;
        U16            Anar;            // Value written to ANAR (Auto-negotiation Advertisement register)
        U16            Anar1000;        // Value written to 1000BASE-T control register (Auto-negotiation Advertisement register for 1000BASE-T)
        U16            Bmcr;            // Value written to BMCR (basic mode control register)
        U8             Addr;
        U8             UseRMII;         // 0: MII, 1: RMII
        U8             LastLinkChange;  // Direction of last link change. 0: No link change yet, 1: Link changed to up, 2: Link changed to down
        U8             SupportGmii;     // On PHY init we detect if the PHY supports Gigabit Ethernet and save the result here. If 0 on PHY init disables Gigabit Ethernet support.
        U8             IFaceId;
};

typedef void (*IP_PHY_pfConfig)(unsigned IFaceId);

typedef struct {
  const U8* pAddrList;
        U8  NumAddr;
} IP_PHY_ALT_LINK_STATE_ADDR;

typedef struct {
               IP_PHY_pfConfig             pfConfig;
  const        IP_PHY_ALT_LINK_STATE_ADDR* pAltPhyAddr;
  const struct IP_PHY_HW_DRIVER*           pDriver;
  const        void*                       pAccess;
               void*                       pContext;         // Context that is used to attach the driver specific context to.
               void*                       pUserContext;     // User/internal specified context. Also used by some drivers to pass a context different callbacks in the driver.
               U32                         Speed;
               U32                         SpeedStableCnt;
               U32                         LastSavedSpeed;   // Disables periodic PHY access from the stack for link checks if set.
               U32                         LastSavedDuplex;  // Disables periodic PHY access from the stack for link checks if set.
               U8                          InitDone;
               U8                          LastLinkChange;   // Direction of last link change. 0: No link change yet, 1: Link changed to up, 2: Link changed to down.
               U8                          DoNotUseStaticFilters;
               U8                          DisableChecks;
} IP_PHY_CONTEXT_EX;

typedef struct IP_PHY_HW_DRIVER {
  int (*pfAttachContext)(unsigned IFaceId, IP_PHY_CONTEXT_EX* pContext);
  int (*pfInit)         (unsigned IFaceId, IP_PHY_CONTEXT_EX* pContext);
  int (*pfGetLinkState) (unsigned IFaceId, IP_PHY_CONTEXT_EX* pContext, U32* pDuplex, U32* pSpeed);
  int (*pfControl)      (unsigned IFaceId, IP_PHY_CONTEXT_EX* pContext, unsigned Cmd, void* p);  // Control function for various jobs.
} IP_PHY_HW_DRIVER;

typedef struct {
  int (*pfInit)        (IP_PHY_CONTEXT *pContext);
  int (*pfGetLinkState)(IP_PHY_CONTEXT *pContext, U32 *pDuplex, U32 *pSpeed);
} IP_PHY_DRIVER;

typedef struct {
  unsigned (*pfRead) (void *pContext, unsigned RegIndex);
  void     (*pfWrite)(void *pContext, unsigned RegIndex, unsigned Data);
} IP_PHY_API;

typedef void (*IP_NI_pfOnPhyReset)(unsigned IFaceId, void *pContext, const IP_PHY_API *pApi);

typedef struct IP_HOOK_ON_PHY_RESET {
         IP_NI_pfOnPhyReset    pf;       // Pointer to the function to be called from the hook.
  struct IP_HOOK_ON_PHY_RESET *pNext;    // Pointer to the next hook function.
} IP_HOOK_ON_PHY_RESET;

void IP_PHY_AddDriver             (unsigned IFaceId, const IP_PHY_HW_DRIVER* pDriver, const void* pAccess, IP_PHY_pfConfig pf);
int  IP_PHY_AddPTPDriver          (unsigned IFaceId, const IP_PTP_DRIVER* pPTPDriver, U32 Clock);
void IP_PHY_AddResetHook          (IP_HOOK_ON_PHY_RESET *pHook, IP_NI_pfOnPhyReset pf);
void IP_PHY_ConfigAddr            (unsigned IFaceId, unsigned Addr);
void IP_PHY_ConfigAltAddr         (unsigned IFaceId, const IP_PHY_ALT_LINK_STATE_ADDR* pAltPhyAddr);
void IP_PHY_ConfigGigabitSupport  (unsigned IFaceId, unsigned OnOff);
void IP_PHY_ConfigSupportedModes  (unsigned IFaceId, unsigned Modes);
void IP_PHY_ConfigUseStaticFilters(unsigned IFaceId, unsigned OnOff);
void IP_PHY_DisableCheck          (U32 Mask);
void IP_PHY_DisableCheckEx        (unsigned IFaceId, U32 Mask);
void IP_PHY_ReInit                (unsigned IFaceId);
void IP_PHY_SetWdTimeout          (int ShiftCnt);

#define IP_PHY_GENERIC_AddResetHook  IP_PHY_AddResetHook  // Compatibility macro for old API
void IP_PHY_GENERIC_RemapAccess   (unsigned IFaceId, unsigned AccessIFaceId);

extern const IP_PHY_DRIVER    IP_PHY_Generic;         // Generic PHY driver (legacy).
extern const IP_PHY_HW_DRIVER IP_PHY_Driver_Generic;  // Generic PHY driver.
extern const IP_PHY_HW_DRIVER IP_PHY_Driver_MARVELL_88E1111_Fiber;

#define PHY_DISABLE_CHECK_ID                   (1uL << 0)
#define PHY_DISABLE_CHECK_LINK_STATE_AFTER_UP  (1uL << 1)
#define PHY_DISABLE_WATCHDOG                   (1uL << 2)

/*********************************************************************
*
*       Network interface configuration and handling
*/
void IP_NI_ClrBPressure             (unsigned IFaceId);
int  IP_NI_ConfigLinkCheckMultiplier(unsigned IFaceId, unsigned Multiplier);
void IP_NI_ConfigPHYAddr            (unsigned IFaceId, U8 Addr);                                // Configure PHY Addr (5-bit)
void IP_NI_ConfigPHYMode            (unsigned IFaceId, U8 Mode);                                // Configure PHY Mode: 0: MII, 1: RMII
void IP_NI_ConfigUsePromiscuousMode (unsigned IFaceId, unsigned OnOff);
void IP_NI_ConfigPoll               (unsigned IFaceId);
void IP_NI_ForceCaps                (unsigned IFaceId, U8 CapsForcedMask, U8 CapsForcedValue);  // Allows to forcibly set the checksum capabilities of the hardware
int  IP_NI_GetAdminState            (unsigned IFaceId);
int  IP_NI_GetIFaceType             (unsigned IFaceId, char* pBuffer, U32* pNumBytes);
int  IP_NI_GetState                 (unsigned IFaceId);
int  IP_NI_GetTxQueueLen            (unsigned IFaceId);
void IP_NI_SetAdminState            (unsigned IFaceId, int AdminState);
void IP_NI_SetBPressure             (unsigned IFaceId);
int  IP_NI_SetTxBufferSize          (unsigned IFaceId, unsigned NumBytes);
int  IP_NI_AddPTPDriver             (unsigned IFaceId, const IP_PTP_DRIVER* pPTPDriver, U32 CoreClock);
int  IP_NI_PauseRx                  (unsigned IFaceId, U32 Pause);
int  IP_NI_PauseRxInt               (unsigned IFaceId, U32 Pause);

/*********************************************************************
*
*       IP stack tasks
*/
void IP_Task       (void);
void IP_RxTask     (void);
void IP_ShellServer(void);

typedef int  (IP_RX_HOOK)     (IP_PACKET *pPacket);
typedef void IP_ON_RX_FUNC    (IP_PACKET *pPacket);

#define IP_TASK_REQ_SHUTDOWN_MASK  (1u << 0)
#define IP_TASK_LEAVE_LOOP_MASK    (1u << 1)
#define IP_TASK_IP_MASK            (1u << 2)
#define IP_TASK_RX_MASK            (1u << 3)
#define IP_TASK_WIFI_ISR_MASK      (1u << 4)

/*********************************************************************
*
*       IP hooks
*/
typedef struct IP_HOOK_AFTER_CONFIG {
  int    (*pf)(void);                                         // Pointer to the function to be called from the hook.
  struct IP_HOOK_AFTER_CONFIG *pNext;                         // Pointer to the next hook function.
} IP_HOOK_AFTER_CONFIG;

typedef struct IP_HOOK_AFTER_INIT {
  void   (*pf)(void);                                         // Pointer to the function to be called from the hook.
  struct IP_HOOK_AFTER_INIT *pNext;                           // Pointer to the next hook function.
} IP_HOOK_AFTER_INIT;

typedef struct IP_HOOK_ON_STATE_CHANGE {
  void   (*pf)(unsigned IFaceId, U8 AdminState, U8 HWState);  // Pointer to the function to be called from the hook.
  struct IP_HOOK_ON_STATE_CHANGE *pNext;                      // Pointer to the next hook function.
} IP_HOOK_ON_STATE_CHANGE;

typedef struct IP_HOOK_ON_ETH_TYPE {
  int (*pf)(unsigned IFaceId, IP_PACKET* pPacket, void* pBuffer, U32 NumBytes);  // Pointer to the function to be called from the hook.
  struct IP_HOOK_ON_ETH_TYPE* pNext;                                             // Pointer to the next hook.
  U16 Type;                                                                      // Ethernet type that triggers the callback.
} IP_HOOK_ON_ETH_TYPE;

typedef struct IP_HOOK_ON_PACKET_FREE {
  void (*pf)(IP_PACKET* pPacket);        // Pointer to the function to be called from the hook.
  struct IP_HOOK_ON_PACKET_FREE* pNext;  // Pointer to the next hook.
} IP_HOOK_ON_PACKET_FREE;

typedef struct IP_HOOK_ON_LINK_CHANGE_STRUCT IP_HOOK_ON_LINK_CHANGE;
struct IP_HOOK_ON_LINK_CHANGE_STRUCT {
  void (*pf)(unsigned IFaceId, U32 Duplex, U32 Speed);  // Pointer to the function to be called from the hook.
  IP_HOOK_ON_LINK_CHANGE* pNext;                        // Pointer to the next hook.
};

typedef struct IP_HOOK_ON_RX_FIFO_ADD_STRUCT IP_HOOK_ON_RX_FIFO_ADD;
struct IP_HOOK_ON_RX_FIFO_ADD_STRUCT {
  IP_HOOK_ON_RX_FIFO_ADD *pNext;
  int (*pf)(IP_PACKET* pPacket, int AllowNoLock, int InInt);
};

typedef struct IP_HOOK_ON_HW_ADDR_CHANGE_STRUCT IP_HOOK_ON_HW_ADDR_CHANGE;
struct IP_HOOK_ON_HW_ADDR_CHANGE_STRUCT {
  void (*pf)(unsigned IFace, const U8* pAddr, unsigned Len);  // Pointer to the function to be called from the hook.
  IP_HOOK_ON_HW_ADDR_CHANGE* pNext;                             // Pointer to the next hook.
};

/*********************************************************************
*
*       IP_CACHE_...
*
*  Cache related functions
*/
void IP_CACHE_SetConfig(const SEGGER_CACHE_CONFIG *pConfig, unsigned ConfSize);

/*********************************************************************
*
*       Core functions
*/
#define IP_ConfTCPSpace  IP_ConfigTCPSpace  // Compatibility macro for old API

      void       IP_AddAfterConfigHook          (IP_HOOK_AFTER_CONFIG* pHook, int  (*pf)(void));
      void       IP_AddAfterInitHook            (IP_HOOK_AFTER_INIT*   pHook, void (*pf)(void));
      void       IP_AddBuffers                  (int NumBuffers, int BytesPerBuffer);
      int        IP_AddEtherInterface           (const IP_HW_DRIVER *pDriver);
      void       IP_AddHWAddrChangeHook         (IP_HOOK_ON_HW_ADDR_CHANGE* pHook, void (*pf)(unsigned IFace, const U8* pAddr, unsigned Len));
      void       IP_AddLinkChangeHook           (IP_HOOK_ON_LINK_CHANGE* pHook, void (*pf)(unsigned IFaceId, U32 Duplex, U32 Speed));
      int        IP_AddLoopbackInterface        (void);
      void       IP_AddMemory                   (U32* pMem, U32 NumBytes);
      void       IP_AddStateChangeHook          (IP_HOOK_ON_STATE_CHANGE *pHook, void (*pf)(unsigned IFaceId, U8 AdminState, U8 HWState));
      int        IP_AddVirtEtherInterface       (unsigned HWIFaceId);
      void*      IP_Alloc                       (U32 NumBytesReq);
      void*      IP_AllocEx                     (U32* pBaseAddr, U32 NumBytesReq);
      void       IP_Free                        (void *p);
      void       IP_AllowBackPressure           (char v);
      void       IP_AssignMemory                (U32 *pMem, U32 NumBytes);
      void       IP_ConfigDoNotAddLowLevelChecks(void);
      void       IP_ConfigMaxIFaces             (unsigned NumIFaces);
      void       IP_ConfigNumLinkDownProbes     (U8 IFaceId, U8 NumProbes);
      void       IP_ConfigNumLinkUpProbes       (U8 IFaceId, U8 NumProbes);
      void       IP_ConfigOffCached2Uncached    (I32 Off);
      void       IP_ConfigReportSameMacOnNet    (unsigned OnOff, void* p);
      void       IP_ConfigTCPSpace              (unsigned SendSpace, unsigned RecvSpace);  // Set window sizes
      int        IP_Connect                     (unsigned IFaceId);
      void       IP_DeInit                      (void);
      void       IP_DisableIPRxChecksum         (U8 IFace);
      void       IP_DisableIPv4                 (void);
      int        IP_Disconnect                  (unsigned IFaceId);
      void       IP_EnableIPRxChecksum          (U8 IFace);
      I32        IP_Exec                        (void);
      int        IP_FindIFaceByIP               (void* pAddr, unsigned Len);
      void       IP_GetAddrMask                 (U8 IFace, U32 *pAddr, U32 *pMask);
      int        IP_GetCurrentLinkSpeed         (void);
      int        IP_GetCurrentLinkSpeedEx       (unsigned IFaceId);
      U32        IP_GetFreePacketCnt            (U32 NumBytes);
      U32        IP_GetGWAddr                   (U8 IFace);
      U32        IP_GetIFaceHeaderSize          (unsigned IFaceId);
      void       IP_GetHWAddr                   (unsigned IFaceId, U8 *pDest, unsigned Len);
      U32        IP_GetIPAddr                   (unsigned IFaceId);
const char *     IP_GetIPPacketInfo             (IP_PACKET *pPacket);
      U32        IP_GetMaxAvailPacketSize       (int IFaceId);
      int        IP_GetPrimaryIFace             (void);
const char *     IP_GetRawPacketInfo            (const IP_PACKET *pPacket, U16 *pNumBytes);
      int        IP_GetVersion                  (void);                   // Format: Mmmrr. Sample 10201 is 1.02a
      int        IP_IFaceIsReady                (void);
      int        IP_IFaceIsReadyEx              (unsigned IFaceId);
      int        IP_Init                        (void);
      int        IP_IsAllZero                   (const U8* p, unsigned NumBytes);
      int        IP_IsExpired                   (I32 Time);
      int        IP_IsInitialized               (void);
      void       IP_Panic                       (const char *sError);
      void       IP_PrintStatus                 (void);
      void       IP_SetAddrMask                 (U32 Addr, U32 Mask);
      void       IP_SetAddrMaskEx               (U8 IFace, U32 Addr, U32 Mask);
      void       IP_SetPacketToS                (IP_PACKET *pPacket, U8 ToS);
      void       IP_SetGlobalMcTTL              (int v);
      void       IP_SetLocalMcTTL               (int v);
      void       IP_SetTTL                      (int v);
      void       IP_SetGWAddr                   (U8 IFace, U32 GWAddr);
      void       IP_SetHWAddr                   (const U8 *pHWAddr);
      void       IP_SetHWAddrEx                 (unsigned IFaceId, const U8* pHWAddr, unsigned NumBytes);
      void       IP_SetIFaceConnectHook         (unsigned IFaceId, int (*pf)(unsigned IFaceId));
      void       IP_SetIFaceDisconnectHook      (unsigned IFaceId, int (*pf)(unsigned IFaceId));
      int        IP_SetPrimaryIFace             (int IFaceId);
      void       IP_SetRandCallback             (void (*pfGetRand)(U8* pBuffer, unsigned NumBytes));
      int        IP_SendPacket                  (unsigned IFace, void *pData, int NumBytes);
      int        IP_SendPing                    (U32 FHost, char *pData, unsigned NumBytes, U16 SeqNum);
      int        IP_SendPingEx                  (U32 IFaceId, U32 FHost, char *pData, unsigned NumBytes, U16 SeqNum);
      int        IP_SendPingCheckReply          (U32 IFaceId, U32 FHost, char* pData, unsigned NumBytes, unsigned ms);
#if IP_SUPPORT_ON_PACKET_FREE_CB
      void       IP_SetOnPacketFreeCallback     (IP_PACKET* pPacket, void (*pfOnFreeCB)(IP_PACKET* pPacketCB, void* pContextCB), void* pContext);
#endif
      void       IP_SetRxHook                   (IP_RX_HOOK *pfRxHook);
      int        IP_SetSupportedDuplexModes     (unsigned IFace, unsigned DuplexMode);
      void       IP_SetUseRxTask                (void);
      unsigned   IP_Shutdown                    (unsigned LeaveTaskLoop, U32 Timeout);
      IP_PACKET* IP_AllocEtherPacket            (unsigned IFaceId, U32 NumBytes, U8** ppBuffer);
      void       IP_FreePacket                  (IP_PACKET* pPacket);
      int        IP_SendEtherPacket             (unsigned IFaceId, IP_PACKET* pPacket, U32 NumBytes);
      void       IP_AddEtherTypeHook            (IP_HOOK_ON_ETH_TYPE* pHook, int (*pf)(unsigned IFaceId, IP_PACKET* pPacket, void* pBuffer, U32 NumBytes), U16 Type);
      void       IP_AddOnPacketFreeHook         (IP_HOOK_ON_PACKET_FREE *pHook, void (*pf)(IP_PACKET* pPacket));
      void       IP_X_Config                    (void);
      U32        IP_GetMTU                      (unsigned IFaceId);
      void       IP_SetMTU                      (unsigned IFaceId, U32 Mtu);

/*********************************************************************
*
*       Fragmentation related functions
*/
#if IP_SUPPORT_FRAGMENTATION_IPV4
void IP_FRAGMENT_Enable  (void);
void IP_FRAGMENT_ConfigRx(U16 MaxFragments, U32 Timeout, U8 KeepOOO);
#endif

/*********************************************************************
*
*       IP_MICREL_TAIL_TAGGING_
*/
int IP_MICREL_TAIL_TAGGING_AddInterface(unsigned HWIFaceId, U8 InTag, U8 OutTag);

/*********************************************************************
*
*       IP_ARP_
*/
void IP_ARP_CleanCache              (void);
void IP_ARP_CleanCacheByInterface   (unsigned IFaceId);
void IP_ARP_ConfigAllowGratuitousARP(U8 OnOff);
int  IP_ARP_ConfigNumEntries        (unsigned NumEntries);
void IP_ARP_ConfigAgeout            (U32 Ageout);
void IP_ARP_ConfigAgeoutSniff       (U32 Ageout);
void IP_ARP_ConfigAgeoutNoReply     (U32 Ageout);
void IP_ARP_ConfigMaxRetries        (unsigned Retries);
void IP_ARP_ConfigMaxPending        (unsigned NumPackets);

/*********************************************************************
*
*       IP_AutoIP_
*/

#define  AUTOIP_STATE_UNUSED      0
#define  AUTOIP_STATE_INITREBOOT  1
#define  AUTOIP_STATE_INIT        2
#define  AUTOIP_STATE_SENDPROBE   3
#define  AUTOIP_STATE_CHECKREPLY  4
#define  AUTOIP_STATE_BOUND       5

typedef void (IP_AUTOIP_INFORM_USER_FUNC)(U32 IFace, U32 Stat);

void IP_AutoIP_Activate       (unsigned IFaceId);
int  IP_AutoIP_Halt           (unsigned IFaceId, char KeepIP);
void IP_AutoIP_SetUserCallback(unsigned IFaceId, IP_AUTOIP_INFORM_USER_FUNC * pfInformUser);
void IP_AutoIP_SetStartIP     (unsigned IFaceId, U32 IPAddr);


/*********************************************************************
*
*       IP_SOCKET_
*  Typically located in IP_Socket.h but kept in here for dependencies
*  like IP_PACKET or other public structures.
*/
int IP_SOCKET_SetCallback(int hSock, int (*pfCallback)(int hSock, IP_PACKET* pPacket, int MsgCode));

/*********************************************************************
*
*       IP_TCP_
*/
#define IPV4_TCP_HEADER_LEN  (72u)  // Helper define, typical  worst-case header assumed by the stack for IPv4 & TCP. 40 IP/TCP, 12 RTTM, 14+2 Ether, 4 VLAN.

typedef struct {
  U16 FPort;
  U16 LPort;
} IP_TCP_ACCEPT_INFO;

typedef struct IP_TCP_ACCEPT_HOOK_STRUCT IP_TCP_ACCEPT_HOOK;
struct IP_TCP_ACCEPT_HOOK_STRUCT {
  IP_TCP_ACCEPT_HOOK* pNext;
  void (*pfAccept)(int hSock, IP_TCP_ACCEPT_INFO* pInfo, void* pContext);
  void*               pContext;
  int                 hSock;
};

int  IP_TCP_Accept                (IP_TCP_ACCEPT_HOOK* pHook, void (*pfAccept)(int hSock, IP_TCP_ACCEPT_INFO* pInfo, void* pContext), int hSock, void* pContext);
void IP_TCP_Add                   (void);
void IP_TCP_DisableRxChecksum     (U8 IFace);
void IP_TCP_EnableRxChecksum      (U8 IFace);
void IP_TCP_Set2MSLDelay          (unsigned v);
void IP_TCP_SetConnKeepaliveOpt   (U32 Init, U32 Idle, U32 Period, U32 Cnt);
void IP_TCP_SetRetransDelayRange  (unsigned RetransDelayMin, unsigned RetransDelayMax);

/*********************************************************************
*
*       TCP Zero copy
*/
IP_PACKET*  IP_TCP_Alloc      (int NumBytes);
IP_PACKET*  IP_TCP_AllocEx    (int NumBytes, int NumBytesHeader);
void        IP_TCP_Free       (           IP_PACKET * pPacket);
int         IP_TCP_Send       (int hSock, IP_PACKET * pPacket);
int         IP_TCP_SendAndFree(int hSock, IP_PACKET * pPacket);

/*********************************************************************
*
*       UDP & RAW zero copy return values
*/
#define IP_RX_ERROR              -1
#define IP_OK                     0
#define IP_OK_KEEP_PACKET         1
#define IP_OK_KEEP_IN_SOCKET      2
#define IP_OK_TRY_OTHER_HANDLER   3

/*********************************************************************
*
*       UDP
*/
typedef struct {
  IP_DLIST_ITEM Link;
  U16           LPort;
  U16           FPort;
  void         *pIPAddrInfo;
  int           (*pfOnRx)(IP_PACKET* pPacket, void* pContext);
  void         *pContext;
  U8            IsIPv6;
} IP_UDP_CONNECTION;

typedef IP_UDP_CONNECTION*  IP_UDP_CONN_HANDLE;  // Used as handle by the application.

void                 IP_UDP_Add              (void);
IP_UDP_CONNECTION *  IP_UDP_AddEchoServer    (U16 LPort);
IP_UDP_CONNECTION *  IP_UDP_Open             (IP_ADDR FAddr, U16 FPort,                U16 LPort, int (*handler)(IP_PACKET *pPacket, void *pContext), void *pContext);
IP_UDP_CONNECTION *  IP_UDP_OpenEx           (IP_ADDR FAddr, U16 FPort, IP_ADDR LAddr, U16 LPort, int (*handler)(IP_PACKET *pPacket, void *pContext), void *pContext);
void                 IP_UDP_Close            (IP_UDP_CONNECTION* pCon);
IP_PACKET          * IP_UDP_Alloc            (int NumBytes);
IP_PACKET          * IP_UDP_AllocEx          (unsigned IFaceId, unsigned NumBytes);
int                  IP_UDP_Send             (int IFace, IP_ADDR FHost, U16 fport, U16 lport, IP_PACKET *pPacket);
int                  IP_UDP_SendAndFree      (int IFace, IP_ADDR FHost, U16 fport, U16 lport, IP_PACKET *pPacket);
void                 IP_UDP_Free             (IP_PACKET *pPacket);
U16                  IP_UDP_FindFreePort     (void);
U16                  IP_UDP_GetFPort         (const IP_PACKET *pPacket);
U16                  IP_UDP_GetLPort         (const IP_PACKET *pPacket);
unsigned             IP_UDP_GetIFIndex       (const IP_PACKET *pPacket);
void               * IP_UDP_GetDataPtr       (const IP_PACKET *pPacket);
U16                  IP_UDP_GetDataSize      (const IP_PACKET *pPacket);
void                 IP_UDP_GetDestAddr      (const IP_PACKET *pPacket, void *pDestAddr, int AddrLen);
void                 IP_UDP_GetSrcAddr       (const IP_PACKET *pPacket, void *pSrcAddr , int AddrLen);
int                  IP_UDP_ReducePayloadLen (IP_PACKET *pPacket, int NumBytes);

void                 IP_UDP_EnableRxChecksum (void);
void                 IP_UDP_DisableRxChecksum(void);
void                 IP_UDP_EnableTxChecksum (void);
void                 IP_UDP_DisableTxChecksum(void);

/*********************************************************************
*
*       RAW
*/
typedef struct {
  IP_DLIST_ITEM Link;
  U32           LHost;
  U32           FHost;
  int           (*pfOnRx)(IP_PACKET* pPacket, void* pContext);
  void *        pContext;
  U8            Protocol;
  U8            IpHdrIncl;
} IP_RAW_CONNECTION;

typedef IP_RAW_CONNECTION*  IP_RAW_CONN_HANDLE;  // Used as handle by the application.

#define IPPROTO_IP      (0)  // Dummy value for TCP.
#define IPPROTO_ICMP    (1)  // Internet Control Message Protocol.
#define IPPROTO_TCP     (6)  // Transmission Control Protocol.
#define IPPROTO_UDP    (17)  // User Datagram Protocol.
#define IPPROTO_RAW   (255)  // RAW IP packets.

void                 IP_RAW_Add              (void);
int                  IP_RAW_AddPacketToSocket(int hSock, IP_PACKET* pPacket);
IP_RAW_CONNECTION*   IP_RAW_Open             (IP_ADDR FAddr, IP_ADDR LAddr, U8 Protocol, int (*handler)(IP_PACKET* pPacket, void* pContext), void * pContext);
void                 IP_RAW_Close            (IP_RAW_CONNECTION* pCon);
IP_PACKET          * IP_RAW_Alloc            (unsigned IFaceId, unsigned NumBytesData, int IpHdrIncl);
int                  IP_RAW_Send             (int IFace, IP_ADDR FHost, U8 Protocol, IP_PACKET * pPacket);
int                  IP_RAW_SendAndFree      (int IFace, IP_ADDR FHost, U8 Protocol, IP_PACKET * pPacket);
void                 IP_RAW_Free             (IP_PACKET * pPacket);
unsigned             IP_RAW_GetIFIndex       (const IP_PACKET *pPacket);
void               * IP_RAW_GetDataPtr       (const IP_PACKET *pPacket);
U16                  IP_RAW_GetDataSize      (const IP_PACKET *pPacket);
void                 IP_RAW_GetDestAddr      (const IP_PACKET *pPacket, void *pDestAddr, int AddrLen);
void                 IP_RAW_GetSrcAddr       (const IP_PACKET *pPacket, void *pSrcAddr , int AddrLen);
int                  IP_RAW_ReducePayloadLen (IP_PACKET *pPacket, int NumBytes);

/*********************************************************************
*
*       IGMP_
*/
#define IP_IGMP_MCAST_ALLHOSTS_GROUP    0xE0000001uL  // 224.0.0.1
#define IP_IGMP_MCAST_ALLROUTERS_GROUP  0xE0000002uL  // 224.0.0.2
#define IP_IGMP_MCAST_ALLRPTS_GROUP     0xE0000016uL  // 224.0.0.22, IGMPv3

int  IP_IGMP_Add       (void);
int  IP_IGMP_AddEx     (unsigned IFaceId);
void IP_IGMP_JoinGroup (unsigned IFaceId, IP_ADDR GroupIP);
void IP_IGMP_LeaveGroup(unsigned IFaceId, IP_ADDR GroupIP);

/*********************************************************************
*
*       UPNP_
*/
void IP_UPNP_Activate(unsigned IFace, const char * acUDN);

/*********************************************************************
*
*       IP_PPPOE_
*/
typedef void (IP_PPPOE_INFORM_USER_FUNC)(U32 IFaceId, U32 Stat);

int        IP_PPPOE_AddInterface   (unsigned IFaceId);
void       IP_PPPOE_ConfigRetries  (unsigned IFaceId, U32 NumTries, U32 Timeout);
void       IP_PPPOE_Reset          (unsigned IFaceId);
void       IP_PPPOE_SetAuthInfo    (unsigned IFaceId, const char * sUser, const char * sPass);
void       IP_PPPOE_SetUserCallback(U32 IFaceId, IP_PPPOE_INFORM_USER_FUNC * pfInformUser);

/*********************************************************************
*
*       IP_VLAN_
*/
int IP_VLAN_AddInterface(unsigned HWIFace, U16 VLANId);

/*********************************************************************
*
*       IP_SNTPC_
*/
typedef struct IP_NTP_TIMESTAMP {
  U32 Seconds;
  U32 Fractions;
} IP_NTP_TIMESTAMP;

#define IP_SNTPC_STATE_NO_ANSWER  0  // Request sent but no answer received from NTP server within timeout
#define IP_SNTPC_STATE_UPDATED    1  // Timestamp received from NTP server
#define IP_SNTPC_STATE_KOD        2  // Kiss-Of-Death received from server. This means the server wants us to use another server.

void IP_SNTPC_ConfigTimeout         (unsigned ms);
int  IP_SNTPC_GetTimeStampFromServer(unsigned IFaceId, const char * sServer, IP_NTP_TIMESTAMP * pTimestamp);

/*********************************************************************
*
*       IP_MODEM_
*/
struct IP_PPP_CONTEXT;
typedef struct {
  void (*pfInit)               (struct IP_PPP_CONTEXT * pPPPContext); //
  void (*pfSend)               (U8 Data);                             // Send the first data byte
  void (*pfSendNext)           (U8 Data);                             // Send the next data byte
  void (*pfTerminate)          (U8 IFaceId);                          // Terminate connection
  void (*pfOnPacketCompletion) (void);                                // Optional. Called when packet is complete. Normally used for packet oriented PPP interfaces GPRS or USB modems.
} IP_PPP_LINE_DRIVER;

extern const IP_PPP_LINE_DRIVER MODEM_Driver;

      int    IP_MODEM_Connect            (const char * sATCommand);
      void   IP_MODEM_Disconnect         (unsigned IFaceId);
const char * IP_MODEM_GetResponse        (unsigned IFaceId, char * pBuffer, unsigned NumBytes, unsigned * pNumBytesInBuffer);
      void   IP_MODEM_SendString         (unsigned IFaceId, const char * sCmd);
      int    IP_MODEM_SendStringEx       (unsigned IFaceId, const char * sCmd, const char * sResponse, unsigned Timeout, unsigned RecvBufOffs);
      void   IP_MODEM_SetAuthInfo        (unsigned IFaceId, const char * sUser, const char * sPass);
      void   IP_MODEM_SetConnectTimeout  (unsigned IFaceId, unsigned ms);
      void   IP_MODEM_SetInitCallback    (void (*pfInit)(void));
      void   IP_MODEM_SetInitString      (const char * sInit);
      void   IP_MODEM_SetSwitchToCmdDelay(unsigned IFaceId, unsigned ms);

/*********************************************************************
*
*       IP_PPP_
*/
typedef void (IP_PPP_INFORM_USER_FUNC)(U32 IFaceId, U32 Stat);

int  IP_PPP_AddInterface   (const IP_PPP_LINE_DRIVER * pLineDriver, int ModemIndex);
void IP_PPP_OnRx           (struct IP_PPP_CONTEXT * pContext, U8 * pData, int NumBytes);
void IP_PPP_OnRxChar       (struct IP_PPP_CONTEXT * pContext, U8 Data);
int  IP_PPP_OnTxChar       (unsigned Unit);
void IP_PPP_SetUserCallback(U32 IFaceId, IP_PPP_INFORM_USER_FUNC * pfInformUser);

/*********************************************************************
*
*       IP_ICMP_
*/
void IP_ICMP_Add              (void);
void IP_ICMP_DisableRxChecksum(U8 IFace);
void IP_ICMP_EnableRxChecksum (U8 IFace);
void IP_ICMP_SetRxHook        (IP_RX_HOOK *pfRxHook);

//
// Echo or Echo Reply Message
//
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |     Type      |     Code      |          Checksum             |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |           Identifier          |        Sequence Number        |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |     Data ...
//   +-+-+-+-+-
//
typedef struct {
  U8  Type;
  U8  Code;
  U16 Checksum;
  U16 Id;
  U16 Seq;
} IP_ICMP_HEADER;

#define IP_ICMP_TYPE_ECHO_REPLY        (0u)
#define IP_ICMP_TYPE_DEST_UNREACHABLE  (3u)
#define IP_ICMP_TYPE_ECHO_REQUEST      (8u)

/*********************************************************************
*
*       Log/Warn functions (public)
*/
void IP_AddLogFilter     (U32 TypeMask);
void IP_AddWarnFilter    (U32 TypeMask);
void IP_RemoveLogFilter  (U32 TypeMask);
void IP_RemoveWarnFilter (U32 TypeMask);
void IP_SetLogFilter     (U32 TypeMask);
void IP_SetWarnFilter    (U32 TypeMask);
void IP_Log              (const char* s);
void Interrupt_Log       (const char* s);
void IP_Logf_Application (const char* sFormat, ...);
void IP_Warn             (const char* s);
void IP_Warnf_Application(const char* sFormat, ...);

/*********************************************************************
*
*       Log/Warn functions (internal)
*
*  Typically only used by the stack internally. Made public only for
*  usage in IP_Config callbacks for driver init log messages.
*/
void IP_Logf (U32 TypeMask, const char * sFormat, ...);
void IP_Warnf(U32 TypeMask, const char * sFormat, ...);

/*********************************************************************
*
*       Client: DNS-SD (DNS - service discovery) / mDNS / LLMNR
*
*/

//
//  Reply structure.
//
typedef struct {
        void*     pContext;
        U16       Type;
  const char*     sName;
  const char*     sRoot;
        U32       TTL;
  union {
    struct {
      IP_ADDR     IPAddr;  // In network endianness.
    } A;
    //
#if IP_SUPPORT_IPV6
    struct {
      U8*         pIPAddrV6;
    } AAAA;
#endif
    //
    struct {
      char*       sDomainName;
      char*       sRoot;
    } PTR;
    //
    struct {
      U16         Priority;
      U16         Weight;
      U16         Port;
      char*       sTarget;
      char*       sRoot;
    } SRV;
    //
    struct {
      char*       sTXT;  // This string is not null-terminated.
      U8          Length;
    } TXT;
  } Config;
} IP_MDNS_REPLY;

//
// Request structure
//
typedef struct {
        void* pContext;        // Context that will be set for each reply in IP_MDNS_REPLY as well.
  const char* sHost;
        void (*pfResult)(IP_MDNS_REPLY* pReply, unsigned IsLast);
        U16   Type;
        U8    FirstReplyOnly;  // Wait for the first reply and ignore potential other replies.
} IP_DNSSD_REQUEST;

int IP_MDNS_ResolveHost        (unsigned IFaceId, const IP_DNSSD_REQUEST* pRequest, int ms);
int IP_MDNS_ResolveHostSingleIP(unsigned IFaceId, void* pIP, const char* sHost, U16 Type, int ms);

/*********************************************************************
*
*       DNS (Domain name system)
*
*  Name resolution
*/
// Description of data base entry for a single host.
struct hostent {
  char *  h_name;        // Official name of host.
  char ** h_aliases;     // Alias list.
  int     h_addrtype;    // Host address type.
  int     h_length;      // Length of address.
  char ** h_addr_list;   // List of addresses from name server.
#define h_addr h_addr_list[0] /* Address, for backward compatibility.  */
#ifdef DNS_CLIENT_UPDT
  // Extra variables passed in to Dynamic DNS updates.
  char *  h_z_name;      // IN- zone name for UPDATE packet.
  ip_addr h_add_ipaddr;  // IN- add this ip address for host name in zone.
  U32     h_ttl;         // IN- time-to-live field for UPDATE packet.
#endif
};

int  IP_ResolveHost       (const char *sHost, U32 *pIPAddr, U32 ms);
int  IP_DNS_ResolveHostEx (unsigned IFaceId, const IP_DNSSD_REQUEST* pRequest, int Ms);
void IP_DNS_SetServer     (U32 DNSServerAddr);
U32  IP_DNS_GetServer     (void);
int  IP_DNS_SetServerEx   (unsigned IFaceId, U8 DNSServer, const U8 *pDNSAddr, int AddrLen);
void IP_DNS_GetServerEx   (unsigned IFaceId, U8 DNSServer, U8 *pDNSAddr, int *pAddrLen);
void IP_DNS_SetMaxTTL     (U32 TTL);
int  IP_DNS_SendDynUpdate (unsigned IFaceId, const char* sHost, const char* sDomain, U32 IPv4Addr, int ClearPreviousRR, U32 ms);
void IP_DNS_SetTSIGContext(char *KeyName, char *KeyAlgoName, int (*pfSign)(U8* pData, U16 DataLength, U8* pDigest, int DigestMaxSize), int (*pfGetTime)(U32* pSeconds)); // The 2 strings must be static.

/*********************************************************************
*
*       Server: DNS-SD (DNS - service discovery) / mDNS / LLMNR
*
*  Discovery.
*/
#define IP_DNS_SERVER_TYPE_A       (1u)       // IPv4 address.
#define IP_DNS_SERVER_TYPE_PTR     (12u)      // PTR record.
#define IP_DNS_SERVER_TYPE_TXT     (16u)      // TXT record.
#define IP_DNS_SERVER_TYPE_SRV     (33u)      // SRV record.
#define IP_DNS_SERVER_TYPE_AAAA    (28u)      // IPv6 address.
#define IP_DNS_SERVER_TYPE_ALL     (255u)     // All records '*'.

#define IP_DNS_SERVER_FLAG_FLUSH   (1u << 0)  // Tell the host that is asking to flush this entry in any case.
                                              // Typically at least A and AAAA records should use the FLUSH bit
                                              // as they should always be unique names.

//
// Configurations for the different DNS-SD types: A, AAAA, PTR, SRV, TXT.
//
typedef struct {
  char*                    sName;        // No need to have a A record for the hostname. The IP address of the interface is used in this case.
  IP_ADDR                  IPAddr;
} IP_DNS_SERVER_A;

#if IP_SUPPORT_IPV6
typedef struct {
  char*                    sName;        // No need to have a AAAA record for the hostname. The IP address of the interface is used in this case.
  U8                       aIPAddrV6[IPV6_ADDR_LEN];
} IP_DNS_SERVER_AAAA;
#endif

typedef struct {
  char*                    sName;        // Either a real name _service._udp.local or a formated IP address 01.00.168.192.in-addr.arpa
  char*                    sDomainName;  // If NULL, hostname will be used.
} IP_DNS_SERVER_PTR;

typedef struct {
  char*                    sName;
  U16                      Priority;
  U16                      Weight;
  U16                      Port;
  char*                    sTarget;      // If NULL, hostname will be used.
} IP_DNS_SERVER_SRV;

typedef struct {
  char*                    sName;        // If NULL, hostname will be used.
  char*                    sTXT;
} IP_DNS_SERVER_TXT;

//
// DNS-SD configuration.
//
typedef struct {
  U32                        TTL;
  union {
    IP_DNS_SERVER_A          A;
#if IP_SUPPORT_IPV6
    IP_DNS_SERVER_AAAA       AAAA;
#endif
    IP_DNS_SERVER_PTR        PTR;
    IP_DNS_SERVER_SRV        SRV;
    IP_DNS_SERVER_TXT        TXT;          // Note: All TXT records for the same name shall be contiguous in the configuration table.
  } Config;
  U8                         Type;         // A, AAAA, PTR, SRV, TXT
  U8                         Flags;        // IP_DNS_SERVER_FLAG_FLUSH; If no flush flag is used for override, future releases might try to figure out the flush flag by probing.
                                           // See RFC 6762 section 10.2. "Announcements to Flush Outdated Cache Entries" and section 8.1 "Probing".
} IP_DNS_SERVER_SD_CONFIG;

//
// Discovery configuration. The apSDConfig array is a list of IP_DNS_SERVER_SD_CONFIG.
//
typedef struct {
  const char*                      sHostname;
        U32                        TTL;          // If set to 0, default value DNS_TTL_INIT will be used.
        unsigned                   NumConfig;    // Number of configuration in the array pointed by apSDConfig. 0 if apSDConfig is NULL.
  const IP_DNS_SERVER_SD_CONFIG*   apSDConfig;   // Configuration array. Could be NULL.
} IP_DNS_SERVER_CONFIG;

int IP_MDNS_SERVER_Start(const IP_DNS_SERVER_CONFIG* pConfig);
int IP_MDNS_SERVER_Stop (void);
int IP_DNS_SERVER_Start (const IP_DNS_SERVER_CONFIG* pConfig);
int IP_DNS_SERVER_Stop  (void);

/*********************************************************************
*
*       Netbios Name Service (Add-on)
*/
typedef struct {
  char* sName;
  U8    NumBytes;
} IP_NETBIOS_NAME;

int  IP_NETBIOS_Init (U32 IFaceId, const IP_NETBIOS_NAME * paHostnames, U16 LPort);
int  IP_NETBIOS_Start(U32 IFaceId);
void IP_NETBIOS_Stop (U32 IFaceId);

/*********************************************************************
*
*       Utility functions
*/
#define IP_LoadU32BE  SEGGER_RdU32BE
#define IP_LoadU32LE  SEGGER_RdU32LE
#define IP_LoadU16BE  SEGGER_RdU16BE
#define IP_LoadU16LE  SEGGER_RdU16LE
#define IP_StoreU32BE SEGGER_WrU32BE
#define IP_StoreU32LE SEGGER_WrU32LE
#define IP_tolower    SEGGER_tolower
#define IP_isalpha    SEGGER_isalpha
#define IP_isalnum    SEGGER_isalnum

U32  IP_LoadU32TE    (const U8* pData);
I32  IP_BringInBounds(I32 v, I32 Min, I32 Max);
int  IP_PrintIPAddr  (char* pBuffer, U32 IPAddr, int BufferSize);
int  IP_PrintIPAddrEx(char* pBuffer, int BufferSize, const U8* pIPAddr, int AddrLen);
U32  IP_SwapU32      (U32 v);

/*********************************************************************
*
*       IP_DHCPC_...
*
*  DHCP (Dynamic host configuration protocol) client functions.
*/
#define  DHCPC_RESET_CONFIG       0
#define  DHCPC_USE_STATIC_CONFIG  1
#define  DHCPC_USE_DHCP_CONFIG    2

//
// DHCP client states
//
#define  DHCPC_STATE_UNUSED       0u
#define  DHCPC_STATE_INIT         1u
#define  DHCPC_STATE_INITREBOOT   2u
#define  DHCPC_STATE_REBOOTING    3u
#define  DHCPC_STATE_SELECTING    4u
#define  DHCPC_STATE_REQUESTING   5u
#define  DHCPC_STATE_BOUND        6u
#define  DHCPC_STATE_RENEWING     7u
#define  DHCPC_STATE_REBINDING    8u
#define  DHCPC_STATE_RESTARTING   9u
#define  DHCPC_STATE_CHECK_IP    10u  // Not part of the diagram: This state is entered before "BOUND", when waiting for answer to ARP-packet to make sure IP-addr is not in use
#define  DHCPC_STATE_WAIT_INIT   11u  // Not part of the diagram: This state is entered if CHECK_IP fails. RFC2131, 3.1.5 says: The client SHOULD wait a minimum of ten seconds before restarting the configuration process to avoid excessive network traffic in case of looping.

typedef struct {
  char* sDomain;
  U32   IPAddr;
  U32   Gateway;
  U32   SubnetMask;
  U8    DomainLen;
} IP_DHCPC_STATE_INFO;

typedef struct IP_DHCPC_HOOK_ON_STATE_CHANGE {
  void (*pf)(unsigned IFaceId, unsigned State, IP_DHCPC_STATE_INFO* pInfo);  // Pointer to the function to be called from the hook.
  struct IP_DHCPC_HOOK_ON_STATE_CHANGE* pNext;                               // Pointer to the next hook.
} IP_DHCPC_HOOK_ON_STATE_CHANGE;

/*********************************************************************
*
*       IP_DHCPC_ON_OPTION_INFO
*
*  Function description
*    Returns information about the next DHCP option to be processed.
*/
typedef struct {
  const U8* pVal;    // Value of the DHCP option.
        int Status;  // * == 0: O.K.
                     // * <  0: Parse error, abort of parser.
        U8  Type;    // DHCP option type. Please refer to RFC 1533
                     // for further information.
        U8  Len;     // Length of the option value.
} IP_DHCPC_ON_OPTION_INFO;

/*********************************************************************
*
*       IP_DHCPC_ON_OPTION_FUNC
*
*  Function description
*    Callback executed for every DHCP option received.
*
*  Parameters
*    IFaceId: Zero-based interface index.
*    pInfo  : Further information of type IP_DHCPC_ON_OPTION_INFO
*             about the DHCP option parsed.
*
*  Additional information
*    Once all options are parsed the end marker (option type 0xFF) is
*    reported as well for an easy to detect end of the list from
*    within the callback. No end is signaled if there was an abort
*    that can be detected by looking at pInfo->Status .
*/
typedef void (IP_DHCPC_ON_OPTION_FUNC)(unsigned IFaceId, IP_DHCPC_ON_OPTION_INFO* pInfo);

void     IP_DHCPC_AddStateChangeHook   (IP_DHCPC_HOOK_ON_STATE_CHANGE* pHook, void (*pf)(unsigned IFaceId, unsigned State, IP_DHCPC_STATE_INFO* pInfo));
int      IP_DHCPC_Activate             (int IFaceId, const char * sHost, const char * sDomain, const char * sVendor);
int      IP_DHCPC_ConfigAlwaysStartInit(int IFaceId, U8 OnOff);
int      IP_DHCPC_ConfigDNSManually    (int IFaceId, U8 OnOff);
int      IP_DHCPC_ConfigOnActivate     (int IFaceId, U8 Mode);
int      IP_DHCPC_ConfigOnFail         (int IFaceId, U8 Mode);
int      IP_DHCPC_ConfigOnLinkDown     (int IFaceId, U32 Timeout, U8 Mode);
int      IP_DHCPC_ConfigUniBcStartMode (int IFaceId, U8 Mode);
int      IP_DHCPC_GetOptionRequestList (int IFaceId, U8* pBuffer, unsigned BufferSize);
U32      IP_DHCPC_GetServer            (int IFaceId);
unsigned IP_DHCPC_GetState             (int IFaceId);
int      IP_DHCPC_Halt                 (int IFaceId);
int      IP_DHCPC_Renew                (int IFaceId);
int      IP_DHCPC_SetCallback          (int IFaceId, int (*routine)(int IFaceId, int State) );
int      IP_DHCPC_SetClientId          (int IFaceId, const U8* pClientId, unsigned ClientIdLen);
int      IP_DHCPC_SetOptionRequestList (int IFaceId, const U8* pOptions, unsigned NumOptions);
void     IP_DHCPC_SetOnOptionCallback  (IP_DHCPC_ON_OPTION_FUNC* pf);
void     IP_DHCPC_SetTimeout           (int IFaceId, U32 Timeout, U32 MaxTries, unsigned Exponential);

/*********************************************************************
*
*       IP_DHCPS_...
*
*  DHCP (Dynamic host configuration protocol) server functions.
*/

/*********************************************************************
*
*       IP_DHCPS_RESERVE_ADDR
*
*  Function description
*    Reserves a DHCP IPv4 address via HW address, hostname or both.
*/
typedef struct {
  const U8*   pHWAddr;    // Client HW/MAC address to reserve to. Can be NULL.
        U32   IPAddr;     // IPv4 address to reserve in host endianess.
                          // Does not need to be from the DHCP pool itself.
  const char* sHostName;  // Client hostname to reserve to. Can be NULL.
} IP_DHCPS_RESERVE_ADDR;

/*********************************************************************
*
*       IP_DHCPS_GET_VENDOR_OPTION_INFO
*
*  Function description
*    Returns information about the vendor specific identifier
*    received with DHCP option 60.
*/
typedef struct {
  U8* pVendorClassId;  // Pointer to the DHCP option 60 field received from
                       // a client including type and length bytes. A
                       // typical example would be Type: 60, Len: 8 and
                       // Value: 'M' 'S' 'F' 'T' ' ' '5' '.' '0' for a
                       // Microsoft client that supports vendor specific
                       // DHCP option 43 commands.
} IP_DHCPS_GET_VENDOR_OPTION_INFO;

/*********************************************************************
*
*       IP_DHCPS_GET_VENDOR_OPTION_FUNC
*
*  Function description
*    Inserts a vendor specific configuration for DHCP option 43.
*
*  Parameters
*    IFaceId : Zero-based interface index.
*    pInfo   : Further information of type IP_DHCPS_GET_VENDOR_OPTION_INFO
*              about the vendor of the client.
*    ppOption: Pointer to the pointer where to add further options.
*              The dereferenced pointer needs to be incremented
*              by the number of bytes added. Type and length bytes
*              need to be added by the callback as well.
*    NumBytes: Number of free bytes that can be used to store
*              options from the callback.
*/
typedef void (IP_DHCPS_GET_VENDOR_OPTION_FUNC)(unsigned IFaceId, IP_DHCPS_GET_VENDOR_OPTION_INFO* pInfo, U8** ppOption, unsigned NumBytes);

int  IP_DHCPS_ConfigDNSAddr           (unsigned IFIndex, U32 *paDNSAddr, U8 NumServers);
int  IP_DHCPS_ConfigGWAddr            (unsigned IFIndex, U32 GWAddr);
int  IP_DHCPS_ConfigMaxLeaseTime      (unsigned IFIndex, U32 Seconds);
int  IP_DHCPS_ConfigPool              (unsigned IFIndex, U32 StartIPAddr, U32 SNMask, U32 PoolSize);
void IP_DHCPS_Halt                    (unsigned IFIndex);
int  IP_DHCPS_Init                    (unsigned IFIndex);
int  IP_DHCPS_SetReservedAddresses    (unsigned IFIndex, const IP_DHCPS_RESERVE_ADDR* paAddr, unsigned NumAddr);
void IP_DHCPS_SetVendorOptionsCallback(IP_DHCPS_GET_VENDOR_OPTION_FUNC* pf);
int  IP_DHCPS_Start                   (unsigned IFIndex);

/*********************************************************************
*
*       IP_BOOTPC_...
*
*  BOOTP - bootstrap Protocol
*/
int IP_BOOTPC_Activate(int IFaceId);
//
// We do not need extra functions, since we have the compatible functions with IP_DHCPC_ prefix
//
#define IP_BOOTPC_Halt(IFIndex)                                     IP_DHCPC_Halt(IFIndex)
#define IP_BOOTPC_SetTimeout(IFIndex,Timeout,MaxTries,Exponential)  IP_DHCPC_SetTimeout(IFIndex,Timeout,MaxTries,1)

/*********************************************************************
*
*       IP_ACD_...
*
*       Address conflict detection (ACD)
*/
typedef struct {
  U32 (*pfRenewIPAddr)(unsigned IFace);  // Used to renew the IP address if a conflict has been detected during startup. Return value: New IP addr. to try
  int (*pfDefend)     (unsigned IFace);  // Used to defend the IP address against a conflicting host on the network.
  int (*pfRestart)    (unsigned IFace);  // Used to restart the address conflict detection.
} ACD_FUNC;

int  IP_ACD_Activate(unsigned IFace);
void IP_ACD_Config  (unsigned IFace, unsigned ProbeNum, unsigned DefendInterval, const ACD_FUNC* pACDContext);

/*********************************************************************
*
*       IP_PTP_
*/
#define IP_PTP_FLAGS_SIGNED_MASK             (1uL << 0)  // Negative correction.
#define IP_PTP_FLAGS_COARSE_CORRECTION_MASK  (1uL << 1)  // (Coarse) time correction with calculated offset.
#define IP_PTP_FLAGS_FINE_CORRECTION_MASK    (1uL << 2)  // Fine/drift frequency correction. Not supported by all drivers.
#define IP_PTP_FLAGS_FINE_RESET_MASK         (1uL << 3)  // Fine correction history values used for average caluclation are reset.

typedef enum {
  PTP_STATE_INITIALIZING   = 1,
  PTP_STATE_FAULTY,
  PTP_STATE_DISABLED,
  PTP_STATE_LISTENING,
  PTP_STATE_PRE_MASTER,
  PTP_STATE_MASTER,
  PTP_STATE_PASSIVE,
  PTP_STATE_UNCALIBRATED,
  PTP_STATE_SLAVE
} PTP_STATE;

/*********************************************************************
*
*       IP_PTP_TIMESTAMP
*
*  Function description
*    A structure which stores the PTP timestamp. This is the time
*    passed since the EPOCH of the system.
*/
typedef struct {
  U32 Seconds;      // Time [s] passed since start of epoch of the network, typically since January 1st 1970.
  U32 Nanoseconds;  // The sub-second part of the time passed since the EPOCH expressed in nanoseconds.
} IP_PTP_TIMESTAMP;

#define IP_PTP_INFO_TYPE_CORRECTION       1u  // Typically provided from (simple internal PTP hardware mixed with) our PTP software algorithm where we have full control.
#define IP_PTP_INFO_TYPE_OFFSET           2u  // Typically provided from external PTP hardware where we are only able to receive a subset of information.
#define IP_PTP_INFO_TYPE_MASTER_CHANGED   3u
#define IP_PTP_INFO_TYPE_MASTER_UPDATED   4u

/*********************************************************************
*
*       IP_PTP_CORRECTION_INFO
*
*  Function description
*    Returns information about the latest local time correction.
*/
typedef struct {
  U32 Seconds;      // Seconds
  U32 Nanoseconds;  // Nanoseconds
  U32 Flags;        // ORR-ed combination of IP_PTP_FLAGS_*
  U8  State;        // State before applying the correction.
} IP_PTP_CORRECTION_INFO;

/*********************************************************************
*
*       IP_PTP_OFFSET_INFO
*
*  Function description
*    Returns information about the offset between slave and master.
*/
typedef struct {
  U32 Seconds;      // Seconds
  U32 Nanoseconds;  // Nanoseconds
  U32 Flags;        // ORR-ed combination of IP_PTP_FLAGS_*
} IP_PTP_OFFSET_INFO;

/*********************************************************************
*
*       IP_PTP_MASTER_INFO
*
*  Function description
*    Returns information about a newly selected master or updated
*    parameters with the currently selected one.
*/
typedef struct {
  U8  abGrandmasterIdentity[8];  // Master clock ID.
  U32 UtcOffset;                 // Offset between TAI and UTC in seconds.
  U8  IsUtcOffsetValid;          // Is the offset valid ? Basically this means, has this offset been updated/set by a master ?
} IP_PTP_MASTER_INFO;

/*********************************************************************
*
*       IP_PTP_INFO
*
*  Function description
*    Returns information about the current PTP status. The \c{Type}
*    field has to be evaluated for further information about what part
*    of the union is the information to look at.
*/
typedef struct {
  U8 Type;                              // Type of information and what part of the union to look at. The \c{Type} member
                                        // is followed by a union that might not be correctly displayed or completely missing
                                        // in the manual. The following information describes this union part:
                                        // * IP_PTP_INFO_TYPE_CORRECTION    : Information about the latest local time correction can be found in pInfo->Correction in form of IP_PTP_CORRECTION_INFO .
                                        // * IP_PTP_INFO_TYPE_OFFSET        : Information about the offset between slave and master can be found in pInfo->Offset in form of IP_PTP_OFFSET_INFO .
                                        // * IP_PTP_INFO_TYPE_MASTER_CHANGED: Information about a newly selected master can be found in pInfo->Master in form of IP_PTP_MASTER_INFO .
                                        // * IP_PTP_INFO_TYPE_MASTER_UPDATED: Information about parameter updates for the currently selected master can be found in pInfo->Master in form of IP_PTP_MASTER_INFO .
  union {
    IP_PTP_CORRECTION_INFO Correction;
    IP_PTP_OFFSET_INFO     Offset;
    IP_PTP_MASTER_INFO     Master;
  } Data;
} IP_PTP_INFO;


/*********************************************************************
*
*       IP_PTP_ON_INFO_FUNC
*
*  Function description
*    Callback executed whenever updated PTP information is available.
*
*  Parameters
*    IFaceId : Zero-based interface index.
*    pInfo   : Further information of type IP_PTP_INFO about the actual
*              information available.
*/
typedef void (IP_PTP_ON_INFO_FUNC)(unsigned IFaceId, IP_PTP_INFO* pInfo);

int  IP_PTP_GetTime                 (IP_PTP_TIMESTAMP* pPTPTimestamp);
int  IP_PTP_SetTime                 (IP_PTP_TIMESTAMP* pPTPTimestamp);
int  IP_PTP_OC_Start                (unsigned IFaceId);
int  IP_PTP_OC_Halt                 (void);
void IP_PTP_OC_SetProductDescription(const char* pDesc);
void IP_PTP_OC_SetUserDescription   (const char* pDesc);
void IP_PTP_OC_SetRevision          (const char* pDesc);
void IP_PTP_OC_SetInfoCallback      (IP_PTP_ON_INFO_FUNC* pf);

extern const IP_PTP_DRIVER IP_PTP_Driver_Software;

/*********************************************************************
*
*       IP_NTP_
*/
int  IP_NTP_CLIENT_Start             (void);
int  IP_NTP_CLIENT_Halt              (unsigned ClearUserDefined);
int  IP_NTP_CLIENT_Run               (void);
int  IP_NTP_CLIENT_AddServerPool     (unsigned IFaceId, const char* sPool);
void IP_NTP_CLIENT_FavorLocalClock   (unsigned OnOff);
void IP_NTP_CLIENT_AddServerClock    (unsigned IFaceId, U32 IPAddr);
void IP_NTP_CLIENT_AddServerClockIPv6(unsigned IFaceId, U8* pIPAddr);
int  IP_NTP_GetTimestamp             (IP_NTP_TIMESTAMP* pTimestamp);
#ifdef U64
U64  IP_NTP_GetTime                  (int* pStatus);
#endif

/*********************************************************************
*
*       IP_Show_...
*
*  Text output functions informing about the state of various components of the software
*/
int IP_ShowARP       (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowICMP      (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowIGMP      (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowTCP       (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowSocketList(void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowStat      (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowUDP       (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowUDPSockets(void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowDHCPClient(void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowDNS       (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowDNS1      (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowRAW       (void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);
int IP_ShowRAWSockets(void (*pfSendf)(void * pContext, const char * sFormat, ...), void * pContext);

typedef struct {
  int hSock;
  char * pBuffer;
  int BufferSize;
} IP_SENDF_CONTEXT;

void IP_Sendf(void * pContext, const char * sFormat, ...);

/*********************************************************************
*
*       IP_INFO
*/
//
// Buffer information
//
enum {
  IP_INFO_SMALL_BUFFERS_CONFIG = 0,
  IP_INFO_SMALL_BUFFERS_AVAIL,
  IP_INFO_SMALL_BUFFERS_SIZE,
  IP_INFO_SMALL_BUFFERS_USED_MAX,
  IP_INFO_BIG_BUFFERS_CONFIG,
  IP_INFO_BIG_BUFFERS_AVAIL,
  IP_INFO_BIG_BUFFERS_SIZE,
  IP_INFO_BIG_BUFFERS_USED_MAX
};

int  IP_INFO_GetBufferInfo(int InfoIndex);
void IP_INFO_ResetBufferStats(void);

//
// Connection information
//

typedef void * IP_CONNECTION_HANDLE;
typedef struct {
// Socket info
  void *pSock;
  U16   hSock;
  U8    Type;
  U8    Proto;
  U16   Options;
  U16   BackLog;
// Addr/port info
  U32   ForeignAddr;
  U32   LocalAddr;
  U16   ForeignPort;
  U16   LocalPort;
// TCP Info
  U8    TcpState;
  U16   TcpMtu;
  U16   TcpMss;
  U32   TcpRetransDelay;
  U32   TcpIdleTime;
  U32   RxWindowCur;
  U32   RxWindowMax;
  U32   TxWindow;
} IP_CONNECTION;

typedef struct {
  const char * sTypeName;
  U8  AdminState;
  U8  HWState;
  U8  TypeIndex;
  U32 Speed;
} IP_INFO_INTERFACE;


      int    IP_INFO_GetConnectionInfo     (IP_CONNECTION_HANDLE h, IP_CONNECTION * p);
      int    IP_INFO_GetConnectionList     (IP_CONNECTION_HANDLE *pDest, int MaxItems);
const char * IP_INFO_ConnectionState2String(U8 State);
      int    IP_INFO_GetSocketInfo         (int hSock, IP_CONNECTION * p);

      int    IP_INFO_GetNumInterfaces      (void);
      void   IP_INFO_GetInterfaceInfo      (unsigned IFaceId, IP_INFO_INTERFACE * pInterfaceInfo);

/*********************************************************************
*
*       IP_STATS
*/
typedef struct {
  U32 LastLinkStateChange;  // SNMP: ifLastChange [TimeTicks]. Needs to be converted into in 1/100 seconds since SNMP epoch.
  U32 RxBytesCnt;           // SNMP: ifInOctets [Counter].
  U32 RxUnicastCnt;         // SNMP: ifInUcastPkts [Counter].
  U32 RxNotUnicastCnt;      // SNMP: ifInNUcastPkts [Counter].
  U32 RxDiscardCnt;         // SNMP: ifInDiscards [Counter].
  U32 RxErrCnt;             // SNMP: ifInErrors [Counter].
  U32 RxUnknownProtoCnt;    // SNMP: ifInUnknownProtos [Counter].
  U32 TxBytesCnt;           // SNMP: ifOutOctets [Counter].
  U32 TxUnicastCnt;         // SNMP: ifOutUcastPkts [Counter].
  U32 TxNotUnicastCnt;      // SNMP: ifOutNUcastPkts [Counter].
  U32 TxDiscardCnt;         // SNMP: ifOutDiscards [Counter].
  U32 TxErrCnt;             // SNMP: ifOutErrors [Counter].
} IP_STATS_IFACE;

void            IP_STATS_EnableIFaceCounters   (unsigned IFaceId);
IP_STATS_IFACE* IP_STATS_GetIFaceCounters      (unsigned IFaceId);
U32             IP_STATS_GetLastLinkStateChange(unsigned IFaceId);
U32             IP_STATS_GetRxBytesCnt         (unsigned IFaceId);
U32             IP_STATS_GetRxDiscardCnt       (unsigned IFaceId);
U32             IP_STATS_GetRxErrCnt           (unsigned IFaceId);
U32             IP_STATS_GetRxNotUnicastCnt    (unsigned IFaceId);
U32             IP_STATS_GetRxUnicastCnt       (unsigned IFaceId);
U32             IP_STATS_GetRxUnknownProtoCnt  (unsigned IFaceId);
U32             IP_STATS_GetTxBytesCnt         (unsigned IFaceId);
U32             IP_STATS_GetTxDiscardCnt       (unsigned IFaceId);
U32             IP_STATS_GetTxErrCnt           (unsigned IFaceId);
U32             IP_STATS_GetTxNotUnicastCnt    (unsigned IFaceId);
U32             IP_STATS_GetTxUnicastCnt       (unsigned IFaceId);

/*********************************************************************
*
*       Profiling instrumentation functions
*/
void IP_SYSVIEW_Init(void);

/*********************************************************************
*
*       High precision timing functions
*/
#ifdef U64
void IP_SetMicrosecondsCallback(U64 (*pfGetTime_us)(void));
void IP_SetNanosecondsCallback (U64 (*pfGetTime_ns)(void));
#endif

/*********************************************************************
*
*       Compatibility
*
*  Various defines to map obsolete function names to new ones
*/
#define IP_TCP_GetMTU      IP_GetMTU
#define IP_SetDefaultTTL   IP_SetTTL
#define IP_TCP_SetMTU      IP_SetMTU
#define IP_UDP_CONN        IP_UDP_CONN_HANDLE
#define IP_DNSC_SetMaxTTL  IP_DNS_SetMaxTTL


#if defined(__cplusplus)
  }              // Make sure we have C-declarations in C++ programs
#endif

#if IP_SUPPORT_IPV6
#include "IPV6_IPv6.h"
#endif

#endif   // Avoid multiple inclusion

/*************************** End of file ****************************/
