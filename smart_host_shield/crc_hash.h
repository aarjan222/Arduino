#ifndef _CRC_HASH_
#define _CRC_HASH_

#include "Arduino.h"

#define WIDTH (8)
#define CRC_HASH_TABLE_SIZE (256)
#define TOP_BIT (1 << 7)

class CRC_Hash {
public:
  CRC_Hash();
  CRC_Hash(CRC_Hash &&) = default;
  CRC_Hash(const CRC_Hash &) = default;
  CRC_Hash &operator=(CRC_Hash &&) = default;
  CRC_Hash &operator=(const CRC_Hash &) = default;
  ~CRC_Hash() {}

  void begin(uint8_t polynomial);
  uint8_t get_Hash(uint8_t *buf, uint16_t len);

private:
  uint8_t table_[CRC_HASH_TABLE_SIZE];
};

#endif
