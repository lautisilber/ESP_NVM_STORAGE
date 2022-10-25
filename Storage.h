#if (!defined(_STORAGE_H_))
#define _STORAGE_H_

#include <Arduino.h>
#include <EEPROM.h>

#define STORAGE_EEPROM_SIZE 512 * sizeof(uint8_t)
#define STORAGE_ENTRY_SIZE 32 * sizeof(uint8_t)
#define STORAGE_ENTRIES ((size_t)(STORAGE_EEPROM_SIZE) / (STORAGE_ENTRY_SIZE))


class StorageClass
{
private:
    uint8_t _eeprom[STORAGE_ENTRIES][STORAGE_ENTRY_SIZE];

public:
    StorageClass();
    ~StorageClass();

    bool setInt(int i, uint8_t index);
    bool setFloat(float f, uint8_t index);
    bool setString(const char *str, size_t len, uint8_t index);

    int getInt(uint8_t index, bool forceLoad=false);
    float getFloat(uint8_t index, bool forceLoad=false);
    void getString(char *str, size_t len, uint8_t index, bool forceLoad=false);

    void clear();

    void PrintAll();

private:
    void _load();
    bool _clear();
    bool _read(size_t index);
    bool _write(size_t index) const;
    inline void _cleanIndex(size_t index);
    inline bool _checkIndex(size_t i) const;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_STORAGE)
extern StorageClass Storage;
#endif

#endif
