/************************* include section *************************/

#include "XCom_init.h"

/************************* definition section *************************/

int XCom_Free(XCom_Frame_t *frame)
{
  CHK_PTR(frame);

  frame->id     = 0;
  frame->length = 0;
  frame->type   = 0;

  frame->Conf.type = 0;

  frame->Conf.Tank.diameter.value = 0;
  frame->Conf.Tank.diameter.unit  = 0;
  frame->Conf.Tank.volume.value   = 0;
  frame->Conf.Tank.volume.unit    = 0;
  frame->Conf.Tank.heigth.value   = 0;
  frame->Conf.Tank.heigth.unit    = 0;

  frame->Conf.Sensor.number       = 0;
  frame->Conf.Sensor.ip           = 0;
  frame->Conf.Sensor.role         = 0;
  frame->Conf.Sensor.type         = 0;
  frame->Conf.Sensor.heigth.value = 0;
  frame->Conf.Sensor.heigth.unit  = 0;

  frame->Conf.Fluid.density.value     = 0;
  frame->Conf.Fluid.density.unit      = 0;
  frame->Conf.Fluid.gasPressure.value = 0;
  frame->Conf.Fluid.gasPressure.unit  = 0;

  frame->Conf.Cycle.time.value = 0;
  frame->Conf.Cycle.time.unit  = 0;
  frame->Conf.Cycle.frameLoss  = 0;

  frame->Pressure.type        = 0;
  frame->Pressure.value       = 0;
  frame->Pressure.unit        = 0;
  frame->Pressure.qualityCode = 0;

  frame->Level.type  = 0;
  frame->Level.value = 0;
  frame->Level.unit  = 0;

  return EXIT_SUCCESS;
}

/************************* end of file *************************/