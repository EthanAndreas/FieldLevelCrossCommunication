/************************* include section *************************/

#include "Calculation.h"

/************************* definition section *************************/

OS_MUTEX levelMutex;

static Global_Level_t Level = {0};


void Init_varLevel(void)
{
  OS_MUTEX_Create(&levelMutex);
}

void Set_eventLevel(void) {}

void Set_level(uint32_t value, uint8_t unit)
{
  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&levelMutex));

  Level.value = value;
  Level.unit  = unit;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&levelMutex));
}

Global_Level_t Get_level(void)
{
  Global_Level_t copyLevel = {0};

  CHK_INTERRUPT(OS_MUTEX_LockBlocked(&levelMutex));

  copyLevel = Level;

  CHK_INTERRUPT(OS_MUTEX_Unlock(&levelMutex));

  return copyLevel;
}

void Calculation(void)
{
  int      i;
  uint32_t density = 0, level = 0;
  uint8_t  boardNb = 0;

  Global_ConfTank_t  tank                   = {0};
  Global_ConfFluid_t fluid                  = {0};
  Global_Pressure_t  pressures[MAX_SENSORS] = {0};

  // Fill values needed for the level calculation
  boardNb = Get_confSensorNumber();
  tank    = Get_confTank();
  fluid   = Get_confFluid();

  // Get all the pressures
  for (i = 0; i < MAX_SENSORS; i++)
  {
    pressures[i] = Get_pressure(i);
  }

  // Calculate the level depending on the number of boards
  switch (boardNb)
  {
    case 1:
      if (pressures[ROLE_HIGH].qualityCode != QUALITY_CODE_ERROR)
      {
        level = (Convert_pressure(pressures[ROLE_HIGH].value, pressures[ROLE_HIGH].unit) - fluid.gasPressureValue) / (fluid.densityValue * GRAVITY);
      }

      break;

    case 2:
      if (pressures[ROLE_HIGH].qualityCode != QUALITY_CODE_ERROR)
      {
        // If the sencond sensor had the density role
        if (pressures[ROLE_DENSITY].value != 0 && pressures[ROLE_DENSITY].qualityCode != QUALITY_CODE_ERROR)
        {
          density = (Convert_pressure(pressures[ROLE_HIGH].value, pressures[ROLE_HIGH].unit) - Convert_pressure(pressures[ROLE_DENSITY].value, pressures[ROLE_DENSITY].unit)) / (GRAVITY * tank.heightValue);
          level   = (Convert_pressure(pressures[ROLE_HIGH].value, pressures[ROLE_HIGH].unit) - fluid.gasPressureValue) / (density * GRAVITY);
        }
        // If the sencond sensor had the low role
        else if (pressures[ROLE_LOW].value != 0 && pressures[ROLE_LOW].qualityCode != QUALITY_CODE_ERROR)
        {
          level = (Convert_pressure(pressures[ROLE_HIGH].value, pressures[ROLE_HIGH].unit) - Convert_pressure(pressures[ROLE_LOW].value, pressures[ROLE_LOW].unit)) / (fluid.densityValue * GRAVITY);
        }
      }
      break;

    case 3:
      if (pressures[ROLE_HIGH].qualityCode != QUALITY_CODE_ERROR && pressures[ROLE_DENSITY].qualityCode != QUALITY_CODE_ERROR && pressures[ROLE_LOW].qualityCode != QUALITY_CODE_ERROR)
      {
        density = (Convert_pressure(pressures[ROLE_HIGH].value, pressures[ROLE_HIGH].unit) - Convert_pressure(pressures[ROLE_DENSITY].value, pressures[ROLE_DENSITY].unit)) / (GRAVITY * tank.heightValue);
        level   = (Convert_pressure(pressures[ROLE_HIGH].value, pressures[ROLE_HIGH].unit) - Convert_pressure(pressures[ROLE_LOW].value, pressures[ROLE_LOW].unit)) / (density * GRAVITY);
      }
      break;

    default:
      break;
  }

  // Reset pressure array and put quality code to error
  for (i = 0; i < boardNb; i++)
  {
    Set_pressure(i, 0, 0, QUALITY_CODE_ERROR);
  }

  // Set the level
  Set_level(level, LENGTH_UNIT_METER);
}

/************************* end of file *************************/