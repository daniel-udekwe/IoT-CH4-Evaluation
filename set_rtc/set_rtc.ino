#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  rtc.adjust(DateTime(__DATE__, __TIME__));  // Set RTC to compile date and time
  Serial.println("RTC set to compile date and time.");
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("Current date: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.println();

  delay(1000);
}
