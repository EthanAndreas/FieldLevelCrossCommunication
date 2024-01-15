#ifndef XCOM_CALLBACKS_H
#define XCOM_CALLBACKS_H

/************************* include section *************************/

#include <stdint.h>

/************************* definition section *************************/

// a : timestamp
typedef void (*SetTS)(uint64_t a);

// a : diameter value, b : diameter unit, c : volume value, d : volume unit, e :
// height value, f : height unit
typedef void (*SetConfTank)(uint32_t a, uint8_t b, uint32_t c, uint8_t d,
                            uint32_t e, uint8_t f);
// a : number, b : ip address, c : role, d : type, e : height value, f : height unit
typedef void (*SetConfSensor)(uint8_t a, uint32_t b, uint8_t c, uint8_t d,
                              uint32_t e, uint8_t f);
// a : density value, b : density unit, c : gas pressure value, d : gas pressure
// unit
typedef void (*SetConfFluid)(uint32_t a, uint8_t b, uint32_t c, uint8_t d);
// a : time value, b : time unit, c : frame loss
typedef void (*SetConfCycle)(uint32_t a, uint8_t b, uint8_t c);

typedef void (*SetPressure)(void);

typedef void (*SetLevel)(void);

typedef struct
{
  SetTS         setTS;
  SetConfTank   setConfTank;
  SetConfSensor setConfSensor;
  SetConfFluid  setConfFluid;
  SetConfCycle  setConfCycle;
  SetPressure   setPressure;
  SetLevel      setLevel;
} XCom_callbacks;


void XCom_setCallbacks(XCom_callbacks cbs);

SetTS         XCom_getCB_setTS(void);
SetConfTank   XCom_getCB_setConfTank(void);
SetConfSensor XCom_getCB_setConfSensor(void);
SetConfFluid  XCom_getCB_setConfFluid(void);
SetConfCycle  XCom_getCB_setConfCycle(void);
SetPressure   XCom_getCB_setPressure(void);
SetLevel      XCom_getCB_setLevel(void);

#endif

/************************* end of file *************************/