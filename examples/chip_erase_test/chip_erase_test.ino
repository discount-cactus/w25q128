#include <W25Q128.h>

#define CS_pin 10

W25Q128 flash;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("W25Q128 Test");

  flash.begin(CS_pin);
  Serial.print("JEDEC ID: 0x");
  Serial.println(flash.readJEDECID(), HEX);

  uint32_t testAddr = 0x000100;
  flash.writeByte(testAddr, 0xA5);
  while (flash.isBusy());
  Serial.print("Wrote 0xA5, read back: 0x");
  Serial.println(flash.readByte(testAddr), HEX);  // Expect 0xA5

  //Now erase entire chip
  Serial.println("Erasing chip...");
  flash.eraseChip();  //This will take ~10s

  //Check if erased
  Serial.print("After chip erase, read: 0x");
  Serial.println(flash.readByte(testAddr), HEX);  // Expect 0xFF
}

void loop() {}