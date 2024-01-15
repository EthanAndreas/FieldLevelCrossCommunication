/************************* include section *************************/

#include "XCom_init.h"

/************************* definition section *************************/

static uint64_t msgID = 0;

int XCom_Init_TS(XCom_Frame_t *frame, uint8_t type, uint64_t timestamp, uint32_t offset)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_TS_SIZE;
  frame->type   = XCOM_MSG_TYPE_TS;

  // ts frame
  frame->TS.type      = type;
  frame->TS.timestamp = timestamp;
  frame->TS.offset    = offset;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Send_Tank(XCom_Frame_t *frame, uint32_t diameterValue, uint8_t diameterUnit, uint32_t volumeValue, uint8_t volumeUnit, uint32_t heigthValue, uint8_t heigthUnit)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_SEND_TANK_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf tank frame
  frame->Conf.type                = XCOM_CONF_TYPE_SEND_TANK_INFO;
  frame->Conf.Tank.diameter.value = diameterValue;
  frame->Conf.Tank.diameter.unit  = diameterUnit;
  frame->Conf.Tank.volume.value   = volumeValue;
  frame->Conf.Tank.volume.unit    = volumeUnit;
  frame->Conf.Tank.heigth.value   = heigthValue;
  frame->Conf.Tank.heigth.unit    = heigthUnit;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Get_Tank(XCom_Frame_t *frame)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_GET_TANK_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf tank frame
  frame->Conf.type = XCOM_CONF_TYPE_GET_TANK_INFO;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Send_Sensor(XCom_Frame_t *frame, uint8_t number, uint32_t ip, uint8_t role, uint8_t type, uint32_t heigthValue, uint8_t heigthUnit)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_SEND_SENSOR_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf sensor frame
  frame->Conf.type                = XCOM_CONF_TYPE_SEND_SENSOR_INFO;
  frame->Conf.Sensor.number       = number;
  frame->Conf.Sensor.ip           = ip;
  frame->Conf.Sensor.role         = role;
  frame->Conf.Sensor.type        = type;
  frame->Conf.Sensor.heigth.value = heigthValue;
  frame->Conf.Sensor.heigth.unit  = heigthUnit;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Get_Sensor(XCom_Frame_t *frame)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_GET_SENSOR_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf sensor frame
  frame->Conf.type = XCOM_CONF_TYPE_GET_SENSOR_INFO;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Send_Fluid(XCom_Frame_t *frame, uint32_t densityValue, uint8_t densityUnit, uint32_t gasPressureValue, uint8_t gasPressureUnit)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_SEND_FLUID_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf fluid frame
  frame->Conf.type                    = XCOM_CONF_TYPE_SEND_FLUID_INFO;
  frame->Conf.Fluid.density.value     = densityValue;
  frame->Conf.Fluid.density.unit      = densityUnit;
  frame->Conf.Fluid.gasPressure.value = gasPressureValue;
  frame->Conf.Fluid.gasPressure.unit  = gasPressureUnit;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Get_Fluid(XCom_Frame_t *frame)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_GET_FLUID_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf fluid frame
  frame->Conf.type = XCOM_CONF_TYPE_GET_FLUID_INFO;

  return EXIT_SUCCESS;
}

int XCom_Init_Conf_Cycle(XCom_Frame_t *frame, uint32_t timeValue, uint8_t timeUnit, uint8_t frameLoss)
{
  // frame header
  frame->id     = msgID++;
  frame->length = XCOM_CONF_CYCLE_SIZE;
  frame->type   = XCOM_MSG_TYPE_CONF;

  // conf cycle frame
  frame->Conf.type             = XCOM_CONF_TYPE_CYCLE_INFO;
  frame->Conf.Cycle.time.value = timeValue;
  frame->Conf.Cycle.time.unit  = timeUnit;
  frame->Conf.Cycle.frameLoss  = frameLoss;

  return EXIT_SUCCESS;
}

int XCom_Init_Pressure(XCom_Frame_t *frame, uint8_t type, uint32_t pressureValue, uint8_t pressureUnit, uint8_t qualityCode)
{
  switch (type)
  {
    case XCOM_SEND_PRESSURE:

      // frame header
      frame->id     = msgID++;
      frame->length = XCOM_SEND_PRESSURE_SIZE;
      frame->type   = XCOM_MSG_TYPE_PRESSURE;

      // pressure frame
      frame->Pressure.type        = type;
      frame->Pressure.value       = pressureValue;
      frame->Pressure.unit        = pressureUnit;
      frame->Pressure.qualityCode = qualityCode;

      break;

    case XCOM_GET_PRESSURE:

      // frame header
      frame->id     = msgID++;
      frame->length = XCOM_GET_PRESSURE_SIZE;
      frame->type   = XCOM_MSG_TYPE_PRESSURE;

      // pressure frame
      frame->Pressure.type = type;

      break;

    default:
      panic("Pressure type not recognized.");
      free(frame);
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int XCom_Init_Level(XCom_Frame_t *frame, uint8_t type, uint32_t levelValue, uint8_t levelUnit)
{
  switch (type)
  {
    case XCOM_SEND_LEVEL:

      // frame header
      frame->id     = msgID++;
      frame->length = XCOM_SEND_LEVEL_SIZE;
      frame->type   = XCOM_MSG_TYPE_LEVEL;

      // level frame
      frame->Level.type  = type;
      frame->Level.value = levelValue;
      frame->Level.unit  = levelUnit;

      break;

    case XCOM_GET_LEVEL:

      // frame header
      frame->id     = msgID++;
      frame->length = XCOM_GET_LEVEL_SIZE;
      frame->type   = XCOM_MSG_TYPE_LEVEL;

      // level frame
      frame->Level.type = type;

      break;

    default:
      panic("Level type not recognized.");
      free(frame);
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/************************* end of file *************************/