# Advanced Usage Example

Demostrates use of the EEPROM_Class for a small user-defined data object.


### Custom data object
Define an object for user credentials:

```cpp
struct UserCredentials
{
    char userName[16];
    char password[16];
};

UserCredentials myCredentials = {"", "", ""};

```


### Class instantiation

Instantiate the object, specifying the object type
```cpp
EEPROM_Class<UserCredentials> myEEPROM;

```
```cpp
    // Initialize the data object
    myEEPROM.begin(0, myCredentials);

```
### Object Retrieval
Retrieve the data object from EEPROM

```cpp
    // Fetch object from EEPROM
    myEEPROM.readObject(myCredentials);
```

### 
