/*
  EEPROM.cpp - Library for interacting with EEPROM  asynchronously.
  Created by Bruno Bousquet, February 12, 2021.
  Released into the public domain.
*/

#include "AsyncEEPROM.h"

#include <avr/io.h>
#include <avr/interrupt.h>

EEPROMClass& EEPROM = EEPROMClass::instance();

ISR(EE_READY_vect)
{
    if (EEPROM.length_-- > 0)
    {
        // There's still values in the buffer, trigger a new write.
        EEAR = EEPROM.address_++;
        EEDR = *EEPROM.index_++;
        EECR |= _BV(EEMPE); // Stage write to EEPROM command
        EECR |= _BV(EEPE);  // Start write to EEPROM command
    }
    else
    {
        // Operation finished, disable interrupt and invoke callback.
        EECR &= ~_BV(EERIE);
        EEPROM.callback_();
    }
}