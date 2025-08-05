#ifndef W25Q128_h
#define W25Q128_h

#include "Arduino.h"

class W25Q128{
  public:
      bool begin(uint8_t csPin);
      uint32_t readJEDECID();
      bool isBusy();
      uint8_t readByte(uint32_t address);
      void writeByte(uint32_t address, uint8_t value);
      void read(uint32_t address, uint8_t* buffer, size_t length);
      void write(uint32_t address, const uint8_t* data, size_t length);
      void eraseSector(uint32_t address);
      void eraseBlock32K(uint32_t address);
      void eraseBlock64K(uint32_t address);
      void eraseChip();
      void powerDown();
      void wakeUp();
  private:
      uint8_t _manufacturerID = 0;
      uint8_t _memoryType = 0;
      uint8_t _capacity = 0;
};

#endif
