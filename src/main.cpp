#include <Arduino.h>
#include <Wire.h>

struct I2cData {
    int32_t count; // pulse count since the last read 4 bytes
};

I2cData txData = I2cData { count : 0 };

int windSpeedPin = DD2;

void sendData()
{
    // we always send exactly the same data on each and every request
    Wire.write((byte*)&txData, sizeof(txData));
    Serial.print("Count:");
    Serial.println(txData.count);
    txData.count = 0;
}

void count()
{
    txData.count++;
    Serial.println(txData.count);
}

void setup()
{
    Serial.begin(9600);
    // set the two input pins. The speed pulse requires a pull up.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(windSpeedPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(windSpeedPin), count, FALLING);
    Wire.begin(0x55); // this is the IIC address we will be responding to
    Wire.onRequest(sendData);
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
