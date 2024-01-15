#ifndef __CALCULATION_H
#define __CALCULATION_H

/************************* include section *************************/

#include "Measurement.h"
#include "Task.h"
#include "XCom_helper.h"
#include "XCom_unit.h"

/************************* define section *************************/

#define GRAVITY 10

/************************* definition section *************************/

typedef struct
{
  uint32_t value;
  uint8_t  unit;
} Global_Level_t;


void Init_varLevel(void);

void           Set_eventLevel(void);
void           Set_level(uint32_t value, uint8_t unit);
Global_Level_t Get_level(void);

void Calculation(void);

#endif

/************************* end of file *************************/