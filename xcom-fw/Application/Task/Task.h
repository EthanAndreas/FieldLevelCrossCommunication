#ifndef __TASK_H
#define __TASK_H

/************************* include section *************************/

#include <stdint.h>

#include "IP.h"
#include "RTOS.h"

/************************* define section *************************/

// Main constant
#define TASK_STACK_SIZE_MAIN_TASK 2048

// Webserver constant
#define MAX_CONNECTIONS          2
#define TASK_STACK_SIZE_WEB_TASK 2304

// Mailbox constant
#define MAX_MSG_SIZE 256
#define MAX_MSG      64

// Events
#define EVENT_DATARECEIVED (1 << 0)
#define EVENT_USERSETCONF  (1 << 1)
#define EVENT_TS           (1 << 2)
#define EVENT_CONFTANK     (1 << 3)
#define EVENT_CONFSENSOR   (1 << 4)
#define EVENT_CONFFLUID    (1 << 5)
#define EVENT_CONFCYCLE    (1 << 6)
#define EVENT_PRESSURE     (1 << 7)
#define EVENT_MEASUREMENT  (1 << 8)
#define EVENT_CALCULATION  (1 << 9)

#define EVENTS_INIT_CONF (EVENT_DATARECEIVED | \
                          EVENT_USERSETCONF |  \
                          EVENT_CONFTANK | \ 
                          EVENT_CONFSENSOR |   \
                          EVENT_CONFFLUID |    \
                          EVENT_CONFCYCLE)

#define EVENTS_INIT_TS (EVENT_DATARECEIVED | \
                        EVENT_TS)

#define EVENTS_MAIN (EVENT_DATARECEIVED | \
                     EVENT_TS |           \
                     EVENT_PRESSURE |     \
                     EVENT_MEASUREMENT |  \
                     EVENT_CALCULATION)


#define CHK_INTERRUPT(op)                                           \
  do                                                                \
  {                                                                 \
    if (OS_TIMER_GetCurrent() == NULL && OS_INT_InInterrupt() == 0) \
    {                                                               \
      op;                                                           \
    }                                                               \
  } while (0)

/************************** definition section ***************************/

// Task priorities
enum
{
  TASKPRIO_MAINTASK = 100,  // 100
  TASKPRIO_WEBSCHILD,       // 101
  TASKPRIO_WEBSPARENT,      // 102
  TASKPRIO_IP,              // 103
};

// Mailbox between Main and IP task
typedef struct
{
  uint32_t ip;
  uint16_t fport;
  uint32_t length;
  uint8_t *data;
} MailboxMessage_t;


int      Add_to_queue(MailboxMessage_t *msg);
void     Get_from_queue(MailboxMessage_t *msg);
uint32_t Count_msgQueue(void);

void Set_taskEvent(OS_TASKEVENT Event);
void Get_taskEvent(OS_TASKEVENT *events);
void Get_taskEventBlocked(OS_TASKEVENT *events, OS_TASKEVENT eventsToWait);
void Clear_all_taskEvent(OS_TASKEVENT *events);
void Clear_one_taskEvent(OS_TASKEVENT *events, OS_TASKEVENT event);

void MainTask_Init(void (*Main_routine)(void));

#endif

/************************* end of file *************************/
