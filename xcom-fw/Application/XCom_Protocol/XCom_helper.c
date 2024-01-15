/************************* include section *************************/

#include "XCom_helper.h"

/************************* definition section *************************/

void info_log(int mode, const char *str, ...)
{
  if (mode == TRACE_LEVEL)
  {
    va_list ap;
    va_start(ap, str);
    vprintf(str, ap);
    va_end(ap);
  }
}

void panic(const char *msg, ...)
{
  va_list ap;

  va_start(ap, msg);
  fprintf(stderr, "Error : ");
  vfprintf(stderr, msg, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

uint16_t b2swap(uint16_t val)
{
#if ENDIAN_ORDER == BIG_ENDIAN_VAL
  return val;
#elif ENDIAN_ORDER == LITTLE_ENDIAN_VAL
    return ((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8);
#endif
}

uint32_t b4swap(uint32_t val)
{
#if ENDIAN_ORDER == BIG_ENDIAN_VAL
  return val;
#elif ENDIAN_ORDER == LITTLE_ENDIAN_VAL
    return ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8)
        | ((val & 0x0000FF00) << 8) | ((val & 0x000000FF) << 24);

#endif
}

uint64_t b8swap(uint64_t val)
{
#if ENDIAN_ORDER == BIG_ENDIAN_VAL
  return val;
#elif ENDIAN_ORDER == LITTLE_ENDIAN_VAL
    return ((val & 0xFF00000000000000) >> 56)
        | ((val & 0x00FF000000000000) >> 40)
        | ((val & 0x0000FF0000000000) >> 24)
        | ((val & 0x000000FF00000000) >> 8)
        | ((val & 0x00000000FF000000) << 8)
        | ((val & 0x0000000000FF0000) << 24)
        | ((val & 0x000000000000FF00) << 40)
        | ((val & 0x00000000000000FF) << 56);

#endif
}

uint8_t convertByteArrayTo8(const uint8_t *data)
{
  uint8_t res = 0;

  res = (res << 8) | data[0];

  return res;
}

uint16_t convertByteArrayTo16(const uint8_t *data)
{
  uint8_t res = 0;
  int     i;

  for (i = 0; i < 2; i++)
  {
    res = (res << 8) | data[i];
  }

  return res;
}

uint32_t convertByteArrayTo32(const uint8_t *data)
{
  uint32_t res = 0;
  int      i;

  for (i = 0; i < 4; i++)
  {
    res = (res << 8) | data[i];
  }

  return res;
}

uint64_t convertByteArrayTo64(const uint8_t *data)
{
  uint64_t res = 0;
  int      i;

  for (i = 0; i < 8; i++)
  {
    res = (res << 8) | data[i];
  }

  return res;
}

void convert8ToByteArray(uint8_t var, uint8_t *data) { data[0] = var & 0xFF; }

void convert16ToByteArray(uint16_t var, uint8_t *data)
{
  int i;

  var = b2swap(var);

  for (i = 0; i < 2; i++)
  {
    data[i] = var & 0xFF;
    var     = var >> 8;
  }
}

void convert32ToByteArray(uint32_t var, uint8_t *data)
{
  int i;

  var = b4swap(var);

  for (i = 0; i < 4; i++)
  {
    data[i] = var & 0xFF;
    var     = var >> 8;
  }
}

void convert64ToByteArray(uint64_t var, uint8_t *data)
{
  int i;

  var = b8swap(var);

  for (i = 0; i < 8; i++)
  {
    data[i] = var & 0xFF;
    var     = var >> 8;
  }
}

/************************* end of file *************************/