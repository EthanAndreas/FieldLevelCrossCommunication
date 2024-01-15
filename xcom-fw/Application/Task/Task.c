/************************* include section *************************/

#include "Task.h"

/************************* definition section *************************/

// Main TCB and stack
static OS_STACKPTR int mainStack[TASK_STACK_SIZE_MAIN_TASK];
static OS_TASK         mainTCB;

// Mailbox between IP and main task
static OS_MAILBOX IPtoMainMailbox;
static char       IPtoMainBuffer[MAX_MSG * sizeof(MailboxMessage_t)];


int Add_to_queue(MailboxMessage_t *msg)
{
  return OS_MAILBOX_Put(&IPtoMainMailbox, msg);
}

void Get_from_queue(MailboxMessage_t *msg)
{
  OS_MAILBOX_Get(&IPtoMainMailbox, msg);
}

uint32_t Count_msgQueue(void)
{
  return OS_MAILBOX_GetMessageCnt(&IPtoMainMailbox);
}


void Set_taskEvent(OS_TASKEVENT Event)
{
  OS_TASKEVENT_Set(&mainTCB, Event);
}

void Get_taskEvent(OS_TASKEVENT *events)
{
  *events |= OS_TASKEVENT_Get(&mainTCB);
}

void Get_taskEventBlocked(OS_TASKEVENT *events, OS_TASKEVENT eventsToWait)
{
  *events |= OS_TASKEVENT_GetBlocked(eventsToWait);
}

void Clear_all_taskEvent(OS_TASKEVENT *events)
{
  OS_TASKEVENT_Clear(&mainTCB);

  *events = 0;
}

void Clear_one_taskEvent(OS_TASKEVENT *events, OS_TASKEVENT event)
{
  OS_TASKEVENT_ClearEx(&mainTCB, event);

  *events = *events & (~event);
}


void MainTask_Init(void (*Main_routine)(void))
{
  OS_MAILBOX_Create(&IPtoMainMailbox, sizeof(MailboxMessage_t), MAX_MSG, IPtoMainBuffer);
  OS_CREATETASK(&mainTCB, "Main Task", Main_routine, TASKPRIO_MAINTASK, mainStack);
}

/************************* end of file *************************/