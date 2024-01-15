#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

/************************* include section *************************/

#include "Configuration.h"
#include "Task.h"
#include "XCom_helper.h"
#include "XCom_unit.h"

/************************* define section *************************/

#define SENSOR_1       0
#define SENSOR_2       1
#define SENSOR_3       2
#define MAX_SENSORS    3
#define SIZE_PRESSURE  6
#define SIZE_PRESSURES (MAX_SENSORS * SIZE_PRESSURE)

#define BAR_TO_PA 100000
#define PSI_TO_PA 6895

/************************* definition section *************************/

typedef struct
{
  uint32_t value;
  uint8_t  unit;
  uint8_t  qualityCode;
} Global_Pressure_t;


void Init_varPressure(void);

void Set_eventPressure(void);

void              Set_pressure(uint8_t sensorID, uint32_t value, uint8_t unit, uint8_t qualityCode);
void              Set_currentPressure(uint32_t value, uint8_t unit, uint8_t qualityCode);
Global_Pressure_t Get_pressure(uint8_t index);
Global_Pressure_t Get_currentPressure(void);

uint32_t Convert_pressure(uint32_t value, uint8_t unit);

void Measurement(void);

#endif

/************************* end of file *************************/