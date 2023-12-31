/*

Prints time stamps for 5 seconds using getXXX functions

Based on DS3231_set.pde
by Eric Ayars
4/11

Added printing back of time stamps and increased baud rate
(to better synchronize computer and RTC)
Andy Wickert
5/15/2011

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 clock;

bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
  // Start the serial port
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();


  

  
}

void loop() {
       delay(1000);
      Serial.print(clock.getYear(), DEC);
      Serial.print("-");
      Serial.print(clock.getMonth(century), DEC);
      Serial.print("-");
      Serial.print(clock.getDate(), DEC);
      Serial.print(" ");
      /*Serial.print(clock.getHour(h12Flag, pmFlag), DEC); //24-hr
      Serial.print(":");
      Serial.print(clock.getMinute(), DEC);
      Serial.print(":");
      Serial.println(clock.getSecond(), DEC); */
      int hrs = clock.getHour(h12Flag, pmFlag);
      int mts = clock.getMinute();
      int sec = clock.getSecond();
      String currentTime = String(hrs) + ":" + String(mts) + ":" + String(sec);
      Serial.println(currentTime);
}
