
#include "AsyncEEPROM.h"

uint8_t buffer[] = {
    0,1,2,3,4,5,6,7
};

void setup()
{
    Serial.begin(9600);

    delay(1000);

    EEPROM.setCallback([](){
        Serial.println("Write ended");
    });

    Serial.println("Write begins");
    EEPROM.write(0, buffer, 8);

    Serial.println("Waiting...");
    while(EEPROM.isBusy())
    {
        // wait
    }

    bool success = true;
    for(int i = 0; i < 8; i++)
    {
        success &= buffer[i] == EEPROM.read(i);
    }

    if (success)
    {
        Serial.println("Success!");
    }
    else
    {
        Serial.println("Test failed");
    }
}

void loop()
{

}