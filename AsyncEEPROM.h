/*
  EEPROM.h - Library for interacting with EEPROM  asynchronously.
  Created by Bruno Bousquet, February 12, 2021.
  Released into the public domain.
*/

#ifndef ASYNC_EEPROM_H
#define ASYNC_EEPROM_H

#include <avr/io.h>

// Function to call when no callback is used.
static void emptyCallback() {}

// Forward declare the EEPROM interrupt handler, for future use
extern "C" void EE_READY_vect();

class EEPROMClass
{
    constexpr EEPROMClass()
        : index_{0}, length_{0}, address_{0}, callback_{emptyCallback}
    {
        // Empty
    }

public:
    static EEPROMClass& instance()
    {
        // Declare a static variable to hold the EEPROM class.
        // A static variable gets allocated for the lifetime of the program.
        static EEPROMClass instance_;

        // Return a reference to the static variable.
        return instance_;
    }

    void setCallback(void (*callback)())
    {
        // If the given function pointer is null, set the active callback to emptyCallback
        callback_ = (callback == nullptr) ? emptyCallback : callback;
    }

    bool isBusy() { return EECR & _BV(EEPE); }

    void write(uint16_t address, uint8_t data)
    {
        // Declare a static buffer to hold the single byte to write to EEPROM
        static uint8_t temp_buffer;

        temp_buffer = data;

        // Start the async write to EEPROM
        write(address, &temp_buffer, 1);
    }

    void write(uint16_t address, uint8_t* data, uint16_t length)
    {
        if (isBusy())
        {
            // EEPROM is busy, abort write
        }
        else
        {
            address_ = address;
            length_ = length;
            index_ = data;
            EECR |= _BV(EERIE); // Trigger first write by enabling EEPROM interrupt
        }
    }

    uint8_t read(uint16_t address)
    {
        if (isBusy())
        {
            // Currently writing to memory, cannot read, return random value.
            return 0xff;
        }
        else
        {
            EEAR = address;   // Set address to read from
            EECR = _BV(EERE); // Trigger read command
            return EEDR;      // Return read command
        }
    }

private:
    uint8_t* index_ = 0;
    uint16_t length_ = 0;
    uint16_t address_ = 0;
    void (*callback_)() = emptyCallback;

    // Grant private member access to EEPROM interrupt handler
    friend void EE_READY_vect();
};

extern EEPROMClass& EEPROM;

#endif // ASYNC_EEPROM_H
