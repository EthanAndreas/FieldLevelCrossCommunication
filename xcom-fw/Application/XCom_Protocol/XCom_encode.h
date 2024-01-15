#ifndef XCOM_ENCODE_H
#define XCOM_ENCODE_H

/************************* include section *************************/

#include "XCom_helper.h"
#include "XCom_protocol.h"

/************************* definition section *************************/

int XCom_encode(XCom_Frame_t frame, size_t length, uint8_t *data);

#endif

/************************* end of file *************************/