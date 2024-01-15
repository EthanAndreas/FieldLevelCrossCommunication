/************************* include section *************************/

#include "Internal.h"

/************************* definition section *************************/

static int getReceived = 0;


void Variables_Init(void)
{
  Init_varConf();
  Init_varLevel();
  Init_varPressure();
  Init_varTS();
}

void Callbacks_Init(void)
{
  XCom_callbacks cbs;

  cbs.setTS = Set_ts;

  cbs.setConfTank   = Set_confTank;
  cbs.setConfSensor = Set_confSensor;
  cbs.setConfFluid  = Set_confFluid;
  cbs.setConfCycle  = Set_confCycle;

  cbs.setPressure = Set_eventPressure;

  cbs.setLevel = Set_eventLevel;

  XCom_setCallbacks(cbs);
}


void GetPacket_handler(uint32_t ip, uint16_t fport, XCom_Frame_t frame)
{
  int i;

  Global_ConfTank_t   tank     = {0};
  Global_ConfSensor_t sensor   = {0};
  Global_ConfFluid_t  fluid    = {0};
  Global_Pressure_t   pressure = {0};
  Global_Level_t      level    = {0};

  switch (frame.type)
  {
    case XCOM_MSG_TYPE_CONF:
      switch (frame.Conf.type)
      {
        case XCOM_CONF_TYPE_GET_TANK_INFO:
          IP_Log("Conf tank asked");

          tank = Get_confTank();
          Send_XCom_Conf_Send_Tank(ip, fport, tank.diameterValue,
                                   tank.diameterUnit, tank.volumeValue, tank.volumeUnit,
                                   tank.heightValue, tank.heightUnit);

          break;

        case XCOM_CONF_TYPE_GET_SENSOR_INFO:
          IP_Log("Conf sensor asked");

          sensor = Get_confSensor(getReceived);

          Send_XCom_Conf_Send_Sensor(ip, fport, sensor.number, sensor.ip,
                                     sensor.role, sensor.type, sensor.heightValue, sensor.heightUnit);

          getReceived++;

          if (getReceived >= MAX_SENSORS - 1)
          {
            getReceived = 0;
          }

          break;

        case XCOM_CONF_TYPE_GET_FLUID_INFO:
          IP_Log("Conf fluid asked");

          fluid = Get_confFluid();
          Send_XCom_Conf_Send_Fluid(ip, fport, fluid.densityValue,
                                    fluid.densityUnit, fluid.gasPressureValue,
                                    fluid.gasPressureUnit);

          break;
      }
      break;

    case XCOM_MSG_TYPE_PRESSURE:
      if (frame.Pressure.type == XCOM_GET_PRESSURE)
      {
        IP_Log("Pressure asked");

        pressure = Get_currentPressure();
        Send_XCom_Pressure(ip, fport, XCOM_SEND_PRESSURE, pressure.value,
                           pressure.unit, pressure.qualityCode);
      }
      break;

    case XCOM_MSG_TYPE_LEVEL:
      if (frame.Level.type == XCOM_GET_LEVEL)
      {
        IP_Log("Level asked");

        level = Get_level();
        Send_XCom_Level(ip, fport, XCOM_SEND_LEVEL, level.value, level.unit);
      }

      break;
  }
}

/************************* end of file *************************/