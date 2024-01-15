#ifndef MAIN_H
#define MAIN_H

/************************* include section *************************/

#include "BSP.h"
#include "IP_stack.h"
#include "Internal.h"
#include "RTOS.h"
#include "Task.h"
#include "UDP_send.h"
#include "XCom_decode.h"

/************************* define section *************************/

#define MAIN_Log IP_Log

/************************** definition section ***************************/

void Main_routine(void)
{
  uint8_t index;

  OS_TASKEVENT     events = 0;
  MailboxMessage_t lastMsg;
  XCom_Frame_t     lastFrame;
  uint32_t         monitoringIPaddr;

  /***** Initialization *****/
  BSP_ToggleLED(1);

  Variables_Init();
  Callbacks_Init();
  IP_InitTask();
  Conf_Init();
  TS_Init();

  /***** Programm *****/
  BSP_ToggleLED(2);

  while (true)
  {
    Get_taskEventBlocked(&events, EVENTS_MAIN);

    // Handle all the packets in the queue
    if (events & EVENT_DATARECEIVED)
    {
      MAIN_Log("Event : data received");

      // Get the last message
      Get_from_queue(&lastMsg);

      // Decode the last message
      XCom_decode(lastMsg.data, &(lastMsg.length), &lastFrame);

      // Check for new events
      Get_taskEvent(&events);

      // Handle packet of get type
      GetPacket_handler(lastMsg.ip, lastMsg.fport, lastFrame);

      // Reset EVENT_DATARECEIVED if there is no more message in the queue
      if (Count_msgQueue() == 0)
      {
        Clear_one_taskEvent(&events, EVENT_DATARECEIVED);
      }
    }

    if (events & EVENT_TS)
    {
      MAIN_Log("Event : ts re-init");

      // if secondary board, send back the cycle packet received
      if (Get_isMain() == false)
      {
        Send_XCom_TS(Get_confSensorMainIP(), XCOM_PORT, XCOM_TS_TYPE_CYCLE, 0, 0);
      }

      TS_Init();

      Clear_one_taskEvent(&events, EVENT_TS);
    }

    if (events & EVENT_PRESSURE)
    {
      index            = Get_confSensorIndex(lastMsg.ip);
      monitoringIPaddr = Get_monitoringAppIP();

      // if a board, store the pressure received
      if (index != NOT_A_SENSOR)
      {
        Set_pressure(index, lastFrame.Pressure.value, lastFrame.Pressure.unit,
                     lastFrame.Pressure.qualityCode);

        Restart_latencyOneTimer(lastMsg.ip);
      }

      // if the monitoring application, simulate the pressure
      else if (monitoringIPaddr == lastMsg.ip)
      {
        MAIN_Log("Event : set simulate pressure");

        Set_currentPressure(lastFrame.Pressure.value,
                            lastFrame.Pressure.unit, QUALITY_CODE_SIMULATE);
      }
    }

    if (events & EVENT_MEASUREMENT)
    {
      MAIN_Log("Event : pressure measurement");

      Measurement();

      Clear_one_taskEvent(&events, EVENT_MEASUREMENT);
    }

    if (events & EVENT_CALCULATION)
    {
      MAIN_Log("Event : level calculation");

      Calculation();

      Restart_latencyAllTimer();

      Clear_one_taskEvent(&events, EVENT_CALCULATION);
    }
  }
}


int main(void)
{
  OS_InitKern();
  OS_InitHW();
  BSP_Init();
  MainTask_Init(Main_routine);

  OS_Start();

  return EXIT_SUCCESS;
}

#endif

/************************* end of file *************************/