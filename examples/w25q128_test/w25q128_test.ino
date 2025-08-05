//Simple read test from the chip
#include <W25Q128.h>

#define CS_pin 10

W25Q128 flash;

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  Serial.println("W25Q128 Test");
  flash.begin(CS_pin);
  uint32_t id = flash.readJEDECID();
  Serial.print("JEDEC ID: 0x");
  Serial.println(id, HEX);

  // Example: write and then read back
  uint32_t addr = 0x000100;
  flash.eraseSector(addr);
  flash.writeByte(addr, 0x5A);   // Write test value
  while (flash.isBusy());        // Wait for write to finish

  uint8_t val = flash.readByte(addr);
  Serial.print("Read Byte: 0x");
  Serial.println(val, HEX);      // Should print 0x5A

  Serial.println("Erasing chip...");
  flash.eraseChip();  // This will take ~10s
  Serial.print("After chip erase, read: 0x");
  Serial.println(flash.readByte(addr), HEX);  // Expect 0xFF
}

void loop(){}