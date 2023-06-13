#include <SPI.h>
#include <LoRa.h>
#include <SD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define SS_PIN 4
#define RST_PIN 9
#define DI0_PIN 2

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


File myFile;

void setup() {
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("CH4 Emission Proj.");
  
  Serial.begin(9600);
  //while (!Serial);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa initialized.");
  if (!SD.begin(SS_PIN)) {
    Serial.println("Starting SD card failed!");
    while (1);
  }
  Serial.println("SD card initialized.");
}





void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: ");

    while (LoRa.available()) {
      String data = LoRa.readString();
      Serial.print(data);
      Serial.println("");
      //Serial.println(data[4]);
      Serial.println(millis());




      
      myFile = SD.open("data.txt", FILE_WRITE);
      if (myFile) {
        myFile.print(data);
        
        lcd.setCursor(1,1);
        lcd.print("Node 1: ");
        lcd.setCursor(8,1);
        if (data[0] != '+')
        {
        lcd.print(" +");
        lcd.print(data);
        //lcd.setCursor(14,1);
        //lcd.print("PPM");
        //lcd.print(" +Sync");
        lcd.setCursor(16,1);
        lcd.print(LoRa.packetRssi());
        }


        
        lcd.setCursor(1,2);
        lcd.print("Node 2: ");
        lcd.setCursor(8,2);
        if (data[0] == '+')
        { 
        lcd.print(" ");
        lcd.print(data);
        //lcd.setCursor(14,2);
        //lcd.print("PPM");
        //lcd.print(" +Sync");
        lcd.setCursor(16,2);
        lcd.print(LoRa.packetRssi());
        }

        
        lcd.setCursor(1,3);
        lcd.print("Node 3: ");

        myFile.print(", ");
        myFile.print(LoRa.packetRssi());
        myFile.print(", ");
        myFile.print(millis());
        myFile.println(" milli");

       /* myFile.print(", ");
        myFile.print(data);
        myFile.print(", ");
        myFile.println(LoRa.packetRssi());*/
        
        myFile.close();
        Serial.println("Data saved to SD card.");
      } else {
        Serial.println("Error opening data.txt");
      }
    }

    Serial.println();
  }
}

