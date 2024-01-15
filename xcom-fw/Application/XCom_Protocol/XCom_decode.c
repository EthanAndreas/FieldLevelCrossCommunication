/************************* include section *************************/

#include "XCom_decode.h"

/************************* definition section *************************/

int XCom_decode_TS(uint8_t *data, size_t *length, XCom_TS_t *ts)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(ts);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_TS_SIZE, "Payload is too small for XCom TS.");

  // type
  ts->type = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\tTS type : %d\n", ts->type);

  // timestamp
  ts->timestamp = convertByteArrayTo64(data);
  data += sizeof(uint64_t);
  info_log(DEBUG, "\tTS timestamp: %ld\n", ts->timestamp);

  // offset
  ts->offset = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\tTS offset : %d\n", ts->offset);

  (*length) -= XCOM_FRAME_TS_SIZE;

  if (NULL != XCom_getCB_setTS())
  {
    XCom_getCB_setTS()(ts->timestamp);
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Conf_Tank(uint8_t *data, size_t *length, XCom_Conf_Tank_t *tank)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(tank);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_CONF_TANK_SIZE,
           "\nPayload is too small for XCom tank information.");

  // diameter
  tank->diameter.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tTank diameter value : %d\n", tank->diameter.value);

  tank->diameter.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tTank diameter unit : %d\n", tank->diameter.unit);

  // volume
  tank->volume.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tTank volume value : %d\n", tank->volume.value);

  tank->volume.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tTank volume unit : %d\n", tank->volume.unit);

  // heigth
  tank->heigth.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tTank heigth value : %d\n", tank->heigth.value);

  tank->heigth.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tTank heigth unit : %d\n", tank->heigth.unit);

  (*length) -= XCOM_FRAME_CONF_TANK_SIZE;

  if (NULL != XCom_getCB_setConfTank())
  {
    XCom_getCB_setConfTank()(tank->diameter.value, tank->diameter.unit, tank->volume.value, tank->volume.unit, tank->heigth.value, tank->heigth.unit);
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Conf_Sensor(uint8_t *data, size_t *length, XCom_Conf_Sensor_t *sensor)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(sensor);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_CONF_SENSOR_SIZE,
           "Payload is too small for XCom sensor information.");

  // number
  sensor->number = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor number : %d\n", sensor->number);

  // IP address
  sensor->ip = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tSensor IP address : %d\n", sensor->ip);

  // role
  sensor->role = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor role : %d\n", sensor->role);
  
  // type
  sensor->type = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor type : %d\n", sensor->type);

  // heigth
  sensor->heigth.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tSensor heigth value : %d\n", sensor->heigth.value);

  sensor->heigth.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor heigth unit : %d\n", sensor->heigth.unit);

  (*length) -= XCOM_FRAME_CONF_SENSOR_SIZE;

  if (NULL != XCom_getCB_setConfSensor())
  {
    XCom_getCB_setConfSensor()(sensor->number, sensor->ip, sensor->role, sensor->type, sensor->heigth.value, sensor->heigth.unit);
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Conf_Fluid(uint8_t *data, size_t *length, XCom_Conf_Fluid_t *fluid)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(fluid);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_CONF_FLUID_SIZE,
           "Payload is too small for XCom fluid information.");

  // density
  fluid->density.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tFluid density value : %d\n", fluid->density.value);

  fluid->density.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tFluid density unit : %d\n", fluid->density.unit);

  // gas pressure
  fluid->gasPressure.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tFluid gas pressure value : %d\n", fluid->gasPressure.value);

  fluid->gasPressure.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tFluid gas pressure unit : %d\n", fluid->gasPressure.unit);

  (*length) -= XCOM_FRAME_CONF_FLUID_SIZE;

  if (NULL != XCom_getCB_setConfFluid())
  {
    XCom_getCB_setConfFluid()(fluid->density.value, fluid->density.unit, fluid->gasPressure.value, fluid->gasPressure.unit);
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Conf_Cycle(uint8_t *data, size_t *length, XCom_Conf_Cycle_t *cycle)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(cycle);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_CONF_CYCLE_SIZE,
           "Payload is too small for XCom cycle information.");

  // cycle time
  cycle->time.value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\t\tCycle time value : %d\n", cycle->time.value);

  cycle->time.unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tCycle time unit : %d\n", cycle->time.unit);

  // frame loss
  cycle->frameLoss = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\t\tCycle frame loss : %d\n", cycle->frameLoss);

  (*length) -= XCOM_FRAME_CONF_CYCLE_SIZE;

  if (NULL != XCom_getCB_setConfCycle())
  {
    XCom_getCB_setConfCycle()(cycle->time.value, cycle->time.unit, cycle->frameLoss);
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Conf(uint8_t *data, size_t *length, XCom_Conf_t *conf)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(conf);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_CONF_SIZE, "Payload is too small for XCom configuration header.");

  // type
  conf->type = convertByteArrayTo8(data);
  data += sizeof(uint8_t);

  (*length) -= XCOM_FRAME_CONF_SIZE;

  // next header
  switch (conf->type)
  {
    case XCOM_CONF_TYPE_SEND_TANK_INFO:
      info_log(DEBUG, "\tConf type : Send tank info (0x%0x)\n", conf->type);
      return XCom_decode_Conf_Tank(data, length, &(conf->Tank));

    case XCOM_CONF_TYPE_GET_TANK_INFO:
      info_log(DEBUG, "\tConf type : Get tank info (0x%0x)\n", conf->type);
      break;

    case XCOM_CONF_TYPE_SEND_SENSOR_INFO:
      info_log(DEBUG, "\tConf type : Send sensor info (0x%0x)\n", conf->type);
      return XCom_decode_Conf_Sensor(data, length, &(conf->Sensor));

    case XCOM_CONF_TYPE_GET_SENSOR_INFO:
      info_log(DEBUG, "\tConf type : Get sensor info (0x%0x)\n", conf->type);
      break;

    case XCOM_CONF_TYPE_SEND_FLUID_INFO:
      info_log(DEBUG, "\tConf type : Send fluid info (0x%0x)\n", conf->type);
      return XCom_decode_Conf_Fluid(data, length, &(conf->Fluid));

    case XCOM_CONF_TYPE_GET_FLUID_INFO:
      info_log(DEBUG, "\tConf type : Get fluid info (0x%0x)\n", conf->type);
      break;

    case XCOM_CONF_TYPE_CYCLE_INFO:
      info_log(DEBUG, "\tConf type : Cycle info (0x%0x)\n", conf->type);
      return XCom_decode_Conf_Cycle(data, length, &(conf->Cycle));

    default:
      panic("Conf type not recognized.");
      break;
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Pressure(uint8_t *data, size_t *length, XCom_Pressure_t *pressure)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(pressure);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_GET_PRESSURE_SIZE, "Payload is too small for XCom get pressure.");

  // type
  pressure->type = convertByteArrayTo8(data);
  data += sizeof(uint8_t);

  if (pressure->type == XCOM_GET_PRESSURE)
  {
    info_log(DEBUG, "\tPressure type : Get pressure (0x%0x)\n", pressure->type);
    (*length) -= XCOM_FRAME_GET_PRESSURE_SIZE;

    return EXIT_SUCCESS;
  }

  if (pressure->type != XCOM_SEND_PRESSURE)
  {
    panic("Pressure type not recognized.");
    return EXIT_FAILURE;
  }

  info_log(DEBUG, "\tPressure type : Send pressure (0x%0x)\n", pressure->type);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_SEND_PRESSURE_SIZE,
           "Payload is too small for XCom send pressure.");

  // pressure
  pressure->value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\tPressure value : %d\n", pressure->value);

  pressure->unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\tPressure unit : %d\n", pressure->unit);

  // quality codewe need
  pressure->qualityCode = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\tPressure quality code : %d\n", pressure->qualityCode);

  (*length) -= XCOM_FRAME_SEND_PRESSURE_SIZE;

  if (NULL != XCom_getCB_setPressure())
  {
      XCom_getCB_setPressure()();
  }

  return EXIT_SUCCESS;
}

int XCom_decode_Level(uint8_t *data, size_t *length, XCom_Level_t *level)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(level);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_GET_LEVEL_SIZE, "Payload is too small for XCom get level.");

  // type
  level->type = convertByteArrayTo8(data);
  data += sizeof(uint8_t);

  if (level->type == XCOM_GET_LEVEL)
  {
    info_log(DEBUG, "\tLevel type : Get level (0x%0x)\n", level->type);
    (*length) -= XCOM_FRAME_GET_LEVEL_SIZE;

    return EXIT_SUCCESS;
  }

  if (level->type != XCOM_SEND_LEVEL)
  {
    panic("Level type not recognized.");
    return EXIT_FAILURE;
  }

  info_log(DEBUG, "\tLevel type : Send level (0x%0x)\n", level->type);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_SEND_LEVEL_SIZE, "Payload is too small for XCom send level.");

  // level
  level->value = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "\tLevel value : %d\n", level->value);

  level->unit = convertByteArrayTo8(data);
  data += sizeof(uint8_t);
  info_log(DEBUG, "\tLevel unit : %d\n", level->unit);

  (*length) -= XCOM_FRAME_SEND_PRESSURE_SIZE;

  if (NULL != XCom_getCB_setLevel())
  {
      XCom_getCB_setLevel()();
  }

  return EXIT_SUCCESS;
}

int XCom_decode(uint8_t *data, size_t *length, XCom_Frame_t *frame)
{
  int error;

  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(length);
  CHK_PTR(frame);

  // check data size before fill it
  CHK_SIZE(*length, XCOM_FRAME_SIZE, "Payload is too small for XCom header.");

  // msg ID
  frame->id = convertByteArrayTo64(data);
  data += sizeof(uint64_t);
  info_log(DEBUG, "Msg ID : %ld\n", frame->id);

  // msg length
  frame->length = convertByteArrayTo32(data);
  data += sizeof(uint32_t);
  info_log(DEBUG, "Msg Length : %d\n", frame->length);

  // msg type
  frame->type = convertByteArrayTo8(data);
  data += sizeof(uint8_t);

  (*length) -= XCOM_FRAME_SIZE;

  // next header
  switch (frame->type)
  {
    case XCOM_MSG_TYPE_TS:
      info_log(DEBUG, "Msg type : Time synchro (0x%0x)\n", frame->type);
      error = XCom_decode_TS(data, length, &(frame->TS));
      break;

    case XCOM_MSG_TYPE_CONF:
      info_log(DEBUG, "Msg type : Conf (0x%0x)\n", frame->type);
      error = XCom_decode_Conf(data, length, &(frame->Conf));
      break;

    case XCOM_MSG_TYPE_PRESSURE:
      info_log(DEBUG, "Msg type : Pressure (0x%0x)\n", frame->type);
      error = XCom_decode_Pressure(data, length, &(frame->Pressure));
      break;

    case XCOM_MSG_TYPE_LEVEL:
      info_log(DEBUG, "Msg type : Level (0x%0x)\n", frame->type);
      error = XCom_decode_Level(data, length, &(frame->Level));
      break;

    default:
      panic("Msg type not recognized.");
      return EXIT_FAILURE;
  }

  if (error == EXIT_FAILURE)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/************************* end of file *************************/