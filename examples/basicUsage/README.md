# Basic Usage Example

Demonstrates use of the UserSettingsClass for common system settings.


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
    Time.zone(mySettings.getTimeZone()); // Set time zone
    Time.setDSTOffset(mySettings.getDstOffset()); // Set DST offset
    if (mySettings.isDSTEnabled())
    {
        Time.beginDST(); // Set Daylight Savings Time flag
    }
    else
    {
        Time.endDST(); // Note: this is not needed on startup, as the DST setting defaults to off.
    }

    WiFi.setHostname(mySettings.getHostName());
    WiFi.selectAntenna(mySettings.getAntennaType());
```

Refer to the [API Documentation](https://randyrtx.github.io/EEPROM_Class/) for further details.

## LICENSE
Copyright 2019 Randy E. Rainwater

Licensed under the MIT License

