# Advanced Usage Example

Demostrates use of the EEPROM_Class for a small user-defined data object.

### User-defined data object (example)
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
### Updating the object
```cpp
    strcpy(myCredentials.userName, "NewUserName");
    strcpy(myCredentials.password, "NewPassword");
    myEEPROM.writeObject(myCredentials);
```
Refer to the [API Documentation](https://randyrtx.github.io/EEPROM_Class/) for further details.

## LICENSE
Copyright 2019 Randy E. Rainwater

Licensed under the MIT License

