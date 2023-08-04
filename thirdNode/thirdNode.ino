#include <SPI.h>
#include <LoRa.h>

#include <DS3231.h>
#include <Wire.h>
    
const byte MQ4_Pin = A0;
const int R_O = 945;

DS3231 clock;
bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  Wire.begin();

  //while (!Serial);  

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  } 
}

void loop() {
  Serial.println(getMethanePPM2());
  Serial.println(getMethanePPM());

  int hrs = clock.getHour(h12Flag, pmFlag);
  int mts = clock.getMinute();
  int sec = clock.getSecond();
  String currentTime = String(hrs) + ":" + String(mts) + ":" + String(sec);
  Serial.println(currentTime);
  
  Serial.println(" ");
  LoRa.beginPacket();
  LoRa.print('*');
  LoRa.print((float)max(getMethanePPM(), getMethanePPM2()));
  LoRa.print(", ");
  LoRa.print(currentTime);
  LoRa.endPacket();
  delay(500);
  
}

float getMethanePPM(){
  float a0 = analogRead(A0); //get raw reading from sensor
  float v_o = a0 * 5 / 1023; //convert reading to volts
  float R_S = (5-v_o) * 1000 / v_o; //apply formula for getting RS
  float PPM = pow(R_S/R_O, -2.95) * 1000; //apply formula for getting PPM
  float PPM2 = KALMAN(PPM)*2;
  return PPM2; //return PPM value to calling function  
}

float getMethanePPM2(){
  float a0 = analogRead(A1); //get raw reading from sensor
  float v_o = a0 * 5 / 1023; //convert reading to volts
  float R_S = (5-v_o) * 1000 / v_o; //apply formula for getting RS
  float PPM = pow(R_S/R_O, -2.95) * 1000; //apply formula for getting PPM
  float PPM2 = KALMAN(PPM)*2;
  return PPM2; //return PPM value to calling function  
}

float KALMAN (float U){                                              
  static const float R = 0.33; //noise covariance
  static const float H = 1.00; //measurement map
  //at k = 0 we have these initial condidtions
  //i.e before simulations starts
  static float Q = 15; //initial covariance of estimated
  static float P = 0; //initial error covariance
  static float U_hat = 0; //initial estimated state
  static float K = 0; //initial kalman Gain
  //so U comes in, start the KF process
  K = P*H/(H*P*H+R); //Kalman gain
  U_hat = U_hat+ K*(U-H*U_hat);
  //Update error covariance and project it ahead
  P = (1-K*H)*P + Q;
  return U_hat; 
}

//source code can be found below
//https://www.utmel.com/components/how-to-use-mq4-gas-sensor?id=821
//I do not not claim ownership of this code #copyright 10-03-2023

