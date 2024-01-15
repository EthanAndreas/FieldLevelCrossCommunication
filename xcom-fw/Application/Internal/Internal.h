#ifndef __INTERNAL_H
#define __INTERNAL_H

/************************* include section *************************/

#include "Calculation.h"
#include "Configuration.h"
#include "Measurement.h"
#include "Time_synchro.h"
#include "UDP_send.h"
#include "XCom_protocol.h"

/************************* define section *************************/


/************************* definition section *************************/

void Variables_Init(void);
void Callbacks_Init(void);

void GetPacket_handler(uint32_t ip, uint16_t fport, XCom_Frame_t frame);

#endif

/************************* end of file *************************/