[comment]: <> {#mainpage}
## EEPROM_Class Library for Particle 

This library implement classes for managing data objects stored in the emulated EEPROM of Particle devices.


# EEPROM_Class

Implements the basic, low-level functions for storing and retrieving generic data objects in EEPROM. Integrity
is maintained via a 16-bit checksum which is verifed before data is retrieved, and updated whenever the object is rewritten.
May be used standalone for small data objects or used in derived classes for specialized data objects.

#UserSettingsClass
Derives from the EEPROM_Class base class to implement a specialized data object containing common system settings,
such as user/password, clock settings, etc.

Full documentation can be found here: {tbd}

## Examples:

[Advanced Usage Example](examples/advancedUsage)

Demonstrates use of the UserSettingsClass for common system settings.

[Basic Usage Example](examples/basicUsage)

Demostrates use of the EEPROM_Class for a small user-supplied data object.


## LICENSE
Copyright 2019 Randy E. Rainwater

Licensed under the [MIT License](LICENSE)

