#include <EEPROM.h>
#include "StorageNVM.hpp"

typedef struct A {
    int a;
    float b;
    char c[8];
};

typedef struct B {
    char a[32];
    double b;
};

void write() {
    A a = {8, 13.96};
    strlcpy(a.c, "holo", 8);

    B b;
    strlcpy(b.a, "chausitisimo", 32);
    b.b = 1654.324;
    
    StorageNVM.put(0, a);
    StorageNVM.put(1, b);
}

void read() {
    A a;
    B b;
    
    StorageNVM.get(0, a);
    StorageNVM.get(1, b);
    
    Serial.println(a.a);
    Serial.println(a.b);
    Serial.println(a.c);
    Serial.println();
    Serial.println(b.a);
    Serial.println(b.b);
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n");
    delay(1000);

    StorageNVM.addToScheme<A>();
    StorageNVM.addToScheme<B>();

    write();
    Serial.println("write done");

    read();
    Serial.println("read done");
}

void loop() {
    delay(1000);
}
