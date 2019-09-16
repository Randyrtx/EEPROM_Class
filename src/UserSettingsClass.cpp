/**
 * @file UserSettingsClass.cpp
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief  User Configurations Settings Class Member Functions
 * @version 1.0.0
 * @date 2019-09-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
 
#include <Particle.h>
#include "UserSettingsClass.h"

bool UserSettingsClass::begin(uint16_t address)
{
    bool flag;
    _adr_checksum = address;
    _adr_object = _adr_checksum + sizeof(_checksum);
    _eepromSize = sizeof(_mySettings) + sizeof(_checksum);
    EEPROM.get(_adr_checksum, _checksum);
    Log.trace("_adr_checksum: %d, _adr_object: %d, _eepromSize: %d, checksum: 0x%04X", _adr_checksum, _adr_object, _eepromSize, _checksum);

    flag = readObject(_mySettings);
    if (!flag)
    {
        Log.error("UserSettingsClass data invaild, reinitializing...");
        reinitialize();
    }

    return flag;
}


/**
 * @brief Reinitialize Data Objects and EEPROM with defaults
 * 
 */
void UserSettingsClass::reinitialize()
{
    Log.trace("Initializing EEPROM User Settings to defaults.");

    // Set local data to defaults

    _mySettings.timeZone = DEFAULT_USER_TZ;
    _mySettings.dstOffset = DEFAULT_USER_DSTOFFSET;
    _mySettings.dstEnabled = DEFAULT_USER_DSTENABLE;
    strcpy(_mySettings.hostName, DEFAULT_USER_HOSTNAME);
    _mySettings.antennaType = DEFAULT_USER_ANTENNA;

    // Reload EEPROM

    writeObject(_mySettings);

    Log.trace("Time Settings -- Timezone: %d, DST Offset: %f, DST Enabled: %s", _mySettings.timeZone, _mySettings.dstOffset, (_mySettings.dstEnabled) ? "Yes" : "No");
    Log.trace("Hostname: %s", _mySettings.hostName);
    switch (_mySettings.antennaType)
    {
    case ANT_INTERNAL:
        Log.trace("Antenna Type: Internal");
        break;

    case ANT_EXTERNAL:
        Log.trace("Antenna Type: External");
        break;

    case ANT_AUTO:
        Log.trace("Antenna Type: Auto");
        break;

    default:
        break;
    }
}

/**
 * @brief Print contents of the User Settings object to the default log handler
 * 
 */
void UserSettingsClass::logUserData()
{
    Log.info("Stored User Data from EEPROM:");
    Log.info("Timezone: %d", _mySettings.timeZone);
    Log.info("DST Offset: %4.1f", _mySettings.dstOffset);
    Log.info("DST Enabled: %s", (_mySettings.dstEnabled) ? "Yes" : "No");
    Log.info("Hostname: %s", _mySettings.hostName);

    switch (_mySettings.antennaType)
    {
    case ANT_INTERNAL:
        Log.info("Antenna Type: Internal");
        break;

    case ANT_EXTERNAL:
        Log.info("Antenna Type: External");
        break;

    case ANT_AUTO:
        Log.info("Antenna Type: Auto");
        break;

    default:
        break;
    }

    Log.info("Checksum: 0x%04X\n", _checksum);
}

bool UserSettingsClass::setHostName(String hostName)
{
    strcpy(_mySettings.hostName, hostName);
    // Range check:
    if (hostName.length() > (sizeof(_mySettings.hostName) - 1))
    {
        strncpy(_mySettings.hostName, hostName, sizeof(_mySettings.hostName) - 1);
        _mySettings.hostName[sizeof(_mySettings.hostName) - 1] = '\0';
        writeObject(_mySettings);
        Log.warn("Hostname too long, truncated.");
        return false;
    }
    else
    {
        strcpy(_mySettings.hostName, hostName);
        writeObject(_mySettings);
        return true;
    }
}
