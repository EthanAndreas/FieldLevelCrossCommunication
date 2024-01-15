#ifndef XCOM_PROTOCOL_H
#define XCOM_PROTOCOL_H

/************************* include section *************************/

#include "XCom_unit.h"

/************************* define section **************************/

/* Type */

// Msg type
#define XCOM_MSG_TYPE_TS       0x01
#define XCOM_MSG_TYPE_CONF     0x02
#define XCOM_MSG_TYPE_PRESSURE 0x03
#define XCOM_MSG_TYPE_LEVEL    0x04

// TS type
#define XCOM_TS_TYPE_INIT  0x01
#define XCOM_TS_TYPE_CYCLE 0x02

// Conf type
#define XCOM_CONF_TYPE_SEND_TANK_INFO   0x01
#define XCOM_CONF_TYPE_GET_TANK_INFO    0x02
#define XCOM_CONF_TYPE_SEND_SENSOR_INFO 0x03
#define XCOM_CONF_TYPE_GET_SENSOR_INFO  0x04
#define XCOM_CONF_TYPE_SEND_FLUID_INFO  0x05
#define XCOM_CONF_TYPE_GET_FLUID_INFO   0x06
#define XCOM_CONF_TYPE_CYCLE_INFO       0x07

// Pressure type
#define XCOM_SEND_PRESSURE 0x01
#define XCOM_GET_PRESSURE  0x02

// Level type
#define XCOM_SEND_LEVEL 0x01
#define XCOM_GET_LEVEL  0x02

/* Header size */

#define XCOM_FRAME_SIZE               (sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint8_t))  // union not included
#define XCOM_FRAME_TS_SIZE            (sizeof(uint8_t) + sizeof(uint64_t) + sizeof(uint32_t))
#define XCOM_FRAME_CONF_SIZE          (sizeof(uint8_t))  // union not included
#define XCOM_FRAME_CONF_TANK_SIZE     (LENGTH_SIZE + VOLUME_SIZE + LENGTH_SIZE)
#define XCOM_FRAME_CONF_SENSOR_SIZE   (sizeof(uint8_t) + sizeof(uint32_t) + ROLE_SIZE + TYPE_SIZE + LENGTH_SIZE)
#define XCOM_FRAME_CONF_FLUID_SIZE    (DENSITY_SIZE + PRESSURE_SIZE)
#define XCOM_FRAME_CONF_CYCLE_SIZE    (TIME_SIZE + sizeof(uint8_t))
#define XCOM_FRAME_SEND_PRESSURE_SIZE (sizeof(uint8_t) + PRESSURE_SIZE + QUALITY_CODE_SIZE)
#define XCOM_FRAME_GET_PRESSURE_SIZE  (sizeof(uint8_t))
#define XCOM_FRAME_SEND_LEVEL_SIZE    (sizeof(uint8_t) + LENGTH_SIZE)
#define XCOM_FRAME_GET_LEVEL_SIZE     (sizeof(uint8_t))

/* Message size */

#define XCOM_TS_SIZE               (XCOM_FRAME_SIZE + XCOM_FRAME_TS_SIZE)
#define XCOM_CONF_SEND_TANK_SIZE   (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE + XCOM_FRAME_CONF_TANK_SIZE)
#define XCOM_CONF_GET_TANK_SIZE    (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE)
#define XCOM_CONF_SEND_SENSOR_SIZE (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE + XCOM_FRAME_CONF_SENSOR_SIZE)
#define XCOM_CONF_GET_SENSOR_SIZE  (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE)
#define XCOM_CONF_SEND_FLUID_SIZE  (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE + XCOM_FRAME_CONF_FLUID_SIZE)
#define XCOM_CONF_GET_FLUID_SIZE   (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE)
#define XCOM_CONF_CYCLE_SIZE       (XCOM_FRAME_SIZE + XCOM_FRAME_CONF_SIZE + XCOM_FRAME_CONF_CYCLE_SIZE)
#define XCOM_SEND_PRESSURE_SIZE    (XCOM_FRAME_SIZE + XCOM_FRAME_SEND_PRESSURE_SIZE)
#define XCOM_GET_PRESSURE_SIZE     (XCOM_FRAME_SIZE + XCOM_FRAME_GET_PRESSURE_SIZE)
#define XCOM_SEND_LEVEL_SIZE       (XCOM_FRAME_SIZE + XCOM_FRAME_SEND_LEVEL_SIZE)
#define XCOM_GET_LEVEL_SIZE        (XCOM_FRAME_SIZE + XCOM_FRAME_GET_LEVEL_SIZE)

/************************** type section ***************************/

typedef struct
{
  uint8_t  type;
  uint64_t timestamp;
  uint32_t offset;
} XCom_TS_t;


typedef struct
{
  Length_t diameter;
  Volume_t volume;
  Length_t heigth;
} XCom_Conf_Tank_t;

typedef struct
{
  uint8_t  number;
  uint32_t ip;
  uint8_t  role;
  uint8_t  type;
  Length_t heigth;
} XCom_Conf_Sensor_t;

typedef struct
{
  Density_t  density;
  Pressure_t gasPressure;
} XCom_Conf_Fluid_t;

typedef struct
{
  Time_t  time;
  uint8_t frameLoss;
} XCom_Conf_Cycle_t;

typedef struct
{
  uint8_t type;
  union
  {
    XCom_Conf_Tank_t   Tank;
    XCom_Conf_Sensor_t Sensor;
    XCom_Conf_Fluid_t  Fluid;
    XCom_Conf_Cycle_t  Cycle;
  };
} XCom_Conf_t;


typedef struct
{
  uint8_t  type;
  uint32_t value;
  uint8_t  unit;
  uint8_t  qualityCode;
} XCom_Pressure_t;


typedef struct
{
  uint8_t  type;
  uint32_t value;
  uint8_t  unit;
} XCom_Level_t;


typedef struct
{
  uint64_t id;
  uint32_t length;
  uint8_t  type;
  union
  {
    XCom_TS_t       TS;
    XCom_Conf_t     Conf;
    XCom_Pressure_t Pressure;
    XCom_Level_t    Level;
  };
} XCom_Frame_t;

#endif

/************************* end of file *************************/