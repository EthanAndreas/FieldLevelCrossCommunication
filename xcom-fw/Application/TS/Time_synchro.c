/************************* include section *************************/

#include "Time_synchro.h"

/************************* definition section *************************/

OS_MUTEX tsMutex;

static uint64_t            ts[MAX_SENSORS]      = {0};
static Global_ConfSensor_t sensors[MAX_SENSORS] = {0};
static uint64_t            timeToWait           = 0;
static bool_t              started              = false;

void Init_varTS(void)
{
  OS_MUTEX_Create(&tsMutex);
}


void Set_ts()
{
  IP_Log("TS packet received");

  Set_taskEvent(EVENT_TS);
}


void Update_latency(void)
{
  int      i, j, packetReceived;
  uint8_t  boardNb, boardMain;
  uint64_t tempTimer, maxLatency = 0;

  OS_TASKEVENT     events    = 0;
  MailboxMessage_t lastMsg   = {0};
  XCom_Frame_t     lastFrame = {0};

  // Get the number of boards
  boardNb = Get_confSensorNumber();

  // Stop TS if there is no other boards
  if (boardNb <= 1)
    return;

  // Get the index of the main board
  boardMain = Get_confSensorMain();

  // Calculate the average latency on 5 packets
  for (i = 0; i < CYCLE; i++)
  {
    tempTimer = Get_clock();

    // Send cycle packet to every board
    for (j = 0; j < MAX_SENSORS; j++)
    {
      if (j != boardMain && sensors[j].number == boardNb)
      {
        Send_XCom_TS(sensors[j].ip, XCOM_PORT, XCOM_TS_TYPE_CYCLE, 0, 0);
      }
    }

    // Wait to received a packet from each board
    packetReceived = 0;
    while (packetReceived < boardNb - 1)
    {
      Get_taskEventBlocked(&events, EVENTS_INIT_TS);

      // Handle all the packets in the queue
      if (events & EVENT_DATARECEIVED)
      {
        TS_Log("Event : data received");

        // Get the last message
        Get_from_queue(&lastMsg);

        // Decode the message
        XCom_decode(lastMsg.data, &(lastMsg.length), &lastFrame);

        // Check for new events
        Get_taskEvent(&events);

        // Reset EVENT_DATARECEIVED if there is no more message in the queue
        if (Count_msgQueue() == 0)
        {
          Clear_one_taskEvent(&events, EVENT_DATARECEIVED);
        }
      }

      if ((events & EVENT_TS))
      {
        if (lastFrame.TS.type == XCOM_TS_TYPE_CYCLE)
        {
          TS_Log("Cycle packet received");

          // Add the latency of the secondary board
          ts[Get_confSensorIndex(lastMsg.ip)] += (Get_clock() - tempTimer) / 2;
        }

        packetReceived++;
        Clear_one_taskEvent(&events, EVENT_TS);
      }

      XCom_Free(&lastFrame);
    }
  }

  Clear_all_taskEvent(&events);

  // Calculate the average latency
  for (i = 0; i < MAX_SENSORS; i++)
  {
    if (i != boardMain && sensors[i].number == boardNb)
    {
      ts[i] = ts[i] / CYCLE;

      if (maxLatency < ts[i])
      {
        maxLatency = ts[i];
      }
    }
  }

  // Calculate the time to wait before starting measurement
  timeToWait = maxLatency * MULTIPLIER_WAIT_TIME;
}


void Init_tsMain(void)
{
  int     i;
  uint8_t boardNb, boardMain;

  // Get the number of boards
  boardNb = Get_confSensorNumber();

  // Stop TS if there is no other boards
  if (boardNb <= 1)
    return;

  // Get the index of the main board
  boardMain = Get_confSensorMain();

  // Perform the calculation of the latency between each secondary board and main board
  Update_latency();

  // Send the new clock to secondary board
  for (i = 0; i < MAX_SENSORS; i++)
  {
    if (i != boardMain && sensors[i].number == boardNb)
    {
      // Send ts init packet to each board with the new clock and the time to wait for starting measurement
      Send_XCom_TS(sensors[i].ip, XCOM_PORT, XCOM_TS_TYPE_INIT, timeToWait - ts[i], 0);
    }
  }
}

void Init_tsSecondary(void)
{
  uint8_t boardMain;

  OS_TASKEVENT     events    = 0;
  MailboxMessage_t lastMsg   = {0};
  XCom_Frame_t     lastFrame = {0};

  // Get the index of the main board
  boardMain = Get_confSensorMain();

  while (1)
  {
    Get_taskEventBlocked(&events, EVENTS_INIT_TS);

    // handle all the packets in the queue
    if (events & EVENT_DATARECEIVED)
    {
      TS_Log("Event : data received");

      // Get the last message
      Get_from_queue(&lastMsg);

      // Decode the message
      XCom_decode(lastMsg.data, &(lastMsg.length), &lastFrame);

      // Check for new events
      Get_taskEvent(&events);

      // Reset EVENT_DATARECEIVED if there is no more message in the queue
      if (Count_msgQueue() == 0)
      {
        Clear_one_taskEvent(&events, EVENT_DATARECEIVED);
      }
    }

    if ((events & EVENT_TS))
    {
      if (lastFrame.TS.type == XCOM_TS_TYPE_CYCLE)
      {
        TS_Log("TS Cycle packet received");

        // Send back the init packet with own timestamp
        Send_XCom_TS(sensors[boardMain].ip, XCOM_PORT, XCOM_TS_TYPE_CYCLE, 0, 0);

        Clear_one_taskEvent(&events, EVENT_TS);
      }
      else if (lastFrame.TS.type == XCOM_TS_TYPE_INIT)
      {
        TS_Log("TS Init packet received");

        // Retrieve the time to wait before starting measurement
        timeToWait = lastFrame.TS.timestamp;

        Clear_all_taskEvent(&events);
        break;
      }
    }

    XCom_Free(&lastFrame);
  }
}

void TS_Init(void)
{
  int      i, j = 0;
  uint8_t  boardMain;
  uint64_t period;

  uint32_t ip[MAX_SENSORS - 1] = {0};

  // Get the index of the main board
  boardMain = Get_confSensorMain();

  // Get the time cycle entered by the user in ms
  period = OS_TIME_Convertms2Ticks(Get_confCyclePeriodInMS());

  // Get the conf sensors
  for (i = 0; i < MAX_SENSORS; i++)
  {
    sensors[i] = Get_confSensor(i);
  }

  // Enable clock
  Init_clock();

  if (Get_isMain() == true)
  {
    // if the board already started, stop previous timer
    if (started == true)
    {
      Stop_latencyTimer();
      Stop_calcTimer();
      Stop_measTimer();
      Stop_tsTimer();
    }

    Init_tsMain();

    // Retrieve ip address of secondary boards
    for (i = 0; i < MAX_SENSORS; i++)
    {
      if (i != boardMain && sensors[i].number != 0 && j < MAX_SENSORS - 1)
      {
        ip[j] = sensors[i].ip;
        j++;
      }
    }

    // Enable latency timer
    Init_latencyTimer(period, ip[0], ip[1]);

    // Enable calculation timer
    Init_calcTimer(period);

    // Enable measurement timer
    Init_measTimer(timeToWait, period);

    // Enable TS timer 
    Init_tsTimer();
  }
  else
  {
    // if the board already started, stop previous timer
    if (started == true)
    {
      Stop_measTimer();
    }

    Init_tsSecondary();

    // Enable measurement timer
    Init_measTimer(timeToWait, period);
  }

  Stop_clock();

  started = true;
  End_timerInit();
}

/************************* end of file *************************/