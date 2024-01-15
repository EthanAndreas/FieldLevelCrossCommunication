/************************* include section *************************/

#include "XCom_callbacks.h"

/************************* definition section *************************/

static XCom_callbacks callbacks = {0};


void XCom_setCallbacks(XCom_callbacks cbs)
{
  callbacks = cbs;
}


SetTS XCom_getCB_setTS(void)
{
  return callbacks.setTS;
}

SetConfTank XCom_getCB_setConfTank(void)
{
  return callbacks.setConfTank;
}

SetConfSensor XCom_getCB_setConfSensor(void)
{
  return callbacks.setConfSensor;
}

SetConfFluid XCom_getCB_setConfFluid(void)
{
  return callbacks.setConfFluid;
}

SetConfCycle XCom_getCB_setConfCycle(void)
{
  return callbacks.setConfCycle;
}


SetPressure XCom_getCB_setPressure(void)
{
	return callbacks.setPressure;
}


SetLevel XCom_getCB_setLevel(void)
{
	return callbacks.setLevel;
}

/************************* end of file *************************/