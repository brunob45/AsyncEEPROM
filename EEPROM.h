#ifndef ASYNC_EEPROM_H
#define ASYNC_EEPROM_H

#include <stdint.h>

typedef void (*fptr_t)(void);
static void emptyCallback() {}

extern "C" void EE_READY_vect();

class EEPROMClass
{
    constexpr EEPROMClass() : index_{0}, length_{0}, address_{0}, callback_{emptyCallback} {}

    void trigger() { EECR |= _BV(EERIE); }

public:
    static EEPROMClass& instance()
    {
        static EEPROMClass instance;
        return instance;
    }

    void setCallback(fptr_t callback) { callback_ = (callback == nullptr) ? emptyCallback : callback; }

    bool isBusy() { return EECR & _BV(EEPE); }

    void write(uint16_t address, uint8_t data)
    {
        static uint8_t temp_buffer;

        temp_buffer = data;
        write(address, &temp_buffer, 1);
    }

    void write(uint16_t address, uint8_t* data, uint16_t length)
    {
        if (!isBusy())
        {
            address_ = address;
            length_ = length;
            index_ = data;
            trigger();
        }
    }

    uint8_t read(uint16_t address)
    {
        if (isBusy())
        {
            return 0xff;
        }
        else
        {
            EEAR = address;
            EECR = _BV(EERE);
            return EEDR;
        }
    }

protected:
    uint8_t* index_ = 0;
    uint16_t length_ = 0;
    uint16_t address_ = 0;
    fptr_t callback_ = emptyCallback;

    friend void EE_READY_vect();
};

extern EEPROMClass& EEPROM;

#endif // ASYNC_EEPROM_H
