#ifndef XCOM_INIT_H
#define XCOM_INIT_H

/************************* include section *************************/

#include "XCom_helper.h"
#include "XCom_protocol.h"

/************************* definition section *************************/

int XCom_Init_TS(XCom_Frame_t *frame, uint8_t type, uint64_t timestamp, uint32_t offset);

int XCom_Init_Conf_Send_Tank(XCom_Frame_t *frame, uint32_t diameterValue, uint8_t diameterUnit, uint32_t volumeValue, uint8_t volumeUnit, uint32_t heigthValue, uint8_t heigthUnit);
int XCom_Init_Conf_Get_Tank(XCom_Frame_t *frame);

int XCom_Init_Conf_Send_Sensor(XCom_Frame_t *frame, uint8_t number, uint32_t ip, uint8_t role, uint8_t type, uint32_t heigthValue, uint8_t heigthUnit);
int XCom_Init_Conf_Get_Sensor(XCom_Frame_t *frame);

int XCom_Init_Conf_Send_Fluid(XCom_Frame_t *frame, uint32_t densityValue, uint8_t densityUnit, uint32_t gasPressureValue, uint8_t gasPressureUnit);
int XCom_Init_Conf_Get_Fluid(XCom_Frame_t *frame);

int XCom_Init_Conf_Cycle(XCom_Frame_t *frame, uint32_t timeValue, uint8_t timeUnit, uint8_t frameLoss);

int XCom_Init_Pressure(XCom_Frame_t *frame, uint8_t type, uint32_t pressureValue, uint8_t pressureUnit, uint8_t qualityCode);

int XCom_Init_Level(XCom_Frame_t *frame, uint8_t type, uint32_t levelValue, uint8_t levelUnit);

#endif

/************************* end of file *************************/