#include "W25Q128.h"
#include <SPI.h>

static uint8_t _csPin;

#define PAGE_SIZE 256

#define CMD_JEDEC_ID       0x9F
#define CMD_READ_DATA      0x03
#define CMD_PAGE_PROGRAM   0x02
#define CMD_WRITE_ENABLE   0x06
#define CMD_SECTOR_ERASE   0x20
#define CMD_BLOCK_ERASE_32K 0x52
#define CMD_BLOCK_ERASE_64K 0xD8
#define CMD_CHIP_ERASE     0xC7
#define CMD_READ_STATUS    0x05
#define CMD_POWER_DOWN     0xB9
#define CMD_WAKE_UP        0xAB

//Assigns CS pin to control the flash chip and gets the JEDEC ID
bool W25Q128::begin(uint8_t csPin) {
    _csPin = csPin;
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    SPI.begin();
    readJEDECID();
    return true;
}

//Reads the JEDEC ID from the chip
uint32_t W25Q128::readJEDECID() {
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_JEDEC_ID);
    _manufacturerID = SPI.transfer(0x00);
    _memoryType     = SPI.transfer(0x00);
    _capacity       = SPI.transfer(0x00);
    digitalWrite(_csPin, HIGH);

    return (_manufacturerID << 16) | (_memoryType << 8) | _capacity;
}

//Returns the state of the flash chip
//Returns 0 if busy, 1 if chip is busy
bool W25Q128::isBusy() {
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_READ_STATUS);
    uint8_t status = SPI.transfer(0x00);
    digitalWrite(_csPin, HIGH);

    return (status & 0x01); // WIP bit
}

//Reads a byte of data from the chip
uint8_t W25Q128::readByte(uint32_t address) {
    uint8_t result;
    read(address, &result, 1);  // Use the multi-byte read under the hood
    return result;
}

//Writes a byte of data to the chip
void W25Q128::writeByte(uint32_t address, uint8_t value) {
    write(address, &value, 1);  // Use the multi-byte write logic
}

//Reads multiple bytes of data from the chip
void W25Q128::read(uint32_t address, uint8_t* buffer, size_t length) {
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_READ_DATA);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);

    for (size_t i = 0; i < length; i++) {
        buffer[i] = SPI.transfer(0x00);
    }

    digitalWrite(_csPin, HIGH);
}

//Writes multiple bytes of data to the chip
void W25Q128::write(uint32_t address, const uint8_t* data, size_t length) {
    size_t offset = 0;

    while (offset < length) {
        size_t pageOffset = address % PAGE_SIZE;
        size_t spaceInPage = PAGE_SIZE - pageOffset;
        size_t chunkSize = min(spaceInPage, length - offset);

        // Write Enable
        digitalWrite(_csPin, LOW);
        SPI.transfer(CMD_WRITE_ENABLE);
        digitalWrite(_csPin, HIGH);

        // Page Program
        digitalWrite(_csPin, LOW);
        SPI.transfer(CMD_PAGE_PROGRAM);
        SPI.transfer((address >> 16) & 0xFF);
        SPI.transfer((address >> 8) & 0xFF);
        SPI.transfer(address & 0xFF);

        for (size_t i = 0; i < chunkSize; i++) {
            SPI.transfer(data[offset + i]);
        }

        digitalWrite(_csPin, HIGH);

        // Wait until not busy
        while (isBusy());

        address += chunkSize;
        offset += chunkSize;
    }
}

//Erases a sector in the flash chip
void W25Q128::eraseSector(uint32_t address) {
    // Write Enable
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_WRITE_ENABLE);
    digitalWrite(_csPin, HIGH);

    // Sector Erase (4KB)
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_SECTOR_ERASE);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    digitalWrite(_csPin, HIGH);

    // Wait for erase to complete
    while (isBusy());
}

//Erases a 32k sector in the flash chip
void W25Q128::eraseBlock32K(uint32_t address) {
    // Write Enable
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_WRITE_ENABLE);
    digitalWrite(_csPin, HIGH);

    // Send Erase Block command
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_BLOCK_ERASE_32K);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    digitalWrite(_csPin, HIGH);

    // Wait until erase is complete
    while (isBusy());
}

//Erases a 64k sector in the flash chip
void W25Q128::eraseBlock64K(uint32_t address) {
    // Write Enable
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_WRITE_ENABLE);
    digitalWrite(_csPin, HIGH);

    // Send Erase Block command
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_BLOCK_ERASE_64K);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    digitalWrite(_csPin, HIGH);

    // Wait until erase is complete
    while (isBusy());
}

//Erases all contents in the flash chip
void W25Q128::eraseChip() {
    // Enable writing
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_WRITE_ENABLE);
    digitalWrite(_csPin, HIGH);

    // Send chip erase command
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_CHIP_ERASE);
    digitalWrite(_csPin, HIGH);

    // Wait for chip erase to complete
    while (isBusy());  // This can take several seconds!
}

//Powers down the chip to a low-power mode
void W25Q128::powerDown() {
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_POWER_DOWN);
    digitalWrite(_csPin, HIGH);
    delayMicroseconds(3);
}

//Powers on the chip from a low-power mode
void W25Q128::wakeUp() {
    digitalWrite(_csPin, LOW);
    SPI.transfer(CMD_WAKE_UP);
    digitalWrite(_csPin, HIGH);
    delay(1);
}
