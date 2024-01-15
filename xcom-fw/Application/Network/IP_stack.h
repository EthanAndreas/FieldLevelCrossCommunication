#ifndef __IP_STACK_H
#define __IP_STACK_H

/************************* include section *************************/

#include <stdlib.h>

#include "BSP.h"
#include "IP.h"
#include "IP_Webserver.h"
#include "RTOS.h"
#include "Task.h"
#include "WEBS_Conf.h"  // Stack size depends on configuration
#include "XCom_helper.h"
#include "XCom_protocol.h"

/************************** define section ***************************/

// 0: Packets are read in ISR, 1: Packets are read in a task of its own
#define USE_RX_TASK 0

// Callback over UDP
#define UDP_PORT                  3630
#define PACKET_SIZE               256
#define PACKET_TYPE_INDEX         12
#define PACKET_TS_TIMESTAMP_INDEX 14

// Web server over TCP
#define TCP_PORT         80
#define BACK_LOG         20
#define IDLE_TIMEOUT     1000
#define CHILD_ALLOC_SIZE 5120  // numBytes required from memory pool for one connection

/************************** definition section ***************************/

void IP_InitTask(void);

#endif

/************************* end of file *************************/