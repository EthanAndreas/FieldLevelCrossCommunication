/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2021  SEGGER Microcontroller GmbH                 *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : Webserver_DynContent.c
Purpose : Dynamic content for emWeb.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BSP.h"
#include "Configuration.h"
#include "IP.h"
#include "IP_Webserver.h"
#include "RTOS.h"
#include "Task.h"
#include "UDP_send.h"
#include "WEBS_Conf.h"
#include "XCom_protocol.h"

/*********************************************************************
 *
 *       Defines, fixed
 *
 **********************************************************************
 */

#define WAIT_PING_MS 200

/*********************************************************************
 *
 *       Static data
 *
 **********************************************************************
 */

//
// Variables used for CGI samples.
//
int sensorNb = 0;

// Store the boards configuration
static char sensorIPaddr[3][16];

static char sensorRole[3][8];

static int sensorHeightValue[3] = {0};

// Store the tank configuration
int         tankDiameterValue = 0;
static char tankDiameterUnit[3];
int         tankVolumeValue = 0;
static char tankVolumeUnit[3];
int         tankHeightValue = 0;
static char tankHeightUnit[3];

// Store the fluid configuration
int         fluidDensityValue = 0;
static char fluidDensityUnit[5];
int         fluidGasPressureValue = 0;
static char fluidGasPressureUnit[4];

// Store the warning configuration
int timeValue = 0;
int frameLoss = 0;

// Monitoring App Config
static char monitoringAppIPaddr[16];

/******************************************************************************************************************************************
 *
 *       Custom error pages
 *
 *******************************************************************************************************************************************
 */

/*********************************************************************
 *
 *       cbSendCustomErrorPage()
 *
 *  Function description
 *    Callback notified in case an error page shall be sent.
 *
 *  Parameters
 *    pInfo: Structure of type WEBS_SEND_ERROR_PAGE_INFO that contains
 *           more information like the connection context.
 *
 *  Return value
 *    Custom error page sent              : WEBS_OK
 *    Let the server sent its default page: WEBS_NO_ERROR_PAGE_SENT
 */
static int cbSendCustomErrorPage(WEBS_SEND_ERROR_PAGE_INFO *pInfo)
{
  int r;

  r = WEBS_NO_ERROR_PAGE_SENT;

  switch (pInfo->ErrorCode)
  {
    case 404u:
      (void)IP_WEBS_Redirect(pInfo->pOutput, "/Error404.htm", NULL);
      r = WEBS_OK;
      break;
    default:
      break;
  }
  return r;
}

/******************************************************************************************************************************************
 *
 *       CGI sample functions
 *
 *******************************************************************************************************************************************
 */

/*********************************************************************
 *
 *       CopyString
 *
 *  Function description
 *    Local helper function. Copies a string.
 */
static void CopyString(char *sDest, const char *sSrc, int DestSize)
{
  char c;

  while (--DestSize > 0)
  {
    c = *sSrc++;
    if (c == 0)
    {
      break;
    }
    *sDest++ = c;
  }
  *sDest = 0;
}

/*********************************************************************
 *
 *       IP_Str_Byte
 */
uint32_t IP_Str_Byte(char *sensorIPaddr)
{
  int ipBytes[4] = {0};
  if (sscanf(sensorIPaddr, "%d.%d.%d.%d", &ipBytes[0], &ipBytes[1],
             &ipBytes[2], &ipBytes[3]) != 4)
  {
    IP_Log("Error IP address is not valid : ");
    IP_Log(sensorIPaddr);
  }
  return IP_BYTES_TO_UINT32(ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]);
}

/*********************************************************************
 *
 *       Role_Str_Byte
 */
uint8_t Role_Str_Byte(char *role)
{
  if (strcmp(role, "low") == 0)
  {
    return ROLE_LOW;
  }
  else if (strcmp(role, "high") == 0)
  {
    return ROLE_HIGH;
  }
  else if (strcmp(role, "density") == 0)
  {
    return ROLE_DENSITY;
  }
  return 0;
}

/*********************************************************************
 *
 *       Length_Unit_Str_Byte
 */
uint8_t Length_Unit_Str_Byte(char *lengthUnit)
{
  if (strcmp(lengthUnit, "m") == 0)
  {
    return LENGTH_UNIT_METER;
  }
  else if (strcmp(lengthUnit, "ft") == 0)
  {
    return LENGTH_UNIT_FEET;
  }
  else if (strcmp(lengthUnit, "in") == 0)
  {
    return LENGTH_UNIT_INCH;
  }
  return 0;
}

/*********************************************************************
 *
 *       Volume_Unit_Str_Byte
 */
uint8_t Volume_Unit_Str_Byte(char *tankVolumeUnit)
{
  if (strcmp(tankVolumeUnit, "m3") == 0)
  {
    return VOLUME_UNIT_METER3;
  }
  else if (strcmp(tankVolumeUnit, "l") == 0)
  {
    return VOLUME_UNIT_LITER;
  }
  else if (strcmp(tankVolumeUnit, "gal") == 0)
  {
    return VOLUME_UNIT_GALLON;
  }
  return 0;
}

/*********************************************************************
 *
 *       Density_Unit_Str_Byte
 */
uint8_t Density_Unit_Str_Byte(char *fluidDensityUnit)
{
  if (strcmp(fluidDensityUnit, "kgm3") == 0)
  {
    return DENSITY_UNIT_KGBYMETER3;
  }
  else if (strcmp(fluidDensityUnit, "gcm3") == 0)
  {
    return DENSITY_UNIT_GBYCM3;
  }
  return 0;
}

/*********************************************************************
 *
 *       Pressure_Unit_Str_Byte
 */
uint8_t Pressure_Unit_Str_Byte(char *pressureUnit)
{
  if (strcmp(pressureUnit, "bar") == 0)
  {
    return PRESSURE_UNIT_BAR;
  }
  else if (strcmp(pressureUnit, "pa") == 0)
  {
    return PRESSURE_UNIT_PASCAL;
  }
  else if (strcmp(pressureUnit, "psi") == 0)
  {
    return PRESSURE_UNIT_PSI;
  }
  return 0;
}

/*********************************************************************
 *
 *       Fill_confStruct
 */
int Fill_confStruct(void)
{
  // Set this board as main board
  Set_isMain();

  // Fill tank conf
  Set_confTank(tankDiameterValue, Length_Unit_Str_Byte(tankDiameterUnit),
               tankVolumeValue, Volume_Unit_Str_Byte(tankVolumeUnit),
               tankHeightValue, Length_Unit_Str_Byte(tankHeightUnit));

  // Fill sensor infos
  // Set the conf of the current
  Set_confSensor(sensorNb, IP_Str_Byte(sensorIPaddr[0]),
                 Role_Str_Byte(sensorRole[0]), TYPE_MAIN,
                 sensorHeightValue[0], LENGTH_UNIT_METER);
  for (int i = 1; i < sensorNb; i++)
  {
    // Ping board
    if (IP_SendPingCheckReply(0, IP_Str_Byte(sensorIPaddr[i]), NULL, NULL,
                              WAIT_PING_MS) != 0)
    {
      IP_Log("Error board did not answer to ping : ");
      IP_Log(sensorIPaddr[i]);
      return 1;
    }
    IP_Log("Ping OK");

    // Set the conf of the sensor
    Set_confSensor(sensorNb, IP_Str_Byte(sensorIPaddr[i]),
                   Role_Str_Byte(sensorRole[i]), TYPE_SECONDARY,
                   sensorHeightValue[i], LENGTH_UNIT_METER);
  }

  // Fill fluid conf
  Set_confFluid(fluidDensityValue, Density_Unit_Str_Byte(fluidDensityUnit),
                fluidGasPressureValue,
                Pressure_Unit_Str_Byte(fluidGasPressureUnit));

  // Fill cycle conf
  Set_confCycle(timeValue, TIME_UNIT_MILLISECOND, frameLoss);

  // Fill monitoring app conf
  if (monitoringAppIPaddr[0] != '\0')
  {
    Set_monitoringAppIP(IP_Str_Byte(monitoringAppIPaddr));
  }

  // Send data to others sensors (start from 1 like 0 is the current board)
  for (int i = 1; i < sensorNb; i++)
  {
    Send_XCom_Conf_Send_Sensor(IP_Str_Byte(sensorIPaddr[i]), XCOM_PORT, sensorNb,
                               IP_Str_Byte(sensorIPaddr[0]),
                               Role_Str_Byte(sensorRole[0]), TYPE_MAIN,
                               sensorHeightValue[0], LENGTH_UNIT_METER);
    for (int j = 1; j < sensorNb; j++)
    {
      // Send sensor conf
      Send_XCom_Conf_Send_Sensor(
          IP_Str_Byte(sensorIPaddr[i]), XCOM_PORT, sensorNb,
          IP_Str_Byte(sensorIPaddr[j]), Role_Str_Byte(sensorRole[j]),
          TYPE_SECONDARY, sensorHeightValue[j], LENGTH_UNIT_METER);
    }

    // Send tank conf
    Send_XCom_Conf_Send_Tank(
        IP_Str_Byte(sensorIPaddr[i]), XCOM_PORT, tankDiameterValue,
        Length_Unit_Str_Byte(tankDiameterUnit), tankVolumeValue,
        Volume_Unit_Str_Byte(tankVolumeUnit), tankHeightValue,
        Length_Unit_Str_Byte(tankHeightUnit));

    // Send fluid conf
    Send_XCom_Conf_Send_Fluid(IP_Str_Byte(sensorIPaddr[i]), XCOM_PORT,
                              fluidDensityValue, DENSITY_UNIT_KGBYMETER3, 0,
                              0);

    // Send cycle conf
    Send_XCom_Conf_Cycle(IP_Str_Byte(sensorIPaddr[i]), XCOM_PORT, timeValue,
                         TIME_UNIT_MILLISECOND, frameLoss);
  }
  // Config is set
  Set_isSetConf();

  // Set task event
  Set_taskEvent(EVENT_USERSETCONF);

  return 0;
}

/*********************************************************************
 *
 *       cb_GetIP1
 */
static void cb_GetIP1(WEBS_OUTPUT *pOutput, const char *sPara,
                      const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(sensorIPaddr[0], sValue, sizeof(sensorIPaddr[0]));
    sensorNb++;
  }
}

/*********************************************************************
 *
 *       cb_GetIP2
 */
static void cb_GetIP2(WEBS_OUTPUT *pOutput, const char *sPara,
                      const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(sensorIPaddr[1], sValue, sizeof(sensorIPaddr[1]));
    sensorNb++;
  }
}

/*********************************************************************
 *
 *       cb_GetIP3
 */
static void cb_GetIP3(WEBS_OUTPUT *pOutput, const char *sPara,
                      const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(sensorIPaddr[2], sValue, sizeof(sensorIPaddr[2]));
    sensorNb++;
  }
}

/*********************************************************************
 *
 *       cb_GetSensorRole1
 */
static void cb_GetSensorRole1(WEBS_OUTPUT *pOutput, const char *sPara,
                              const char *sValue)
{
  if (sValue != NULL)
    CopyString(sensorRole[0], sValue, sizeof(sensorRole[0]));
}

/*********************************************************************
 *
 *       cb_GetSensorRole2
 */
static void cb_GetSensorRole2(WEBS_OUTPUT *pOutput, const char *sPara,
                              const char *sValue)
{
  if (sValue != NULL)
    CopyString(sensorRole[1], sValue, sizeof(sensorRole[1]));
}

/*********************************************************************
 *
 *       cb_GetSensorRole3
 */
static void cb_GetSensorRole3(WEBS_OUTPUT *pOutput, const char *sPara,
                              const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(sensorRole[2], sValue, sizeof(sensorRole[2]));
  }
}

/*********************************************************************
 *
 *       cb_GetSensorHeightValue1
 */
static void cb_GetSensorHeightValue1(WEBS_OUTPUT *pOutput, const char *sPara,
                                     const char *sValue)
{
  if (sValue == NULL)
  {
    sensorHeightValue[0] = 0;
  }
  else
  {
    sensorHeightValue[0] = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetSensorHeightValue2
 */
static void cb_GetSensorHeightValue2(WEBS_OUTPUT *pOutput, const char *sPara,
                                     const char *sValue)
{
  if (sValue == NULL)
  {
    sensorHeightValue[1] = 0;
  }
  else
  {
    sensorHeightValue[1] = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetSensorHeightValue3
 */
static void cb_GetSensorHeightValue3(WEBS_OUTPUT *pOutput, const char *sPara,
                                     const char *sValue)
{
  if (sValue == NULL)
  {
    sensorHeightValue[2] = 0;
  }
  else
  {
    sensorHeightValue[2] = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetTankDiameterValue
 */
static void cb_GetTankDiameterValue(WEBS_OUTPUT *pOutput, const char *sPara,
                                    const char *sValue)
{
  if (sValue != NULL)
  {
    tankDiameterValue = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetTankHeightValue
 */
static void cb_GetTankHeightValue(WEBS_OUTPUT *pOutput, const char *sPara,
                                  const char *sValue)
{
  if (sValue != NULL)
  {
    tankHeightValue = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetTankVolumeValue
 */
static void cb_GetTankVolumeValue(WEBS_OUTPUT *pOutput, const char *sPara,
                                  const char *sValue)
{
  if (sValue != NULL)
  {
    tankVolumeValue = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetTankDiameterUnit
 */
static void cb_GetTankDiameterUnit(WEBS_OUTPUT *pOutput, const char *sPara,
                                   const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(tankDiameterUnit, sValue, sizeof(tankDiameterUnit));
  }
}

/*********************************************************************
 *
 *       cb_GetTankHeightUnit
 */
static void cb_GetTankHeightUnit(WEBS_OUTPUT *pOutput, const char *sPara,
                                 const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(tankHeightUnit, sValue, sizeof(tankHeightUnit));
  }
}

/*********************************************************************
 *
 *       cb_GetTankVolumeUnit
 */
static void cb_GetTankVolumeUnit(WEBS_OUTPUT *pOutput, const char *sPara,
                                 const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(tankVolumeUnit, sValue, sizeof(tankVolumeUnit));
  }
}

/*********************************************************************
 *
 *       cb_GetDefaultDensity
 */
static void cb_GetDefaultDensity(WEBS_OUTPUT *pOutput, const char *sPara,
                                 const char *sValue)
{
  if ((sValue != NULL) && (atoi(sValue) != 0))
  {
    fluidDensityValue = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetDefaultfluidDensityUnit
 */
static void cb_GetDefaultfluidDensityUnit(WEBS_OUTPUT *pOutput,
                                          const char  *sPara,
                                          const char  *sValue)
{
  if (sValue != NULL)
  {
    CopyString(fluidDensityUnit, sValue, sizeof(fluidDensityUnit));
  }
}

/*********************************************************************
 *
 *       _cb_GetfluidGasPressureValue
 */
static void cb_GetfluidGasPressureValue(WEBS_OUTPUT *pOutput, const char *sPara,
                                        const char *sValue)
{
  if ((sValue != NULL) && (atoi(sValue) != 0))
  {
    fluidDensityValue = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetfluidGasPressureUnit
 */
static void cb_GetfluidGasPressureUnit(WEBS_OUTPUT *pOutput, const char *sPara,
                                       const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(fluidGasPressureUnit, sValue, sizeof(fluidGasPressureUnit));
  }
}

/*********************************************************************
 *
 *       cb_GetCommCycle
 */
static void cb_GetCommCycle(WEBS_OUTPUT *pOutput, const char *sPara,
                            const char *sValue)
{
  if (sValue != NULL)
  {
    timeValue = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetframeLoss
 */
static void cb_GetframeLoss(WEBS_OUTPUT *pOutput, const char *sPara,
                            const char *sValue)
{
  if (sValue != NULL)
  {
    frameLoss = atoi(sValue);
  }
}

/*********************************************************************
 *
 *       cb_GetMonitoringAppIPaddr
 */
static void cb_GetMonitoringAppIPaddr(WEBS_OUTPUT *pOutput, const char *sPara,
                                      const char *sValue)
{
  if (sValue != NULL)
  {
    CopyString(monitoringAppIPaddr, sValue, sizeof(monitoringAppIPaddr));
  }

  // End of the form so store and send the conf
  Fill_confStruct();
}

/*********************************************************************
 *
 *       cb_DefaultHandler
 */
static void cb_DefaultHandler(WEBS_OUTPUT *pOutput, const char *sPara,
                              const char *sValue)
{
  {
    WEBS_USE_PARA(sValue);

    IP_WEBS_SendString(pOutput, "<h1 style=\"color:red\">Unknown CGI ");
    IP_WEBS_SendString(pOutput, sPara);
    IP_WEBS_SendString(pOutput, " - Ignored</h1>");
  }
}

/*********************************************************************
 *
 *       CB_HandleParameter
 */
static void CB_HandleParameter(WEBS_OUTPUT *pOutput, const char *sPara,
                               const char *sValue)
{
  WEBS_USE_PARA(pOutput);
  WEBS_USE_PARA(sPara);
  WEBS_USE_PARA(sValue);
}

/*********************************************************************
 *
 *       GetTimeDate
 *
 *  Function description:
 *    Returns current time and date.
 *
 *    Bit 0-4:   2-second count (0-29)
 *    Bit 5-10:  Minutes (0-59)
 *    Bit 11-15: Hours (0-23)
 *    Bit 16-20: Day of month (1-31)
 *    Bit 21-24: Month of year (1-12)
 *    Bit 25-31: Count of years from 1980 (0-127)
 *
 *  Note:
 *    This is a sample implementation for a clock-less system.
 *    It always returns 01 Jan 2009 00:00:00 GMT
 */
static U32 GetTimeDate(void)
{
  U32 r;
  U16 Sec, Min, Hour;
  U16 Day, Month, Year;

  Sec  = 0;    // 0 based.  Valid range: 0..59
  Min  = 0;    // 0 based.  Valid range: 0..59
  Hour = 0;    // 0 based.  Valid range: 0..23
  Day  = 1;    // 1 based.    Means that 1 is 1. Valid range is 1..31
               // (depending on month)
  Month = 1;   // 1 based.    Means that January is 1. Valid range is 1..12.
  Year  = 29;  // 1980 based. Means that 2008 would be 28.
  r     = Sec / 2 + (Min << 5) + (Hour << 11);
  r |= (U32)(Day + (Month << 5) + (Year << 9)) << 16;
  return r;
}

/*********************************************************************
 *
 *       pfGetFileInfo
 */
static void pfGetFileInfo(const char *sFilename, IP_WEBS_FILE_INFO *pFileInfo)
{
  int v;

  //
  // .cgi files are virtual, everything else is not
  //
  v                    = IP_WEBS_CompareFilenameExt(sFilename, ".cgi");
  pFileInfo->IsVirtual = v ? 0 : 1;
  //
  // .htm/.html files contain dynamic content, everything else is not
  //
  v = IP_WEBS_CompareFilenameExt(sFilename, ".htm");
  if (v != 0)
  {
    v = IP_WEBS_CompareFilenameExt(sFilename, ".html");
  }
  pFileInfo->AllowDynContent = v ? 0 : 1;
  //
  // If file is a virtual file or includes dynamic content,
  // get current time and date stamp as file time
  //
  pFileInfo->DateLastMod = GetTimeDate();
  if (pFileInfo->IsVirtual || pFileInfo->AllowDynContent)
  {
    //
    // Set expiration time and date
    //
    pFileInfo->DateExp =
        GetTimeDate();  // If "Expires" HTTP header field should be
                        // transmitted, set expiration date.
  }
  else
  {
    pFileInfo->DateExp = 0xEE210000;  // Expiration far away (01 Jan
                                      // 2099) if content is static
  }
}

/*********************************************************************
 *
 *       aAccessControl_Basic
 */
static const WEBS_ACCESS_CONTROL aAccessControl_Basic[] = {
    {"/conf/", "Login for configuration", "user:pass", NULL},
    {"/", NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL}};

/*********************************************************************
 *
 *       aCGI
 *
 *  Function description
 *    CGI table, defining callback routines for dynamic content (SSI).
 */
static const WEBS_CGI aCGI[] = {
    // Boards configuration
    {"ip1", cb_GetIP1},
    {"ip2", cb_GetIP2},
    {"ip3", cb_GetIP3},
    {"sensorRole1", cb_GetSensorRole1},
    {"sensorRole2", cb_GetSensorRole2},
    {"sensorRole3", cb_GetSensorRole3},
    {"sensorHeightValue1", cb_GetSensorHeightValue1},
    {"sensorHeightValue2", cb_GetSensorHeightValue2},
    {"sensorHeightValue3", cb_GetSensorHeightValue3},

    // Tank configuration
    {"tankDiameterValue", cb_GetTankDiameterValue},
    {"tankVolumeValue", cb_GetTankVolumeValue},
    {"tankHeightValue", cb_GetTankHeightValue},
    {"tankDiameterUnit", cb_GetTankDiameterUnit},
    {"tankVolumeUnit", cb_GetTankVolumeUnit},
    {"tankHeightUnit", cb_GetTankHeightUnit},

    // Fluid configuration
    {"defaultDensity", cb_GetDefaultDensity},
    {"customDensity", cb_GetDefaultDensity},
    {"customFluidDensityUnit", cb_GetDefaultfluidDensityUnit},

    {"defaultGasPressureValue", cb_GetfluidGasPressureValue},
    {"customGasPressure", cb_GetfluidGasPressureValue},
    {"customGasPressureUnit", cb_GetfluidGasPressureUnit},

    // Warnings configuration
    {"time", cb_GetCommCycle},
    {"frameLoss", cb_GetframeLoss},

    // Monitoring App
    {"monitoringAppIPaddr", cb_GetMonitoringAppIPaddr},

    {NULL, cb_DefaultHandler}};

/*********************************************************************
 *
 *       WebsSample_Application
 *
 *  Function description
 *    Application data table, defines all application specifics used
 *    by the web server.
 */
const WEBS_APPLICATION WebsSample_Application = {
    &aCGI[0], (WEBS_ACCESS_CONTROL *)&aAccessControl_Basic[0],
    CB_HandleParameter, NULL};

/*************************** End of file ****************************/
