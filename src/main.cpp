#include <Arduino.h>
#include <Wire.h>

struct I2cData {
    int32_t count; // pulse count since the last read
};

I2cData txData = I2cData { count : 0 };

int windSpeedPin = DD2;
bool pulse = false;

void sendData()
{
    // we always send exactly the same data on each and every request
    Wire.write((byte*)&txData, sizeof(txData));
    Serial.print("Bytes:");
    Serial.println(int(sizeof(txData)));
    Serial.print("Sending:");
    Serial.println(txData.count);
    txData.count = 0;
}

void count()
{
    txData.count++;
    pulse = true;
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
    if (pulse) {
        digitalWrite(LED_BUILTIN, HIGH);
        pulse = false;
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
    }
    
}
