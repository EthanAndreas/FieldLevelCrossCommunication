/************************** include section ***************************/

#include "UDP_send.h"

/************************** definition section ***************************/

int UDP_Send(uint32_t dest, uint16_t fport, XCom_Frame_t *frame)
{
  int        error;
  IP_PACKET *packet;

  CHK_PTR(packet = IP_UDP_Alloc(frame->length));
  CHK_SUCCESS(XCom_encode(*frame, frame->length, packet->pData));

  CHK_RET(error = IP_UDP_SendAndFree(0, dest, fport, XCOM_PORT, packet));

  if (error == WAIT_FOR_ARP_REPLY)
  {
    IP_Log("Packet sent (Warning - no ARP reply)");
  }
  else
  {
    IP_Log("Packet sent");
  }

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_TS
 */
int Send_XCom_TS(uint32_t dest, uint16_t fport, uint8_t type,
                 uint64_t timestamp, uint32_t offset)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_TS(frame, type, timestamp, offset));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Send_Tank
 */
int Send_XCom_Conf_Send_Tank(uint32_t dest, uint16_t fport,
                             uint32_t diameterValue, uint8_t diameterUnit, uint32_t volumeValue,
                             uint8_t volumeUnit, uint32_t heigthValue, uint8_t heigthUnit)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Send_Tank(frame, diameterValue,
                                       diameterUnit, volumeValue, volumeUnit, heigthValue,
                                       heigthUnit));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Get_Tank
 */
int Send_XCom_Conf_Get_Tank(uint32_t dest, uint16_t fport)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Get_Tank(frame));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Send_Sensor
 */
int Send_XCom_Conf_Send_Sensor(uint32_t dest, uint16_t fport, uint8_t number,
                               uint32_t ip, uint8_t role, uint8_t type, uint32_t heigthValue,
                               uint8_t heigthUnit)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Send_Sensor(frame, number, ip, role,
                                         type, heigthValue, heigthUnit));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Get_Sensor
 */
int Send_XCom_Conf_Get_Sensor(uint32_t dest, uint16_t fport)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Get_Sensor(frame));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Send_Fluid
 */
int Send_XCom_Conf_Send_Fluid(uint32_t dest, uint16_t fport, uint32_t densityValue,
                              uint8_t densityUnit, uint32_t gasPressureValue, uint8_t gasPressureUnit)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Send_Fluid(frame, densityValue, densityUnit,
                                        gasPressureValue, gasPressureUnit));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Get_Fluid
 */
int Send_XCom_Conf_Get_Fluid(uint32_t dest, uint16_t fport)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Get_Fluid(frame));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Conf_Cycle
 */
int Send_XCom_Conf_Cycle(uint32_t dest, uint16_t fport, uint32_t timeValue,
                         uint8_t timeUnit, uint8_t frameLoss)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Conf_Cycle(frame, timeValue, timeUnit, frameLoss));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Pressure
 */
int Send_XCom_Pressure(uint32_t dest, uint16_t fport, uint8_t type,
                       uint32_t pressureValue, uint8_t pressureUnit, uint8_t qualityCode)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Pressure(frame, type, pressureValue, pressureUnit,
                                 qualityCode));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/*********************************************************************
 *
 *       Send_XCom_Level
 */
int Send_XCom_Level(uint32_t dest, uint16_t fport, uint8_t type,
                    uint32_t levelValue, uint8_t levelUnit)
{
  XCom_Frame_t *frame;
  CHK_PTR(frame = calloc(1, sizeof(XCom_Frame_t)));

  CHK_SUCCESS(XCom_Init_Level(frame, type, levelValue, levelUnit));

  if (UDP_Send(dest, fport, frame) != EXIT_SUCCESS)
  {
    free(frame);
    return EXIT_FAILURE;
  }

  free(frame);

  return EXIT_SUCCESS;
}

/************************* end of file *************************/