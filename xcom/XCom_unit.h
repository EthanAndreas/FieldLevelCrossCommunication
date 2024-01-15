#ifndef XCOM_UNIT_H
#define XCOM_UNIT_H

/************************* include section *************************/

#include <stdint.h>

/************************* define section **************************/

/* Unit */
#define LENGTH_UNIT_METER 0x0
#define LENGTH_UNIT_FEET  0x1
#define LENGTH_UNIT_INCH  0x2

#define VOLUME_UNIT_METER3 0x0
#define VOLUME_UNIT_LITER  0x1
#define VOLUME_UNIT_GALLON 0x2

#define PRESSURE_UNIT_PASCAL 0x0
#define PRESSURE_UNIT_BAR    0x1
#define PRESSURE_UNIT_PSI    0x2

#define DENSITY_UNIT_KGBYMETER3 0x0
#define DENSITY_UNIT_GBYCM3     0x1

#define TIME_UNIT_SECOND      0x0
#define TIME_UNIT_MILLISECOND 0x1
#define TIME_UNIT_MICROSECOND 0x2

#define ROLE_HIGH    0x0
#define ROLE_DENSITY 0x1
#define ROLE_LOW     0x2

#define TYPE_SECONDARY 0x0
#define TYPE_MAIN      0x1

#define QUALITY_CODE_ERROR    0x0
#define QUALITY_CODE_REEL     0x1
#define QUALITY_CODE_SIMULATE 0x2

/* Size */
#define LENGTH_SIZE       (sizeof(uint32_t) + sizeof(uint8_t))
#define VOLUME_SIZE       (sizeof(uint32_t) + sizeof(uint8_t))
#define PRESSURE_SIZE     (sizeof(uint32_t) + sizeof(uint8_t))
#define DENSITY_SIZE      (sizeof(uint32_t) + sizeof(uint8_t))
#define TIME_SIZE         (sizeof(uint32_t) + sizeof(uint8_t))
#define ROLE_SIZE         (sizeof(uint8_t))
#define TYPE_SIZE         (sizeof(uint8_t))
#define QUALITY_CODE_SIZE (sizeof(uint8_t))

/************************** type section ***************************/

typedef struct
{
  uint32_t value;
  uint8_t  unit;
} Length_t;

typedef struct
{
  uint32_t value;
  uint8_t  unit;
} Volume_t;

typedef struct
{
  uint32_t value;
  uint8_t  unit;
} Pressure_t;

typedef struct
{
  uint32_t value;
  uint8_t  unit;
} Density_t;

typedef struct
{
  uint32_t value;
  uint8_t  unit;
} Time_t;

#endif

/************************* end of file *************************/