/************************* include section *************************/

#include "Configuration.h"

/************************* definition section *************************/

OS_MUTEX confMutex;

static bool_t isMain = false;

static bool_t isSetConf = false;

static int nbConfSensorRecv = 0;

static Global_ConfTank_t   Tank                 = {0};
static Global_ConfSensor_t Sensors[MAX_SENSORS] = {0};
static Global_ConfFluid_t  Fluid                = {0};
static Global_ConfCycle_t  Cycle                = {0};

static uint32_t monitoringAppIP = 0;

void Init_varConf(void)
{
  OS_MUTEX_Create(&confMutex);
}


void Set_isMain(void)
{
  isMain = true;
}

bool_t Get_isMain(void)
{
  return isMain;
}


void Set_isSetConf(void)
{
  isSetConf = true;
}

bool_t Get_isSetConf(void)
{
  return isSetConf;
}

void Set_confTank(uint32_t diameterValue, uint8_t diameterUnit,
                  uint32_t volumeValue, uint8_t volumeUnit, uint32_t heightValue,
                  uint8_t heightUnit)
{
  CONF_Log("Conf tank packet received");

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  Tank.diameterValue = diameterValue;
  Tank.diameterUnit  = diameterUnit;
  Tank.volumeValue   = volumeValue;
  Tank.volumeUnit    = volumeUnit;
  Tank.heightValue   = heightValue;
  Tank.heightUnit    = heightUnit;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  Set_taskEvent(EVENT_CONFTANK);
}

Global_ConfTank_t Get_confTank(void)
{
  Global_ConfTank_t tank = {0};

  OS_MUTEX_LockBlocked(&confMutex);

  tank = Tank;

  OS_MUTEX_Unlock(&confMutex);

  return tank;
}


void Set_confSensor(uint8_t number, uint32_t ip, uint8_t role,
                    uint8_t type, uint32_t heightValue, uint8_t heightUnit)
{
  CONF_Log("Conf sensor packet received");

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  ((Sensors)[role]).number      = number;
  ((Sensors)[role]).ip          = ip;
  ((Sensors)[role]).role        = role;
  ((Sensors)[role]).type        = type;
  ((Sensors)[role]).heightValue = heightValue;
  ((Sensors)[role]).heightUnit  = heightUnit;

  nbConfSensorRecv++;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  if (nbConfSensorRecv == number)
  {
    Set_taskEvent(EVENT_CONFSENSOR);
    nbConfSensorRecv = 0;
  }
}

void Set_confSensorLessOne(uint32_t ip)
{
  CONF_Log("One connection loss");

  int     i;
  uint8_t index = Get_confSensorIndex(ip);

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  Sensors[index].number      = 0;
  Sensors[index].ip          = 0;
  Sensors[index].role        = 0;
  Sensors[index].type        = 0;
  Sensors[index].heightValue = 0;
  Sensors[index].heightUnit  = 0;

  for (i = 0; i < MAX_SENSORS; i++)
  {
    if (Sensors[i].number > 0)
    {
      Sensors[i].number--;
    }
  }

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));
}

Global_ConfSensor_t Get_confSensor(int i)
{
  Global_ConfSensor_t Sensor = {0};

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  Sensor = Sensors[i];

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return Sensor;
}

uint8_t Get_confSensorIndex(uint32_t ip)
{
  int     i;
  uint8_t index = NOT_A_SENSOR;

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  for (i = 0; i < MAX_SENSORS; i++)
  {
    if ((Sensors)[i].ip == ip)
    {
      index = ((Sensors)[i].role);
      break;
    }
  }

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return index;
}

uint8_t Get_confSensorNumber(void)
{
  int i;

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  for (i = 0; i < MAX_SENSORS; i++)
  {
    if (Sensors[i].number != 0)
    {
      break;
    }
  }

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return Sensors[i].number;
}

uint8_t Get_confSensorMain(void)
{
  int i;

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  for (i = 0; i < MAX_SENSORS; i++)
  {
    if (Sensors[i].type == TYPE_MAIN)
    {
      break;
    }
  }

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return i;
}

uint32_t Get_confSensorMainIP(void)
{
  uint32_t ip;

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  ip = Sensors[Get_confSensorMain()].ip;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return ip;
}

void Set_confFluid(uint32_t densityValue, uint8_t densityUnit,
                   uint32_t gasPressureValue, uint8_t gasPressureUnit)
{
  CONF_Log("Conf fluid packet received");

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  Fluid.densityValue     = densityValue;
  Fluid.densityUnit      = densityUnit;
  Fluid.gasPressureValue = gasPressureValue;
  Fluid.gasPressureUnit  = gasPressureUnit;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  Set_taskEvent(EVENT_CONFFLUID);
}

Global_ConfFluid_t Get_confFluid(void)
{
  Global_ConfFluid_t fluid = {0};

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  fluid = Fluid;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return fluid;
}


void Set_confCycle(uint32_t timeValue, uint8_t timeUnit,
                   uint8_t frameLoss)
{
  CONF_Log("Conf cycle packet received");

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  Cycle.timeValue = timeValue;
  Cycle.timeUnit  = timeUnit;
  Cycle.frameLoss = frameLoss;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  Set_taskEvent(EVENT_CONFCYCLE);
}

Global_ConfCycle_t Get_confCycle(void)
{
  Global_ConfCycle_t cycle = {0};

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  cycle = Cycle;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return cycle;
}

uint32_t Get_confCyclePeriodInMS(void)
{
  uint32_t period;

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  switch (Cycle.timeUnit)
  {
    case TIME_UNIT_MICROSECOND:
      period = Cycle.timeValue * 1000;

    case TIME_UNIT_MILLISECOND:
      period = Cycle.timeValue;

    case TIME_UNIT_SECOND:
      period = Cycle.timeValue / 1000;

    default:
      period = Cycle.timeValue;
  }

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return period;
}


void Set_monitoringAppIP(uint32_t ip)
{
  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  monitoringAppIP = ip;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));
}

uint32_t Get_monitoringAppIP(void)
{
  uint32_t ip = 0;

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&confMutex));

  ip = monitoringAppIP;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&confMutex));

  return ip;
}


void Conf_Init(void)
{
  OS_TASKEVENT     events    = 0;
  MailboxMessage_t lastMsg   = {0};
  XCom_Frame_t     lastFrame = {0};

  // Wait for configuration
  while (true)
  {
    Get_taskEventBlocked(&events, EVENTS_INIT_CONF);

    // Handle all the packets in the queue
    if (events & EVENT_DATARECEIVED)
    {
      CONF_Log("Event : data received");

      // Get the last message
      Get_from_queue(&lastMsg);

      // Decode the last message
      XCom_decode(lastMsg.data, &(lastMsg.length), &lastFrame);

      // Check for new events
      Get_taskEvent(&events);

      if (Count_msgQueue() == 0)
      {
        Clear_one_taskEvent(&events, EVENT_DATARECEIVED);
      }
    }

    if ((events & EVENT_USERSETCONF) ||
        (events & EVENT_CONFTANK &&
         events & EVENT_CONFSENSOR &&
         events & EVENT_CONFFLUID &&
         events & EVENT_CONFCYCLE))
    {
      CONF_Log("Event : configuration completed");

      Clear_all_taskEvent(&events);
      break;
    }

    XCom_Free(&lastFrame);
  }
}


/************************* end of file *************************/