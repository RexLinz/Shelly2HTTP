# Shelly

## Library to access Shelly Gen2+ devices from ESP32 via HTTP rpc calls supporting digest authorization

Set of classes to access shelly Gen2+ devices via HTTP

**NOTE:** For each class implemented the reference to the respective section
of the shelly API documentation is given below.

**NOTE:** Just some functions from the Shelly API are implemented at the moment,
actually just functions to read from shelly devices at all.
Feel free to add other functionality according to the functions already done.

[Full Shelly Gen2+ API documentation](https://shelly-api-docs.shelly.cloud/gen2/)

## library concept

Shelly DEVICES are based on COMPONENTS, each one defining a certain funcitonality.
Actual devices combine several of this components.

This stucture is followed in the implementation starting with class ShellyDevice
dealing with the basic communication only.

It's not intended to use class shellyDevice or one of the components directly in an application.
Use one of the SHELLY DEVICES below or combine COMPONENTS to create a new device similar to the existing ones.

### BASIC COMMUNICATION

#### class shellyDevice

Communication with and basic functions of all shelly Gen2+ devices

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Shelly>

Any shelly device is combining functionality of shellyDevice with additional endpoints like input, switch, cover, energy monitoring

### SHELLY COMPONENTS

These classes define functionality like WiFi, switch, input, meter, ...  
which could not be used directly and will becombined to DEVICE classes later

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Introduction>

#### class shellyWiFi

WiFi should be available in all components using this library

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/WiFi>

#### class shellyInput

Shelly inputs, e.g. available in ShellyPlus1PM, ShellyPlus2PM

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Input>

#### class shellyCover

Shelly cover control, e.g. available in ShellyPlus2PM

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Cover>

#### class shellySwitch

Shelly switch (output), e.g. available in ShellyPlus1PM, SHellyPlus2PM

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Switch>

#### class shellyTemperature

Temperature sensors, available e.g. in ShellyPro3EM

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Temperature>

#### class shellyEM

Multiple phase totalized energy monitoring, e.g. available in ShellyPro3EM in three phase mode

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/EM>

#### class shelly1EM

single phase energy monitoring, e.g. available 3 times in ShellyPro3EM if in monophase mode

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/EM1>

### SHELLY DEVICES

Actual devices assemble basic functions with extensions

#### class ShellyGen2

Basic functionality of all Gen2+ devices
Could be used instead of the specific device classes below to access functions of shellyDevice if you handle all your requests by yourself

<https://shelly-api-docs.shelly.cloud/gen2/ComponentsAndServices/Shelly>

#### class ShellyPlus1PM

ShellyPlus1PM (one switch with button input)

<https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlus1PM>

**NOTE:** ShellyPlus1PM returns energy monitoring data in Switch.GetStatus

#### class ShellyPlus2PMcover

ShellyPlus2PM in cover profile (two interlocked outputs, two inputs for open/close)

<https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlus2PM>

#### class ShellyPlus2PMswitch

ShellyPlus2PM in switch profile (two independend switches, two inputs to control)

<https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlus2PM>

#### class ShellyPlugPlusS

ShellyPlugPlusS (wifi plug adapter)

<https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPlusPlugS>

#### class ShellyPro3EM3phase

ShellyPro3EM (three phase energy monitoring)

<https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPro3EM>

#### class ShellyPro3EMmono

ShellyPro3EM (used as three independent phase monitoring systems)

<https://shelly-api-docs.shelly.cloud/gen2/Devices/Gen2/ShellyPro3EM>
