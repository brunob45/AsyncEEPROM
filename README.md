# Arduino Async EEPROM Library

## Write to EEPROM asynchronously!

This library is using the EEPROM Ready interrupt to be as efficient as possible.

Currently only tested on Arduino Mega2560

Available functions:

- EEPROM.write(address, data): Write a single byte to EEPROM
- EEPROM.write(address, data, length): Write an array of bytes to EEPROM
- EEPROM.setCallback(callback): Set which function to call when the write operation completes
- EEPROM.isBusy(): Get the current state of operation
- EEPROM.read(address): Read a single byte from EEPROM
