#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

/************************* include section *************************/

#include "Internal.h"
#include "Task.h"
#include "XCom_decode.h"
#include "XCom_free.h"
#include "XCom_helper.h"
#include "XCom_protocol.h"
#include "XCom_unit.h"

/************************* define section *************************/

#define MAX_SENSORS  3
#define SIZE_SENSOR  16
#define SIZE_SENSORS (MAX_SENSORS * SIZE_SENSOR)
#define NOT_A_SENSOR (-1)

#define CONF_Log IP_Log

/************************* definition section *************************/

typedef enum
{
  false = 0,  // 0
  true,       // 1
} bool_t;

typedef struct
{
  uint32_t diameterValue;
  uint8_t  diameterUnit;
  uint32_t volumeValue;
  uint8_t  volumeUnit;
  uint32_t heightValue;
  uint8_t  heightUnit;
} Global_ConfTank_t;

typedef struct
{
  uint8_t  number;
  uint32_t ip;
  uint8_t  role;
  uint8_t  type;
  uint32_t heightValue;
  uint8_t  heightUnit;
} Global_ConfSensor_t;

typedef struct
{
  uint32_t densityValue;
  uint8_t  densityUnit;
  uint32_t gasPressureValue;
  uint8_t  gasPressureUnit;
} Global_ConfFluid_t;

typedef struct
{
  uint32_t timeValue;
  uint8_t  timeUnit;
  uint8_t  frameLoss;
} Global_ConfCycle_t;


void Init_varConf(void);

void   Set_isMain(void);
bool_t Get_isMain(void);

void   Set_isSetConf(void);
bool_t Get_isSetConf(void);

void              Set_confTank(uint32_t diameterValue, uint8_t diameterUnit, uint32_t volumeValue, uint8_t volumeUnit, uint32_t heightValue, uint8_t heightUnit);
Global_ConfTank_t Get_confTank(void);

void                Set_confSensor(uint8_t number, uint32_t ip, uint8_t role, uint8_t type, uint32_t heightValue, uint8_t heightUnit);
void                Set_confSensorLessOne(uint32_t ip);
Global_ConfSensor_t Get_confSensor(int i);
uint8_t             Get_confSensorIndex(uint32_t ip);
uint8_t             Get_confSensorNumber(void);
uint8_t             Get_confSensorMain(void);
uint32_t            Get_confSensorMainIP(void);

void               Set_confFluid(uint32_t densityValue, uint8_t densityUnit, uint32_t gasPressureValue, uint8_t gasPressureUnit);
Global_ConfFluid_t Get_confFluid(void);

void               Set_confCycle(uint32_t timeValue, uint8_t timeUnit, uint8_t frameLoss);
Global_ConfCycle_t Get_confCycle(void);
uint32_t           Get_confCyclePeriodInMS(void);

void     Set_monitoringAppIP(uint32_t ip);
uint32_t Get_monitoringAppIP(void);

void Conf_Init(void);

#endif

/************************* end of file *************************/