# W25Q128x SPI Flash Arduino Library

An Arduino library for interfacing with the W25Q128x SPI flash memory chip from Winbond. The library provides low-level access for reading, writing, erasing, and managing power modes.

## Features

- JEDEC ID retrieval
- Single and multi-byte read/write
- Automatic page boundary handling (256-byte pages)
- Sector (4KB), block (32KB, 64KB), and chip erase
- Busy polling to avoid premature access
- Low-power and wake-up modes

## Getting Started

### Include the Library
```cpp
#include <SPI.h>
#include "W25Q128.h"
```

### Initialize
```cpp
W25Q128 flash;

void setup() {
  Serial.begin(115200);
  flash.begin(cs_pin);
}
```

---

## API Reference

### `bool begin(uint8_t csPin)`
Initializes the flash chip.

- **`csPin`**: The Arduino pin connected to the chip's CS (Chip Select) pin.  
  Must be a digital output pin.  

---

### `uint32_t readJEDECID()`
Reads the JEDEC manufacturer and device ID.  
No parameters. Returns a 24-bit ID in the format:

```
[Manufacturer ID] [Memory Type] [Capacity]
```
Example for W25Q128: `0xEF4018`

---

### `bool isBusy()`
Checks if the chip is performing a write/erase.

- **Returns**:  
  - `true` → chip is busy  
  - `false` → chip is idle  

---

### `uint8_t readByte(uint32_t address)`
Reads a single byte from memory.

- **`address`**: 24-bit memory address (0 to chip size - 1).  
- **Returns**: The byte stored at that address.

---

### `void read(uint32_t address, uint8_t* buffer, size_t length)`
Reads multiple bytes into a buffer.

- **`address`**: Start address to read from.  
- **`buffer`**: Pointer to an array where data will be stored.  
- **`length`**: Number of bytes to read.  

---

### `void writeByte(uint32_t address, uint8_t value)`
Writes a single byte to memory.

- **`address`**: Destination address.  
- **`value`**: Byte to write.  

---

### `void write(uint32_t address, const uint8_t* data, size_t length)`
Writes multiple bytes to memory, splitting automatically at page boundaries (256 bytes).

- **`address`**: Start address for writing.  
- **`data`**: Pointer to the array of bytes to write.  
- **`length`**: Number of bytes to write.  

---

### `void eraseSector(uint32_t address)`
Erases a **4KB sector** that contains the given address.

- **`address`**: Any address within the sector to erase.  

---

### `void eraseBlock32K(uint32_t address)`
Erases a **32KB block** that contains the given address.

- **`address`**: Any address within the block to erase.  

---

### `void eraseBlock64K(uint32_t address)`
Erases a **64KB block** that contains the given address.

- **`address`**: Any address within the block to erase.  

---

### `void eraseChip()`
Erases the **entire chip**. **Takes several seconds**.  
No parameters.

---

### `void powerDown()`
Puts the chip into **low-power mode** to reduce consumption.  
No parameters.  

---

### `void wakeUp()`
Wakes the chip from **low-power mode**.  
No parameters.

---

## Notes

- Addresses must be within the chip’s capacity (W25Q128 → `0x000000` to `0xFFFFFF`).
- Erase commands affect **entire sectors or blocks** — partial erasing is not possible.
- Writes must occur on erased memory (0xFF-filled bytes).
- Page writes wrap automatically at 256-byte boundaries.
