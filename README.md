
# EEPROM_Class Library for Particle                                        {#mainpage}

This library implement classes for managing data objects stored in the emulated EEPROM of Particle devices.


## EEPROM_Class

Implements the basic, low-level functions for storing and retrieving generic data objects in EEPROM. Integrity
is maintained via a 16-bit checksum which is verifed before data is retrieved, and updated whenever the object is rewritten.
May be used standalone for small data objects or used in derived classes for specialized data objects.

## UserSettingsClass
Derives from the EEPROM_Class base class to implement a specialized data object containing common system settings,
such as user/password, clock settings, etc.

Full documentation can be found here: {tbd}

## Simple Object use with EEPROM_Class
### User-defined data object
```
    struct SettingsObject
    {
        char userName[16];
        char password[16];
        char hostName[32];
    };

    SettingsObject mySettings = {"", "", ""};
```
### Class instantiation and startup
```
    EEPROM_Class<SettingsObject> myEEPROM;
```
### Initialize the class object
```
    // Initialize the data object
    myEEPROM.begin(0, mySettings);
```
### Read object from EEPROM into the RAM object
```
    // Fetch object from EEPROM
    myEEPROM.readObject(mySettings);
```

### Class-provided user configuration data object
```
struct SettingsObject
{
    char userName[16];
    char password[16];
    int8_t timeZone;
    float dstOffset;
    bool dstEnabled;
    char hostName[32];
    WLanSelectAntenna_TypeDef antennaType;
};
```
### Class instantiation and startup
```
    // User Configuration Settings Object
    UserSettingsClass mySettings;

    mySettings.begin(myAddress)
```

Refer to the provided examples for more details.

## Examples:

### Basic Usage Example

Demostrates use of the EEPROM_Class for a small user-supplied data object.

### Advanced Usage Example

Demonstrates use of the UserSettingsClass for common system settings.


## LICENSE
Copyright 2019 Randy E. Rainwater

Licensed under the MIT License

