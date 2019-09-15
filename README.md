
# EEPROM_Class Library for Particle

This library implements classes for managing data objects stored in the emulated EEPROM of Particle devices.


## EEPROM_Class

```cpp
template <class OBJ>
class EEPROM_Class {}
```
Implements the basic, low-level functions for storing and retrieving generic data objects in EEPROM. Integrity is maintained via a 16-bit checksum which is verifed before data is retrieved, and updated whenever the object is rewritten. May be used standalone for small data objects or used in derived classes for specialized data objects.

Implemented as a template class so that objects of any type may be used.

## UserSettingsClass
```cpp
class UserSettingsClass : public EEPROM_Class<SettingsObject> {}
```
Derives from the EEPROM_Class base class to implement a specialized data object containing clock settings and Wifi information not already provided by the system OS.


[API Documentation](https://randyrtx.github.io/EEPROM_Class/)

## Simple Object use with EEPROM_Class
### User-defined data object
```cpp
    // Define structure of the object
    struct UserCredentials
    {
        char userName[16];
        char password[16];
    };

    // Declare the object
    UserCredentials myCredentials = {"", ""};

    // Instantiate the class object with the type
    EEPROM_Class<UserCredentials> myEEPROM;
    
    // Define the EEPROM address for the object, 
    //      where: 0 <= address <= EEPROM.length()  - (sizeof(object) + sizeof(uint16_t))
    #define mySettingsAddress 0
    
    // Initialize the data object with EEPROM image
    if(myEEPROM.begin(myCredentialsAddress, myCredentials))
        // Valid Image Loaded
    else
        // Checksum error, image not loaded
```
Refer to the [API Documentation](https://randyrtx.github.io/EEPROM_Class/) and [Examples](https://github.com/Randyrtx/EEPROM_Class/tree/master/examples) for further details.

## UserSettingsClass  User Configuration Object

### Data Object
```cpp
    struct SettingsObject
    {
        int8_t timeZone;
        float dstOffset;
        bool dstEnabled;
        char hostName[32];
        WLanSelectAntenna_TypeDef antennaType;
    };
```
### Class instantiation and startup
```cpp
	// User Settings Class Object
	UserSettingsClass mySettings;
    
    // Define the EEPROM address for the object,
    //      where: 0 <= address <= EEPROM.length() - (sizeof(object) + sizeof(uint16_t))
    #define mySettingsAddress 0
    
    if (mySettings.begin(mySettingsAddress))
    {
        // Valid Image Loaded
    }
    else
    {
        // Checksum error, image not loaded
    }
```
#### Typical use:
```cpp
    Time.zone(mySettings.getTimeZone());		  // Set time zone
    Time.setDSTOffset(mySettings.getDstOffset()); // Set DST offset
    if (mySettings.isDSTEnabled())
    {
        Time.beginDST(); // Set Daylight Savings Time flag
    }

    WiFi.setHostname(mySettings.getHostName());
    WiFi.selectAntenna(mySettings.getAntennaType());
```

Refer to the provided [examples](tbd) for more details.

## Examples:

[Basic Usage Example](https://github.com/Randyrtx/EEPROM_Class/tree/master/examples/basicUsage): Demonstrates use of the EEPROM_Class for a small user-supplied data object.



[Advanced Usage Example](https://github.com/Randyrtx/EEPROM_Class/tree/master/examples/advancedUsage): Demonstrates use of the UserSettingsClass for common system settings.

## LICENSE
Copyright 2019 Randy E. Rainwater

Licensed under the MIT License

