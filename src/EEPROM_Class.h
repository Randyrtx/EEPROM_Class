/**
 * @file EEPROM_Class.h
 * @author Randy E. Rainwater (randyrtx@outlook.com)
 * @brief EEPROM Class Header
 * @version 1.0.0
 * @date 2019-09-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <Particle.h>

/**
 * @brief EEPROM Class
 * 
 * Creates a generic storage block for saving data in EEPROM.
 * 
 * The storage block consists of two parts
 * 	- The data object itself.
 * 	- EEPROM image of the data object accessed via the Particle EEPROM.put() and EEPROM.get() functions.
 * 
 * The EEPROM image is copied to the data object on startup using the Class init() function. Thereafter,
 * any change to individual items in the data object result in the full object being transferred to EEPROM
 * via EEPROM.put(), and a new checksum is calculated and stored as well.
 * 
 * The class is intended to be used as the base class for deriving more data-specific classes.
 */

template <class OBJ>
class EEPROM_Class
{
public:
	/**
	 * @brief Construct a new eeprom class object
	 * 
	 */
	EEPROM_Class()
	{
		Log.trace("in EEPROM_Class Constructor.");
	}

	/**
	 * @brief Destroy the eeprom class object
	 * 
	 */
	~EEPROM_Class()
	{
		Log.trace("in EEPROM_Class Destructor.");
	}

	// EEPROM_Class(uint16_t address, OBJ &object)
	// {
	//     Log.trace("in EEPROM_Class Constructor.");
	// 	_adr_checksum = address;
	//     _adr_object = _adr_checksum + sizeof(_checksum);
	// 	_eepromSize = sizeof(object) + sizeof(_checksum);
	// 	Log.trace("_adr_checksum = %d, _adr_object = %d, _eepromSize = %d", _adr_checksum, _adr_object, _eepromSize);
	// }

	/**
	 * @brief Initialize the object and load data from EEPROM
	 * 
	 * @param address: EEPROM relative address for the saved object
	 * @param object: reference to the data object 
	 * @return true: EEPROM image loaded
	 * @return false: EEPROM image invalid
	 */
	bool begin(uint16_t address, OBJ &object)
	{
		_adr_checksum = address;
		_adr_object = _adr_checksum + sizeof(_checksum);
		_eepromSize = sizeof(object) + sizeof(_checksum);
		EEPROM.get(_adr_checksum, _checksum);
		Log.trace("_adr_checksum: %d, _adr_object: %d, _eepromSize: %d, checksum: 0x%04X", _adr_checksum, _adr_object, _eepromSize, _checksum);

		return readObject(object);
	}

	/**
	 * @brief Write object to EEPROM
	 * 
	 * @param object 
	 */
	void writeObject(OBJ &object)
	{
		EEPROM.put(_adr_object, object);
		_setChecksum();
	}

	/**
	 * @brief Read the object from EEPROM if checksum valid
	 * 
	 * @param object 
	 * @return true Object loaded
	 * @return false Checksum invalid, object not loaded
	 */
	bool readObject(OBJ &object)
	{
		if (_verifyChecksum())
		{
			EEPROM.get(_adr_object, object);
			Log.trace("EEPROM object image Loaded.");
			return true;
		}
		else
		{
			Log.error("EEPROM object image invalid.");
			return false;
		}
	}

	/**
	 * @brief Verify Checksum (Public)
	 * 
	 * @return true Checksum valid
	 * @return false Checksum invalid
	 */
	bool verifyChecksum()
	{
		return _verifyChecksum();
	}

protected:
	/******************************************************************************
 * Private members
 ******************************************************************************/

	/** @brief Address assigned to the data object in EEPROM.
	 */
	int _adr_object;

	/** @brief Address assigned to the checksum value in EEPROM.
	 * 
	 * Checksum is placed at the beginning of the memory block occupied by the data object.
	 */
	size_t _adr_checksum;

	/** @brief Total memory size of the data object (bytes)
	 */
	size_t _eepromSize;

	/** @brief 16-bit checksum of EEPROM object image
	 */
	uint16_t _checksum;

	/** 
	 * @brief Verifies checksum stored for the data block (Private)
	 * 
	 * @return true Checksum valid
	 * @return false Checksum error
	 */
	bool _verifyChecksum()
	{
		uint16_t temp;
		uint16_t checkSum;

		// Retrieve stored checksum value
		EEPROM.get(_adr_checksum, checkSum);

		temp = _calcChecksum();

		Log.trace("Stored Checksum: 0x%04X, Calculated: 0x%04X", checkSum, temp);

		if (checkSum == temp)
		{
			Log.info("EEPROM User Settings Checksum valid.");
			return true;
		}
		else
		{
			Log.error("EEPROM User Settings Checksum invalid.");
			return false;
		}
	}

	/******************************************************************************
 * Private functions
 ******************************************************************************/

private:
	/** 
	 * @brief Calculates and stores checksum in EEPROM
	 */
	void _setChecksum()
	{
		uint16_t temp = 0;

		for (size_t i = _adr_object; i < (_adr_checksum + _eepromSize); i++)
		{
			temp += EEPROM.read(i);
		}

		EEPROM.put(_adr_checksum, temp);
		_checksum = temp;

		Log.trace("EEPROM Checksum Updated: 0x%04X", temp);
	}

	/** 
	 * @brief calculate checksum of the data block
	 * 
	 * @return uint16_t calculated checksum
	 */
	uint16_t _calcChecksum()
	{
		uint16_t temp = 0;

		// Compute Checksum over the full EEPROM Space
		for (size_t i = _adr_object; i < (_adr_checksum + _eepromSize); i++)
		{
			temp += EEPROM.read(i);
		}
		return temp;
	}
};
