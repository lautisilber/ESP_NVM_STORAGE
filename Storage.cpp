#if (!defined(_STORAGE_CPP_))
#define _STORAGE_CPP_

#include "Storage.h"

#include <Arduino.h>
#include <EEPROM.h>

#include "Storage.h"


#define ST_MAX(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define ST_MIN(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define ST_ABS(N) ((N < 0) ? (-N) : (N))


inline bool StorageClass::_checkIndex(size_t i) const {
    return i < STORAGE_ENTRIES;
}

bool StorageClass::setInt(int i, uint8_t index)
{
    if (!_checkIndex(index))
        return false;
    _cleanIndex(index);
    // memcpy(_eeprom[index], &i, ST_MIN(sizeof(i), STORAGE_ENTRY_SIZE));
    snprintf((char *)_eeprom[index], STORAGE_ENTRY_SIZE, "%d", i);
    return _write(index);
}

bool StorageClass::setFloat(float f, uint8_t index)
{
    if (!_checkIndex(index))
        return false;
    _cleanIndex(index);
    // memcpy(_eeprom[index], &f, ST_MIN(sizeof(f), STORAGE_ENTRY_SIZE));
    snprintf((char *)_eeprom[index], STORAGE_ENTRY_SIZE, "%4.4f", f);
    return _write(index);
}

bool StorageClass::setString(const char *str, size_t len, uint8_t index)
{
    if (!_checkIndex(index))
        return false;
    _cleanIndex(index);
    strncpy((char *)_eeprom[index], str, ST_MIN(len, STORAGE_ENTRY_SIZE));
    return _write(index);
}

int StorageClass::getInt(uint8_t index, bool forceLoad)
{
    if (!_checkIndex(index))
        return 0;
    if (forceLoad)
        _read(index);
    char temp[STORAGE_ENTRY_SIZE + 1]{0};
    strlcpy(temp, (const char *)_eeprom[index], STORAGE_ENTRY_SIZE);
    return atoi(temp);
}

float StorageClass::getFloat(uint8_t index, bool forceLoad)
{
    if (!_checkIndex(index))
        return 0.0;
    if (forceLoad)
        _read(index);
    char temp[STORAGE_ENTRY_SIZE + 1]{0};
    strlcpy(temp, (const char *)_eeprom[index], STORAGE_ENTRY_SIZE);
    return (float)atof(temp);
}

void StorageClass::getString(char *str, size_t len, uint8_t index, bool forceLoad) {
    if (!_checkIndex(index)) return;
    if (forceLoad)
        _read(index);
    strlcpy(str, (const char *)_eeprom[index], ST_MIN(len, STORAGE_ENTRY_SIZE));
}


void StorageClass::clear() {
    _clear();
}



StorageClass::StorageClass()
{
    EEPROM.begin(STORAGE_EEPROM_SIZE);
    _load();
}

StorageClass::~StorageClass()
{
    EEPROM.end();
}

void StorageClass::_load()
{
    for (size_t i = 0; i < STORAGE_ENTRIES; i++)
    {
        for (size_t j = 0; j < STORAGE_ENTRY_SIZE; j++)
        {
            _eeprom[i][j] = EEPROM.read(i * STORAGE_ENTRY_SIZE + j);
        }
    }
}

bool StorageClass::_write(size_t index) const
{
    if (!_checkIndex(index))
        return false;
    for (size_t i = 0; i < STORAGE_ENTRY_SIZE; i++)
    {
        EEPROM.write(index * STORAGE_ENTRY_SIZE + i, _eeprom[index][i]);
    }
    return EEPROM.commit();
}

bool StorageClass::_read(size_t index)
{
    if (!_checkIndex(index))
        return false;
    for (size_t i = 0; i < STORAGE_ENTRY_SIZE; i++)
    {
        _eeprom[index][i] = EEPROM.read(index * STORAGE_ENTRY_SIZE + i);
    }
    return true;
}

bool StorageClass::_clear()
{
    for (size_t i = 0; i < STORAGE_EEPROM_SIZE; i++)
    {
        EEPROM.write(i, 0);
    }
    memset(_eeprom, 0, STORAGE_EEPROM_SIZE);
    return EEPROM.commit();
}

inline void StorageClass::_cleanIndex(size_t index)
{
    if (!_checkIndex(index))
        return;
    memset(_eeprom[index], 0, STORAGE_ENTRY_SIZE);
}

void StorageClass::PrintAll() {
    _load();
    for (uint8_t i = 0; i < STORAGE_ENTRIES; i++) {
        char buf[STORAGE_ENTRY_SIZE+1]{0};
        getString(buf, STORAGE_ENTRY_SIZE+1, i);
        Serial.print(i);
        Serial.print(": '");
        Serial.print(buf);
        Serial.println("'");
    }
}


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_STORAGE)
StorageClass Storage;
#endif

#endif
