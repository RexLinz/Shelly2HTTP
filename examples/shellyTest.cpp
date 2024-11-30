#include <Arduino.h>
#include <WiFi.h>
// JSON library imported from https://arduinojson.org
// used to demonstrate more complex parsing of JSON strings returned by shelly devices
#include <ArduinoJson.h>
#include "network.h"
#include "shellyDevice.h"
#include "myShelly.h" // IP addresses and passwords

ShellyPlugPlusS    plugS_1(PLUGS1IP, ShellyPASS);
ShellyPlus1PM      shelly1_1(SHELLY11IP); // without authentication
ShellyPlus2PMcover blindControl(BLINDIP, ShellyPASS);
ShellyPro3EM3phase gridSupply(GRIDIP, ShellyPASS);

void setup() 
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("========================");
    Serial.println("ESP SHELLY test starting");
    Serial.println();
    while (Serial.available())
        Serial.read(); // clear input

    setupNetwork(); // prepare WiFi networks to be connected
    while (!connectWiFi(10000))
        ; // do not continue until WiFi is connected
    plugS_1.name      = "PlugS #1";
    shelly1_1.name    = "Shelly1PM #1";
    blindControl.name = "Living room blinds";
    gridSupply.name   = "EM3 grid supply";
}

void loop() 
{
    if (Serial.available())
    {
        char c = Serial.read();
        // variables used for JSON string parsing
        String ShellyResponse;
        JsonDocument shelly;
        float floatVal;
        switch (c)
        { 
        case '1': // ShellyPLus1PM #1 Switch status
            Serial.println(shelly1_1.name);
            ShellyResponse = shelly1_1.SwitchGetStatus(); //Switch.GetStatus?id=0
            Serial.println(ShellyResponse); 
            // extract and display some fields
            if (ShellyResponse.length() >= 80) // expect some minimum length to be valid
            {
                deserializeJson(shelly, ShellyResponse);
                floatVal = shelly["voltage"];
                Serial.print("  " + String(floatVal) + " V, ");
                floatVal = shelly["current"];
                Serial.print(String(floatVal) + " A, ");
                floatVal = shelly["apower"];
                Serial.print(String(floatVal) + " W, ");
                floatVal = shelly["temperature"]["tC"];
                Serial.println(String(floatVal) + " degC");
            }
            Serial.println("  active power = " + String(shelly1_1.ActivePower()) + " W");
            Serial.println("  temperature  = " + String(shelly1_1.TemperatureDegC()) + " degC");
            Serial.println("  rssi         = " + String(shelly1_1.WiFiRSSI()) + "dB");
            break;
        case '2': // ShellyPlus2PM "Wohnzimmer"
            Serial.println(blindControl.name);
            ShellyResponse = blindControl.CoverGetStatus();  // cover status
            Serial.println(ShellyResponse); 
            Serial.println("  active power = " + String(blindControl.ActivePower()) + " W");
            Serial.println("  temperature  = " + String(blindControl.TemperatureDegC()) + " degC");
            Serial.println("  rssi         = " + String(blindControl.WiFiRSSI()) + " dB");
            break;
        case 'd': // ShellyPlugS "diverse"
            Serial.println(plugS_1.name);
            ShellyResponse = plugS_1.shellyGetStatus();  // cover status
            Serial.println(ShellyResponse); 
            Serial.println("  active power = " + String(plugS_1.ActivePower()) + " W");
            Serial.println("  temperature  = " + String(plugS_1.TemperatureDegC()) + " degC");
            Serial.println("  rssi         = " + String(plugS_1.WiFiRSSI()) + " dB");
            break;
        case '3': // energy monitoring status from Shelly 3PM
            Serial.println(gridSupply.name);
            ShellyResponse = gridSupply.EMGetStatus(); // EM.GetStatus?id=0
            Serial.println(ShellyResponse); 
            // extract and display some fields
            if (ShellyResponse.length() >= 80) // expect some minimum length to be valid
            {
                deserializeJson(shelly, ShellyResponse);
                floatVal = shelly["total_current"];
                Serial.print(String(floatVal) + " A, ");
                floatVal = shelly["total_act_power"];
                Serial.print(String(floatVal) + " W, ");
                floatVal = shelly["total_aprt_power"];
                Serial.println(String(floatVal) + " VA");
            }
            Serial.println("  active power = " + String(gridSupply.TotalActivePower()) + " W");
            Serial.println("  temperature  = " + String(gridSupply.TemperatureDegC()) + " degC");
            Serial.println("  rssi         = " + String(gridSupply.WiFiRSSI()) + " dB");
            break;
        }
        Serial.println();
    }
}
