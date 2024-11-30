#ifndef _SHELLYDEVICE_H_
#define _SHELLYDEVICE_H_
#include <Arduino.h>

// set of classes to access shelly Gen2+ devices via HTTP
// NOTE: Just some functions from the shelly API are implemented
// https://shelly-api-docs.shelly.cloud/gen2/

// ============================================================================
// SHELLY COMPONENTS 
// these classes define functionality which could not be used directly and
// will becombined to device classes later
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Introduction 

// communication and basic functions of all shelly Gen2+ devices
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Shelly
// Any shelly device is combining functionality of shellyDevice with 
// additional endpoints like input, switch, cover, energy monitoring
class shellyDevice
{
protected:
    shellyDevice() {}; // do not allow direct use
public:
    // set server IP and password in case this device is requireing authentication
    shellyDevice(String serverIP, String password="") :  
        _server("http://" + serverIP), 
        _user("admin"), 
        _password(password),
        name(serverIP) {};
    String name; // could be used by user to identify device. Set to serverIP as default by constructor
    String server() { return _server; }; // return full server IP string
    String extractParam(String s, String param, char delimiter); // extract part between param and delimiter
    // do HTTP GET command access respective method in /rpc tree of web interface
    String GET(String rpcMethod);   // function adds "/rpc/" to method passed
    // common functions for all Gen2+ devices
    String shellyGetStatus()       
        { return GET("shelly.GetStatus"); };
    String shellyGetConfig()
        { return GET("shelly.GetConfig"); };
    String shellyListMethods()
        { return GET("shelly.ListMethods"); };
    String shellyGetDeviceInfo()
        { return GET("shelly.GetDeviceInfo"); };
    String shellyCheckForUpdate()
        { return GET("shelly.CheckForUpdate"); };
    String shellyGetComponents()
        { return GET("shelly.GetComponents"); };
private:
    String _server;
    String _user;
    String _password;
    String SHA256hash(String s);    // calculate SHA256 hash for string s
};

// shelly wifi, should be available in all components using this library
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/WiFi 
class shellyWiFi : virtual public shellyDevice
{
protected:
    shellyWiFi() {}; // do not allow direct use of this class
public:
    String WiFiGetConfig()
        { return GET("WiFi.GetConfig"); };
    String WiFiGetStatus()
        { return GET("WiFi.GetStatus"); };
    int WiFiRSSI();
};

// shelly inputs, e.g. available in ShellyPlus1PM, ShellyPlus2PM
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Input
class shellyInput : virtual public shellyDevice
{
protected:
    shellyInput() {}; // do not allow direct use of this class
public:
    String InputGetConfig(uint8_t id=0)
        { return GET("Input.GetConfig?id=" + String(id)); };
    String InputGetStatus(uint8_t id=0)
        { return GET("Input.GetStatus?id=" + String(id)); };
};

// shelly cover, e.g. available in ShellyPlus2PM
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Cover 
class shellyCover : virtual public shellyDevice
{
protected:
    shellyCover() {}; // do not allow direct use of this class
public:
    String CoverGetConfig(uint8_t id=0)
        { return GET("Cover.GetConfig?id=" + String(id)); };
    String CoverGetStatus(uint8_t id=0)
        { return GET("Cover.GetStatus?id=" + String(id)); };
    String CoverOpen(uint8_t id=0)
        { return GET("Cover.Open?id=" + String(id)); };
    String CoverClose(uint8_t id=0)
        { return GET("Cover.Close?id=" + String(id)); };
    String CoverStop(uint8_t id=0)
        { return GET("Cover.Stop?id=" + String(id)); };
    String CoverGoToPosition(uint8_t pos=100, uint8_t id=0)
        { return GET("Cover.GoToPosition?id=" + String(id) + "&pos=" + String(pos)); };
    float TemperatureDegC(uint8_t id=0);
    float ActivePower(uint8_t id=0);
};

// shelly switch, e.g. available in ShellyPlus1PM, SHellyPlus2PM
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Switch
class shellySwitch : virtual public shellyDevice
{
protected:
    shellySwitch() {}; // do not allow direct use of this class
public:
    String SwitchSet(bool on, uint8_t id=0)
        { return GET("Switch.Set?id=" + String(id) + "&on=" + on ? "true":"false"); };
    String SwitchToggle(uint8_t id=0)
        { return GET("Switch.Toggle?id=" + String(id)); };
    String SwitchGetConfig(uint8_t id=0)
        { return GET("Switch.GetConfig?id=" + String(id)); };
    String SwitchGetStatus(uint8_t id=0)
        { return GET("Switch.GetStatus?id=" + String(id)); };
    float TemperatureDegC(uint8_t id=0);
    float ActivePower(uint8_t id=0);
};

// temperature sensors, available e.g. in ShellyPro3EM
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Temperature
class shellyTemperature : virtual public shellyDevice
{
protected:
    shellyTemperature() {}; // do not allow direct use of this class
public:
    String TemperatureGetConfig(uint8_t id=0)
        { return GET("Temperature.GetConfig?id=" + String(id)); };
    String TemperatureGetStatus(uint8_t id=0)
        { return GET("Temperature.GetStatus?id=" + String(id)); };
    float TemperatureDegC(uint8_t id=0);
};

// multiple phase totalized energy monitoring, e.g. available in ShellyPro3EM in three phase mode
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/EM
class shellyEM : virtual public shellyDevice
{
protected:
    shellyEM() {}; // do not allow direct use of this class
public:
    String EMGetConfig(uint8_t id=0)
        { return GET("EM.GetConfig?id=" + String(id)); };
    String EMGetStatus(uint8_t id=0) // e.g. ShellyPro3EM
        { return GET("EM.GetStatus?id=" + String(id)); };
    float TotalActivePower(uint8_t id=0);
};

// single phase energy monitoring, e.g. available 3 times in ShellyPro3EM if in monophase mode
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/EM1
class shellyEM1 : virtual public shellyDevice
{
protected:
    shellyEM1() {}; // do not allow direct use of this class
public:
    String EM1GetConfig(uint8_t id=0)
        { return GET("EM1.GetConfig?id=" + String(id)); };
    String EM1GetStatus(uint8_t id=0)
        { return GET("EM1.GetStatus?id=" + String(id)); };
};

// ============================================================================
// SHELLY DEVICES
// actual devices assemble basic functions with extensions

// basic functionality of all Gen2+ devices
// could be used instead of the specific device classes below
// to access functions of shellyDevice if you handle all your requests by yourself 
// https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Shelly 
class ShellyGen2 : public shellyWiFi
{
public:
    ShellyGen2(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

// ShellyPlus1PM (one switch with button input)
// https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlus1PM 
// NOTE: ShellyPlus1PM returns energy monitoring data in Switch.GetStatus
class ShellyPlus1PM : public shellyWiFi, public shellyInput, public shellySwitch
{
public:
    ShellyPlus1PM(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

// ShellyPlus2PM in cover profile (two interlocked outputs, two inputs for open/close)
// https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlus2PM 
class ShellyPlus2PMcover : public shellyWiFi, public shellyCover, public shellyInput
{
public:
    ShellyPlus2PMcover(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

// ShellyPlus2PM in switch profile (two independend switches, two inputs to control)
// https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlus2PM
class ShellyPlus2PMswitch : public shellyWiFi, public shellySwitch, public shellyInput
{
public:
    ShellyPlus2PMswitch(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

// ShellyPlugPlusS (wifi plug adapter)
// https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlusPlugS
class ShellyPlugPlusS : public shellyWiFi, public shellySwitch
{
public:
    ShellyPlugPlusS(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

// ShellyPro3EM (three phase energy monitoring)
// https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPro3EM 
class ShellyPro3EM3phase : public shellyWiFi, public shellyEM, public shellyTemperature
{
public:
    ShellyPro3EM3phase(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

// ShellyPro3EM (used as three independent phase monitoring systems)
// https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPro3EM 
class ShellyPro3EMmono : public shellyWiFi, public shellyEM, public shellyEM1, public shellyTemperature
{
public:
    ShellyPro3EMmono(String serverIP, String password="") :
        shellyDevice(serverIP, password) {};
};

#endif