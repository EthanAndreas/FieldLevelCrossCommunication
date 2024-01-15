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
----------------------------------------------------------------------
File    : SEGGER_SYS_Win32.c
Purpose : Implementation for API functions.
Revision: $Rev: 21646 $
--------  END-OF-HEADER  ---------------------------------------------
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER_SYS.h"
#include <stdio.h>
#include <string.h>

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static char _aCompilerID[40];

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

const char * SEGGER_SYS_GetCompiler(void) {
#if defined(__INTEL_COMPILER)
  #if defined(_W64)
    sprintf(_aCompilerID, "Intel C %d.%d.%d [x64]", __INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE);
  #else
    sprintf(_aCompilerID, "Intel C %d.%d.%d [x86]", __INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE);
#endif
#elif defined(__SEGGER_CC__)
  sprintf(_aCompilerID, "SEGGER cc %d.%d.%d", __SEGGER_CC__, __SEGGER_CC_MINOR__, __SEGGER_CC_PATCHLEVEL__);
#elif defined(__clang__)
  snprintf(_aCompilerID, sizeof(_aCompilerID), "clang %s", __clang_version__);
#elif defined(__GNUC__)
  sprintf(_aCompilerID, "gcc %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(__IAR_SYSTEMS_ICC__)
  sprintf(_aCompilerID, "IAR icc %d.%d.%d", __VER__ / 1000000, __VER__ / 1000 % 1000, __VER__ % 1000);
#elif defined(_MSC_FULL_VER)
  #if defined(_WIN64)
    sprintf(_aCompilerID, "MSVC %d.%02d.%d [x64]", _MSC_FULL_VER / 10000000, _MSC_FULL_VER / 100000 % 100, _MSC_FULL_VER % 100000);
  #else
    sprintf(_aCompilerID, "MSVC %d.%02d.%d [x86]", _MSC_FULL_VER / 10000000, _MSC_FULL_VER / 100000 % 100, _MSC_FULL_VER % 100000);
  #endif
#else
  strcpy(_aCompilerID, "Cannot be identified");
#endif
  return _aCompilerID;
}

const char * SEGGER_SYS_GetIDE(void) {
#if defined(_MSC_VER)
  switch (_MSC_VER) {
  case 1000: return "Developer Studio 4.0";
  case 1020: return "Developer Studio 4.2";
  case 1100: return "Visual Studio 97 version 5.0";
  case 1200: return "Visual Studio 6.0 version 6.0";
  case 1300: return "Visual Studio .NET 2002 version 7.0";
  case 1310: return "Visual Studio .NET 2003 version 7.1";
  case 1400: return "Visual Studio 2005 version 8.0";
  case 1500: return "Visual Studio 2008 version 9.0";
  case 1600: return "Visual Studio 2010 version 10.0";
  case 1700: return "Visual Studio 2012 version 11.0";
  case 1800: return "Visual Studio 2013 version 12.0";
  case 1900: return "Visual Studio 2015 version 14.0";
  case 1910: return "Visual Studio 2017 version 15.0";
  case 1911: return "Visual Studio 2017 version 15.3";
  case 1912: return "Visual Studio 2017 version 15.5";
  case 1913: return "Visual Studio 2017 version 15.6";
  case 1914: return "Visual Studio 2017 version 15.7";
  case 1915: return "Visual Studio 2017 version 15.8";
  case 1916: return "Visual Studio 2017 version 15.9";
  case 1920: return "Visual Studio 2019 Version 16.0";
  case 1921: return "Visual Studio 2019 Version 16.1";
  default:   return "Visual Studio, version unknown";
  }
#elif defined __SES_VERSION
  static const char _acVersion[] = {
    'S', 'E', 'G', 'G', 'E', 'R', ' ',
    'E', 'm', 'b', 'e', 'd', 'd', 'e', 'd', ' ',
    'S', 't', 'u', 'd', 'i', 'o', ' ',
    '0' + (__SES_VERSION / 10000),
    '.',
    '0' + (__SES_VERSION /  1000 % 10),
    '0' + (__SES_VERSION /   100 % 10),
    (__SES_VERSION % 100 != 0) ? 'a' + (__SES_VERSION % 100 - 1) : '\0',
    '\0'
    };
  //
  return _acVersion;
#elif defined __MCUXPRESSO
  return "MCUXpresso";
#else
  return "Cannot be identified";
#endif
}
