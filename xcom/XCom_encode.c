/************************* include section *************************/

#include "XCom_encode.h"

/************************* definition section *************************/

int XCom_encode_TS(XCom_TS_t ts, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_TS_SIZE,
           "Payload is too small for TS.");

  // ts type
  convert8ToByteArray(ts.type, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\tTS type : %d\n", ts.type);

  // ts timestamp
  convert64ToByteArray(ts.timestamp, &data[*index]);
  *index += sizeof(uint64_t);
  info_log(DEBUG, "\tTS timestamp : %ld\n", ts.timestamp);

  // ts offset
  convert32ToByteArray(ts.offset, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\tTS offset : %d\n", ts.offset);

  return EXIT_SUCCESS;
}

int XCom_encode_Conf_Tank(XCom_Conf_Tank_t tank, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_CONF_TANK_SIZE,
           "Payload is too small for XCom conf tank information.");

  // diameter
  convert32ToByteArray(tank.diameter.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tTank diameter value : %d\n", tank.diameter.value);

  convert8ToByteArray(tank.diameter.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tTank diameter unit : %d\n", tank.diameter.unit);

  // volume
  convert32ToByteArray(tank.volume.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tTank volume value : %d\n", tank.volume.value);

  convert8ToByteArray(tank.volume.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tTank volume unit : %d\n", tank.volume.unit);

  // heigth
  convert32ToByteArray(tank.heigth.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tTank heigth value : %d\n", tank.heigth.value);

  convert8ToByteArray(tank.heigth.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tTank heigth unit : %d\n", tank.heigth.unit);

  return EXIT_SUCCESS;
}

int XCom_encode_Conf_Sensor(XCom_Conf_Sensor_t sensor, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_CONF_SENSOR_SIZE,
           "Payload is too small for XCom conf sensor information.");

  // number
  convert8ToByteArray(sensor.number, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor number : %d\n", sensor.number);

  // IP address
  convert32ToByteArray(sensor.ip, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tSensor IP address : %d\n", sensor.ip);

  // role
  convert8ToByteArray(sensor.role, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor role : %d\n", sensor.role);

  // type
  convert8ToByteArray(sensor.type, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor type : %d\n", sensor.type);

  // heigth
  convert32ToByteArray(sensor.heigth.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tSensor heigth value : %d\n", sensor.heigth.value);

  convert8ToByteArray(sensor.heigth.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tSensor heigth unit : %d\n", sensor.heigth.unit);

  return EXIT_SUCCESS;
}

int XCom_encode_Conf_Fluid(XCom_Conf_Fluid_t fluid, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_CONF_FLUID_SIZE,
           "Payload is too small for XCom conf fluid information.");

  // density
  convert32ToByteArray(fluid.density.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tFluid density value : %d\n", fluid.density.value);

  convert8ToByteArray(fluid.density.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tFluid density unit : %d\n", fluid.density.unit);

  // gas pressure
  convert32ToByteArray(fluid.gasPressure.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tFluid gas pressure value : %d\n",
           fluid.gasPressure.value);

  convert8ToByteArray(fluid.gasPressure.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tFluid gas pressure unit : %d\n",
           fluid.gasPressure.unit);

  return EXIT_SUCCESS;
}

int XCom_encode_Conf_Cycle(XCom_Conf_Cycle_t cycle, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_CONF_CYCLE_SIZE,
           "Payload is too small for XCom conf cycle information.");

  // cycle time
  convert32ToByteArray(cycle.time.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\t\tCycle time value : %d\n", cycle.time.value);

  convert8ToByteArray(cycle.time.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tCycle time unit : %d\n", cycle.time.unit);

  // frame loss
  convert8ToByteArray(cycle.frameLoss, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\t\tCycle frame loss : %d\n", cycle.frameLoss);

  return EXIT_SUCCESS;
}

int XCom_encode_Conf(XCom_Conf_t conf, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_CONF_SIZE,
           "Payload is too small for XCom conf.");

  // type
  convert8ToByteArray(conf.type, &data[*index]);
  *index += sizeof(uint8_t);

  // next header
  switch (conf.type)
  {
    case XCOM_CONF_TYPE_SEND_TANK_INFO:
      info_log(DEBUG, "\tConf type : Send tank info (0x%0x)\n",
               conf.type);
      return XCom_encode_Conf_Tank(conf.Tank, data, length, index);
      break;

    case XCOM_CONF_TYPE_GET_TANK_INFO:
      info_log(DEBUG, "\tConf type : Get tank info (0x%0x)\n", conf.type);
      break;

    case XCOM_CONF_TYPE_SEND_SENSOR_INFO:
      info_log(DEBUG, "\tConf type : Send sensor info (0x%0x)\n",
               conf.type);
      return XCom_encode_Conf_Sensor(conf.Sensor, data, length, index);
      break;

    case XCOM_CONF_TYPE_GET_SENSOR_INFO:
      info_log(DEBUG, "\tConf type : Get sensor info (0x%0x)\n",
               conf.type);
      break;

    case XCOM_CONF_TYPE_SEND_FLUID_INFO:
      info_log(DEBUG, "\tConf type : Send fluid info (0x%0x)\n",
               conf.type);
      return XCom_encode_Conf_Fluid(conf.Fluid, data, length, index);
      break;

    case XCOM_CONF_TYPE_GET_FLUID_INFO:
      info_log(DEBUG, "\tConf type : Get fluid info (0x%0x)\n",
               conf.type);
      break;

    case XCOM_CONF_TYPE_CYCLE_INFO:
      info_log(DEBUG, "\tConf type : Cycle info (0x%0x)\n", conf.type);
      return XCom_encode_Conf_Cycle(conf.Cycle, data, length, index);
      ;
      break;

    default:
      panic("Conf type not recognized.");
      break;
  }

  return EXIT_SUCCESS;
}

int XCom_encode_Pressure(XCom_Pressure_t pressure, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_GET_LEVEL_SIZE,
           "Payload is too small for XCom get pressure.");

  // type
  convert8ToByteArray(pressure.type, &data[*index]);
  *index += sizeof(uint8_t);

  if (pressure.type == XCOM_GET_PRESSURE)
  {
    info_log(DEBUG, "\tPressure type : Get pressure (0x%0x)\n",
             pressure.type);

    return EXIT_SUCCESS;
  }

  if (pressure.type != XCOM_SEND_PRESSURE)
  {
    panic("Pressure type not recognized.");
    return EXIT_FAILURE;
  }

  info_log(DEBUG, "\tPressure type : Send pressure (0x%0x)\n", pressure.type);

  // check data size before fill it
  CHK_SIZE(*index + XCOM_FRAME_SEND_LEVEL_SIZE, length,
           "Payload is too small for XCom send pressure.");

  // pressure
  convert32ToByteArray(pressure.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\tPressure value : %d\n", pressure.value);

  convert8ToByteArray(pressure.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\tPressure unit : %d\n", pressure.unit);

  // quality code
  convert8ToByteArray(pressure.qualityCode, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\tPressure quality code : %d\n", pressure.qualityCode);

  return EXIT_SUCCESS;
}

int XCom_encode_Level(XCom_Level_t level, uint8_t *data, size_t length, size_t *index)
{
  // check pointers are not null
  CHK_PTR(data);
  CHK_PTR(index);

  // check data size before fill it
  CHK_SIZE(length, *index + XCOM_FRAME_GET_LEVEL_SIZE,
           "Payload is too small for XCom get level.");

  // type
  convert8ToByteArray(level.type, &data[*index]);
  *index += sizeof(uint8_t);

  if (level.type == XCOM_GET_LEVEL)
  {
    info_log(DEBUG, "\tLevel type : Get level (0x%0x)\n", level.type);

    return EXIT_SUCCESS;
  }

  if (level.type != XCOM_SEND_LEVEL)
  {
    panic("Level type not recognized.");
    return EXIT_FAILURE;
  }

  info_log(DEBUG, "\tLevel type : Send level (0x%0x)\n", level.type);

  // check data size before fill it
  CHK_SIZE(*index + XCOM_FRAME_SEND_LEVEL_SIZE, length,
           "Payload is too small for XCom send level.");

  // level
  convert32ToByteArray(level.value, &data[*index]);
  *index += sizeof(uint32_t);
  info_log(DEBUG, "\tLevel value : %d\n", level.value);

  convert8ToByteArray(level.unit, &data[*index]);
  *index += sizeof(uint8_t);
  info_log(DEBUG, "\tLevel unit : %d\n", level.unit);

  return EXIT_SUCCESS;
}

int XCom_encode(XCom_Frame_t frame, size_t length, uint8_t *data)
{
  int    error;
  size_t index = 0;

  // check pointer is not null
  CHK_PTR(data);

  // check data size before fill it
  CHK_SIZE(length, index + XCOM_FRAME_SIZE,
           "Payload is too small for XCom header.");

  // msg ID
  convert64ToByteArray(frame.id, &data[index]);
  index += sizeof(uint64_t);
  info_log(DEBUG, "Msg ID : %ld\n", frame.id);

  // msg length
  convert32ToByteArray(frame.length, &data[index]);
  index += sizeof(uint32_t);
  info_log(DEBUG, "Msg length : %d\n", frame.length);

  // msg type
  convert8ToByteArray(frame.type, &data[index]);
  index += sizeof(uint8_t);

  // next header
  switch (frame.type)
  {
    case XCOM_MSG_TYPE_TS:
      info_log(DEBUG, "Msg type : Time synchro (0x%0x)\n", frame.type);
      error = XCom_encode_TS(frame.TS, data, length, &index);
      break;

    case XCOM_MSG_TYPE_CONF:
      info_log(DEBUG, "Msg type : Conf (0x%0x)\n", frame.type);
      error = XCom_encode_Conf(frame.Conf, data, length, &index);
      break;

    case XCOM_MSG_TYPE_PRESSURE:
      info_log(DEBUG, "Msg type : Pressure (0x%0x)\n", frame.type);
      error = XCom_encode_Pressure(frame.Pressure, data, length, &index);
      break;

    case XCOM_MSG_TYPE_LEVEL:
      info_log(DEBUG, "Msg type : Level (0x%0x)\n", frame.type);
      error = XCom_encode_Level(frame.Level, data, length, &index);
      break;

    default:
      panic("Msg type not recognized.");
      return EXIT_FAILURE;
  }

  if (error == EXIT_FAILURE)
  {
    free(data);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/************************* end of file *************************/