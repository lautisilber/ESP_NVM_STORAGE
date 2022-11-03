#if (!defined(_STORAGE_NVM_H_))
#define _STORAGE_NVM_H_

#include <Arduino.h>
#include <EEPROM.h>

#define STORAGE_SIZE 512


class StorageClass
{
private:
    uint8_t _data[STORAGE_SIZE]{0};

public:
    StorageClass();
    ~StorageClass();

    template<typename T>
    T &get(T &v) const;

    template<typename T>
    bool put(const T &v, bool commit=true);

    bool clear();

    void PrintAll();

private:
    inline void _load();
    inline bool _commit();
};





StorageClass::StorageClass()
{
    EEPROM.begin(STORAGE_SIZE);
    _load();
}

StorageClass::~StorageClass()
{
    EEPROM.end();
}


inline void StorageClass::_load()
{
    for (size_t i = 0; i < STORAGE_SIZE; i++)
    {
        _data[i] = EEPROM.read(i);
    }
}

inline bool StorageClass::_commit() {
    for (size_t i = 0; i < STORAGE_SIZE; i++) {
        EEPROM.write(i, _data[i]);
    }
    return EEPROM.commit();
}


template<typename T>
T &StorageClass::get(T &v) const {
    if (sizeof(T) < STORAGE_SIZE) {
        memcpy((uint8_t *) &v, _data, sizeof(T));
    }
    return v;
}


template<typename T>
bool StorageClass::put(const T &v, bool commit) {
    if (sizeof(T) < STORAGE_SIZE) {
        memcpy(_data, (const uint8_t *) &v, sizeof(T));
        if (commit)
            return _commit();
        return true;
    }
    return false;
}


bool StorageClass::clear() {
    for (size_t i = 0; i < STORAGE_SIZE; i++)
    {
        EEPROM.write(i, 0);
    }
    bool res = EEPROM.commit();
    if (res)
        memset(_data, 0, STORAGE_SIZE);
    return res;
}


void StorageClass::PrintAll() {
    _load();
    const uint8_t div = 8;
    for (size_t i = 0; i < STORAGE_SIZE / div; i++) {
        for (size_t j = 0; j < div; j++) {
            Serial.print(_data[i * div + j]);
        }
        Serial.println();
    }
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_STORAGE)
//extern StorageClass StorageNVM;
StorageClass StorageNVM;
#endif

#endif
