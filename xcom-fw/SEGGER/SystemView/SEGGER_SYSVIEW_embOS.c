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

File    : SEGGER_SYSVIEW_embOS.c
Purpose : Interface between embOS and System View.
Revision: $Rev: 22295 $
*/

#include "RTOS.h"
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW_embOS.h"

#if (OS_VERSION < 41201)
  #error "SystemView is only supported in embOS V4.12a and later."
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbSendTaskInfo()
*
*  Function description
*    Sends task information to SystemView
*/
static void _cbSendTaskInfo(const OS_TASK* pTask) {
  SEGGER_SYSVIEW_TASKINFO Info;

  OS_EnterRegion();                // No scheduling to make sure the task list does not change while we are transmitting it
  memset(&Info, 0, sizeof(Info));  // Fill all elements with 0 to allow extending the structure in future version without breaking the code
  Info.TaskID = (U32)pTask;
#if OS_TRACKNAME
  Info.sName = pTask->Name;
#endif
  Info.Prio = pTask->Priority;
#if OS_CHECKSTACK
  Info.StackBase = (U32)pTask->pStackBot;
  Info.StackSize = pTask->StackSize;
#endif
  SEGGER_SYSVIEW_SendTaskInfo(&Info);
  OS_LeaveRegion();                // No scheduling to make sure the task list does not change while we are transmitting it
}

/*********************************************************************
*
*       _cbSendTaskList()
*
*  Function description
*    This function is part of the link between embOS and SYSVIEW.
*    Called from SystemView when asked by the host, it uses SYSVIEW
*    functions to send the entire task list to the host.
*/
static void _cbSendTaskList(void) {
  OS_TASK*    pTask;

  OS_EnterRegion();         // No scheduling to make sure the task list does not change while we are transmitting it
  for (pTask = OS_Global.pTask; pTask; pTask = pTask->pNext) {
    _cbSendTaskInfo(pTask);
  }
#if ((OS_VERSION >= 43800) && (OS_TRACKNAME != 0))  // Human readable object identifiers supported since embOS V4.38
  {
    OS_OBJNAME* p;
    for (p = OS_pObjNameRoot; p != NULL; p = p->pNext) {
      SEGGER_SYSVIEW_NameResource((OS_U32)p->pOSObjID, p->sName);
    }
  }
#endif
  OS_LeaveRegion();         // No scheduling to make sure the task list does not change while we are transmitting it
}

/*********************************************************************
*
*       _cbOnTaskCreate()
*
*  Function description
*    For embOS prior to V4.32 the cast to OS_U32 is necessary
*/
#if (OS_VERSION < 43200)  // Work around different embOS Trace API function types prior to V4.32
static void _cbOnTaskCreate(unsigned int TaskId) {
  SEGGER_SYSVIEW_OnTaskCreate((OS_U32)TaskId);
}
#else
#define _cbOnTaskCreate   SEGGER_SYSVIEW_OnTaskCreate
#endif

/*********************************************************************
*
*       _cbOnTaskStartExec()
*
*  Function description
*    For embOS prior to V4.32 the cast to OS_U32 is necessary
*/
#if (OS_VERSION < 43200)  // Work around different embOS Trace API function types prior to V4.32
static void _cbOnTaskStartExec(unsigned int TaskId) {
  SEGGER_SYSVIEW_OnTaskStartExec((OS_U32)TaskId);
}
#else
#define _cbOnTaskStartExec  SEGGER_SYSVIEW_OnTaskStartExec
#endif

/*********************************************************************
*
*       _cbOnTaskStartReady()
*
*  Function description
*    For embOS prior to V4.32 the cast to OS_U32 is necessary
*/
#if (OS_VERSION < 43200)  // Work around different embOS Trace API function types prior to V4.32
static void _cbOnTaskStartReady(unsigned int TaskId) {
  SEGGER_SYSVIEW_OnTaskStartReady((OS_U32)TaskId);
}
#else
#define _cbOnTaskStartReady SEGGER_SYSVIEW_OnTaskStartReady
#endif

/*********************************************************************
*
*       _cbOnTaskStopReady()
*
*  Function description
*    For embOS prior to V4.32 the cast to OS_U32 is necessary
*/
#if (OS_VERSION < 43200)  // Work around different embOS Trace API function types prior to V4.32
static void _cbOnTaskStopReady(unsigned int TaskId, unsigned int Reason) {
  SEGGER_SYSVIEW_OnTaskStopReady((OS_U32)TaskId, Reason);
}
#else
#define _cbOnTaskStopReady  SEGGER_SYSVIEW_OnTaskStopReady
#endif

/*********************************************************************
*
*       _cbOnTaskTerminate()
*
*  Function description
*    For embOS prior to V4.32 the cast to OS_U32 is necessary
*/
#if (OS_VERSION < 43200)  // Work around different embOS Trace API function types prior to V4.32
static void _cbOnTaskTerminate(unsigned int TaskId) {
  SEGGER_SYSVIEW_OnTaskTerminate((OS_U32)TaskId);
}
#else
#define _cbOnTaskTerminate  SEGGER_SYSVIEW_OnTaskTerminate
#endif

//
// embOS trace API that targets SYSVIEW
//
const OS_TRACE_API embOS_TraceAPI_SYSVIEW = {
  //
  // Specific Trace Events
  //
  SEGGER_SYSVIEW_RecordEnterISR,                //  void (*pfRecordEnterISR)              (void);
  SEGGER_SYSVIEW_RecordExitISR,                 //  void (*pfRecordExitISR)               (void);
  SEGGER_SYSVIEW_RecordExitISRToScheduler,      //  void (*pfRecordExitISRToScheduler)    (void);
  _cbSendTaskInfo,                              //  void (*pfRecordTaskInfo)              (const OS_TASK* pTask);
  _cbOnTaskCreate,                              //  void (*pfRecordTaskCreate)            (OS_U32 TaskId);
  _cbOnTaskStartExec,                           //  void (*pfRecordTaskStartExec)         (OS_U32 TaskId);
  SEGGER_SYSVIEW_OnTaskStopExec,                //  void (*pfRecordTaskStopExec)          (void);
  _cbOnTaskStartReady,                          //  void (*pfRecordTaskStartReady)        (OS_U32 TaskId);
  _cbOnTaskStopReady,                           //  void (*pfRecordTaskStopReady)         (OS_U32 TaskId, unsigned Reason);
  SEGGER_SYSVIEW_OnIdle,                        //  void (*pfRecordIdle)                  (void);
  //
  // Generic Trace Event logging
  //
  SEGGER_SYSVIEW_RecordVoid,                    //  void    (*pfRecordVoid)               (unsigned Id);
  SEGGER_SYSVIEW_RecordU32,                     //  void    (*pfRecordU32)                (unsigned Id, OS_U32 Para0);
  SEGGER_SYSVIEW_RecordU32x2,                   //  void    (*pfRecordU32x2)              (unsigned Id, OS_U32 Para0, OS_U32 Para1);
  SEGGER_SYSVIEW_RecordU32x3,                   //  void    (*pfRecordU32x3)              (unsigned Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2);
  SEGGER_SYSVIEW_RecordU32x4,                   //  void    (*pfRecordU32x4)              (unsigned Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2, OS_U32 Para3);
  SEGGER_SYSVIEW_ShrinkId,                      //  OS_U32  (*pfPtrToId)                  (OS_U32 Ptr);
#if (OS_VERSION >= 41400)  // Tracing timer is supported since embOS V4.14
  SEGGER_SYSVIEW_RecordEnterTimer,              //  void    (*pfRecordEnterTimer)         (OS_U32 TimerID);
  SEGGER_SYSVIEW_RecordExitTimer,               //  void    (*pfRecordExitTimer)          (void);
#endif
#if (OS_VERSION >= 42400)   // Tracing end of call supported since embOS V4.24
  SEGGER_SYSVIEW_RecordEndCall,                 //  void    (*pfRecordEndCall)            (unsigned int Id);
  SEGGER_SYSVIEW_RecordEndCallU32,              //  void    (*pfRecordEndCallReturnValue) (unsigned int Id, OS_U32 ReturnValue);
  _cbOnTaskTerminate,                           //  void    (*pfRecordTaskTerminate)      (OS_U32 TaskId);
  SEGGER_SYSVIEW_RecordU32x5,                   //  void    (*pfRecordU32x5)              (unsigned Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2, OS_U32 Para3, OS_U32 Para4);
#endif
#if (OS_VERSION >= 43800)  // Human readable object identifiers supported since embOS V4.38
  SEGGER_SYSVIEW_NameResource,                  // void  (*pfRecordObjName)               (OS_U32 Id, OS_CONST_PTR char* Para0);
#endif
};

//
// Services provided to SYSVIEW by embOS
//
const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI = {
  OS_GetTime_us64,
  _cbSendTaskList,
};

/*************************** End of file ****************************/
