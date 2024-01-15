#ifndef __UDP_SEND_H
#define __UDP_SEND_H

/************************** include section ***************************/

#include "BSP.h"
#include "IP.h"
#include "RTOS.h"
#include "XCom_encode.h"
#include "XCom_init.h"

/************************** define section ***************************/

#define XCOM_PORT   3630
#define PACKET_SIZE 256

#define WAIT_FOR_ARP_REPLY 1

#define IP_BYTES_TO_UINT32(a, b, c, d) htonl(IP_BYTES2ADDR(a, b, c, d))

/************************** definition section ***************************/

int Send_XCom_TS(uint32_t dest, uint16_t fport, uint8_t type, uint64_t timestamp, uint32_t offset);

int Send_XCom_Conf_Send_Tank(uint32_t dest, uint16_t fport, uint32_t diameterValue, uint8_t diameterUnit, uint32_t volumeValue, uint8_t volumeUnit, uint32_t heigthValue, uint8_t heigthUnit);
int Send_XCom_Conf_Get_Tank(uint32_t dest, uint16_t fport);
int Send_XCom_Conf_Send_Sensor(uint32_t dest, uint16_t fport, uint8_t number, uint32_t ip, uint8_t role, uint8_t type, uint32_t heigthValue, uint8_t heigthUnit);
int Send_XCom_Conf_Get_Sensor(uint32_t dest, uint16_t fport);
int Send_XCom_Conf_Send_Fluid(uint32_t dest, uint16_t fport, uint32_t densityValue, uint8_t densityUnit, uint32_t gasPressureValue, uint8_t gasPressureUnit);
int Send_XCom_Conf_Get_Fluid(uint32_t dest, uint16_t fport);
int Send_XCom_Conf_Cycle(uint32_t dest, uint16_t fport, uint32_t timeValue, uint8_t timeUnit, uint8_t frameLoss);

int Send_XCom_Pressure(uint32_t dest, uint16_t fport, uint8_t type, uint32_t pressureValue, uint8_t pressureUnit, uint8_t qualityCode);

int Send_XCom_Level(uint32_t dest, uint16_t fport, uint8_t type, uint32_t levelValue, uint8_t levelUnit);

#endif

/************************* end of file *************************/