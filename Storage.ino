#include <EEPROM.h>
#include "StorageNVM.hpp"

typedef struct A {
    int a;
    float b;
    char c[8];
};

void write() {
    A a = {8, 13.96};
    strlcpy(a.c, "holo", 8);
    
    StorageNVM.put(a);
}

void read() {
    A a;
    
    StorageNVM.get(a);
    
    Serial.println(a.a);
    Serial.println(a.b);
    Serial.println(a.c);
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n");
    delay(1000);

    write();
    Serial.println("write done");

    read();
    Serial.println("read done");
}

void loop() {
    delay(1000);
}
