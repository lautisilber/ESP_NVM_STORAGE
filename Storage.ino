#include "Storage.h"


void write() {
  int i = -123;
  float f = 1234.5678;
  String s = "Hola que tal";

  Storage.setInt(i, 0);
  Storage.setFloat(f, 1);
  Storage.setString(s.c_str(), strlen(s.c_str()), 2);
}

void read() {
  int i = Storage.getInt(0);
  float f = Storage.getFloat(1);
  const size_t len = 64;
  char c[len]{0};
  Storage.getString(c, len, 2);

  Serial.println(i);
  Serial.println(f);
  Serial.println(c);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  delay(1000);

  //Storage.PrintAll();
  read();
  //Storage.PrintAll();
}

void loop() {
  delay(1000);
}
