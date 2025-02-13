#include "HUSKYLENS.h"
#include <Wire.h>

HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
void printResult(HUSKYLENSResult result);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!huskylens.begin(Wire))
  {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
}

void loop() {
  if (!huskylens.request()){
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  }
  else if (!huskylens.isLearned()){
    Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  }
  else if (!huskylens.available()){
    Serial.println(F("TIDAK ADA OBJEK TERDETEKSI"));
  }
  else
  {
    while (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      printnilaiID(result);
    }
  }
  delay(500);
}

void printnilaiID(HUSKYLENSResult result) {
  String bentuk;
  if (result.ID == 1) {
    bentuk = "LINGKARAN";
  }
  else if (result.ID == 2) {
    bentuk = "PERSEGI";
  }
  else {
    bentuk = "TIDAK DIKETAHUI";
  }
  Serial.print("LEARNING ID:");
  Serial.println(result.ID);
  Serial.print("BENTUK:");
  Serial.println(bentuk);
  delay(500);
}
