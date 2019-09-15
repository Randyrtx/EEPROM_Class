# Basic Usage Example

Demostrates use of the EEPROM_Class for a small user-supplied data object.

### Object Definition

```
struct SettingsObject
{
    char userName[16];
    char password[16];
    char hostName[32];
};

SettingsObject mySettings = {"", "", ""};

```


### Class instantiation
```
EEPROM_Class<SettingsObject> myEEPROM;

```
```
    // Initialize the data object
    myEEPROM.begin(0, mySettings);

```
### Read object from EEPROM into the RAM object
```
    // Fetch object from EEPROM
    myEEPROM.readObject(mySettings);
```

```

```
```

```
```

```
```

```
```

```
```

```
```

```
```

```
