#ifndef XCOM_HELPER_H
#define XCOM_HELPER_H

/************************* include section *************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "XCom_callbacks.h"

/************************* define section *************************/

#define RELEASE     0
#define WARNING     1
#define DEBUG       2
#define TRACE_LEVEL RELEASE

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define LITTLE_ENDIAN_VAL 0x41424344UL
#define BIG_ENDIAN_VAL    0x44434241UL
#define ENDIAN_ORDER      ('ABCD')

#define CHK_SUCCESS(op)       \
  do                          \
  {                           \
    if ((op) != EXIT_SUCCESS) \
    {                         \
      panic(#op);             \
      return EXIT_FAILURE;    \
    }                         \
  } while (0)

#define CHK_RET(op)        \
  do                       \
  {                        \
    if ((op) <= -1)        \
    {                      \
      panic(#op);          \
      return EXIT_FAILURE; \
    }                      \
  } while (0)

#define CHK_SIZE(s1, s2, str) \
  do                          \
  {                           \
    if (s1 < s2)              \
    {                         \
      panic(str);             \
      return EXIT_FAILURE;    \
    }                         \
  } while (0)

#define CHK_PTR(op)        \
  do                       \
  {                        \
    if ((op) == NULL)      \
    {                      \
      panic(#op);          \
      return EXIT_FAILURE; \
    }                      \
  } while (0)

/************************* definition section *************************/

void info_log(int mode, const char *str, ...);
void panic(const char *msg, ...);

uint16_t b2swap(uint16_t val);
uint32_t b4swap(uint32_t val);
uint64_t b8swap(uint64_t val);

uint8_t  convertByteArrayTo8(const uint8_t *data);
uint16_t convertByteArrayTo16(const uint8_t *data);
uint32_t convertByteArrayTo32(const uint8_t *data);
uint64_t convertByteArrayTo64(const uint8_t *data);

void convert8ToByteArray(uint8_t var, uint8_t *data);
void convert16ToByteArray(uint16_t var, uint8_t *data);
void convert32ToByteArray(uint32_t var, uint8_t *data);
void convert64ToByteArray(uint64_t var, uint8_t *data);

#endif

/************************* end of file *************************/