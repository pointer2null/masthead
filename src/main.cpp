#include <Arduino.h>
#include <Wire.h>

int windSpeedPin = DD2;
bool pulse = false;
unsigned char pcount = 0;
byte rq[2];

void sendData()
{
    // If i don't print anything here the Pi Zero always receives this byte with the 
    // MSB set. I've tried everything but this is the only solution.
    // It does not affect a PI 4, just the Zero.
    Serial.print("Sending count ");
    Serial.print(pcount);
    // we always send exactly the same data on each and every request
    int sent = Wire.write(&pcount, 1);

    Serial.print(". Sent ");
    Serial.print(sent);
    Serial.println(" bytes");

    pcount = 0;
}

void count()
{
    pcount++;
    pulse = true;
    Serial.print("Count ");
    Serial.println(pcount);
}

void setup()
{
    Serial.begin(9600);
    // set the two input pins. The speed pulse requires a pull up.
    digitalWrite(SDA, 1);
    digitalWrite(SCL, 1);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(windSpeedPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(windSpeedPin), count, FALLING);
    // Wire.setClock(5000L);
    Wire.begin(0x55); // this is the IIC address we will be responding to
    Wire.onRequest(sendData);
}

void loop()
{
    if (pulse) {
        pulse = false;
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
    }
}
