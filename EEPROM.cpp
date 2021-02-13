#include "EEPROM.h"

#include <avr/interrupt.h>

EEPROMClass& EEPROM = EEPROMClass::instance();

ISR(EE_READY_vect)
{
    if (EEPROM.length_-- > 0)
    {
        // Operation ongoing, trigger a new write.
        EEAR = EEPROM.address_++;
        EEDR = *EEPROM.index_++;
        EECR |= _BV(EEMPE);
        EECR |= _BV(EEPE);
    }
    else
    {
        // Operation finished, disable interrupt and invoke callback.
        EECR &= ~_BV(EERIE);
        EEPROM.callback_();
    }
}