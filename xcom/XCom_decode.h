#ifndef XCOM_DECODE_H
#define XCOM_DECODE_H

/************************* include section *************************/

#include "XCom_helper.h"
#include "XCom_protocol.h"

/************************* definition section *************************/

int XCom_decode(uint8_t *data, size_t *length, XCom_Frame_t *frame);

#endif

/************************* end of file *************************/