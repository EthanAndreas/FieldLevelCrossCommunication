/************************* include section *************************/

#include "Measurement.h"

/************************* definition section *************************/

OS_MUTEX pressureMutex;

static Global_Pressure_t pressures[MAX_SENSORS] = {0};


void Init_varPressure(void)
{
  OS_MUTEX_Create(&pressureMutex);
}

void Set_eventPressure(void)
{
  Set_taskEvent(EVENT_PRESSURE);
}

void Set_pressure(uint8_t sensorID, uint32_t value, uint8_t unit,
                  uint8_t qualityCode)
{
  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&pressureMutex));

  (pressures[sensorID]).value       = value;
  (pressures[sensorID]).unit        = unit;
  (pressures[sensorID]).qualityCode = qualityCode;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&pressureMutex));
}

void Set_currentPressure(uint32_t value, uint8_t unit,
                         uint8_t qualityCode)
{
  uint8_t index;

  index = Get_confSensorIndex(htonl(IP_GetIPAddr(0)));

  Set_pressure(index, value, unit, qualityCode);
}

Global_Pressure_t Get_pressure(uint8_t index)
{
  Global_Pressure_t copyPressure = {0};

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&pressureMutex));

  copyPressure = pressures[index];

  CHK_INTERRUPT(OS_MUTEX_Unlock(&pressureMutex));

  return copyPressure;
}

Global_Pressure_t Get_currentPressure(void)
{
  uint8_t           index;
  Global_Pressure_t copyPressure = {0};

  index = Get_confSensorIndex(htonl(IP_GetIPAddr(0)));

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&pressureMutex));

  copyPressure = pressures[index];

  CHK_INTERRUPT(OS_MUTEX_Unlock(&pressureMutex));

  return copyPressure;
}

uint32_t Convert_pressure(uint32_t value, uint8_t unit)
{
  // Always convert values into Pascal to calculate the level with the same unit
  switch (unit)
  {
    case PRESSURE_UNIT_BAR:
      value *= BAR_TO_PA;
      break;

    case PRESSURE_UNIT_PSI:
      value *= PSI_TO_PA;
      break;

    case PRESSURE_UNIT_PASCAL:
      break;

    default:
      break;
  }

  return value;
}


void Measurement(void)
{
  uint8_t  boardIndex;
  uint32_t boardMainIP, pressure = 0;

  // Fill values needed for the pressure measurement
  boardIndex  = Get_confSensorIndex(htonl(IP_GetIPAddr(0)));
  boardMainIP = Get_confSensor(Get_confSensorMain()).ip;

  // Randomize pressure value
  if (boardIndex == ROLE_HIGH)
  {
    pressure = 13000;
  }
  else if (boardIndex == ROLE_DENSITY)
  {
    pressure = 12000;
  }
  else if (boardIndex == ROLE_LOW)
  {
    pressure = 11000;
  }

  // Retrieve the presure value from the sensor or simulated value
  Set_currentPressure(pressure, PRESSURE_UNIT_PASCAL, QUALITY_CODE_REEL);

  // If the board is secondary, send the pressure to the main board
  if (Get_isMain() == false)
  {
    Send_XCom_Pressure(boardMainIP, XCOM_PORT, XCOM_SEND_PRESSURE, pressure, PRESSURE_UNIT_PASCAL, QUALITY_CODE_REEL);
  }
}

/************************* end of file *************************/