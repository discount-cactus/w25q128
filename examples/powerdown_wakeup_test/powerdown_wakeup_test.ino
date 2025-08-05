//Power-down and wake-up test
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

  Serial.println("Putting flash to sleep...");
  flash.powerDown();
  delay(100);

  Serial.print("JEDEC ID during sleep: 0x");
  Serial.println(flash.readJEDECID(), HEX);  // Expect: 0x000000 or garbage

  Serial.println("Waking up flash...");
  flash.wakeUp();
  delay(100);

  Serial.print("JEDEC ID after wake: 0x");
  Serial.println(flash.readJEDECID(), HEX);  // Expect: 0xEF4018 again
}

void loop(){
}