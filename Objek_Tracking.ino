//Deklarasi Komponen
#include <HUSKYLENS.h>
#include <DFRobot_Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Variabel Servo X dan Y
volatile float mind_n_currentx, mind_n_currenty;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Deklarasi Huskylens
HUSKYLENS huskylens;
Servo servo_9;
Servo servo_10;
//Deklarasi Fungsi Huskylens
void printResult(HUSKYLENSResult result);


void setup() {
  //Perintah LCD
  lcd.init();
  lcd.backlight();
  Wire.begin();

  //Baud Rate
  Serial.begin(115200);
  
  while (!huskylens.begin(Wire))
  {
    //Inisialisasi HuskyLens
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
  //Inisialisasi Motor Servo
  servo_9.attach(7);
  servo_10.attach(8);
  
  //Kalibrasi Servo
  mind_n_currentx = 90;
  mind_n_currenty = 70;
  servo_9.angle(abs(mind_n_currentx));
  servo_10.angle(abs(mind_n_currenty));
}

void loop() {
  //Membersihkan dan Mengatur Posisi LCD
  lcd.clear();
  lcd.setCursor(0, 0);

  //Membaca data dari HuskyLens
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if //Memeriksa data apakah dikenali
  (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if //Memeriksa apa objek terdeteksi
  (!huskylens.available()) lcd.println(F("TIDAK ADA OBJEK TERDETEKSI"));
  
  //Memproses data yang diterima
  else
  {
    Serial.println(F("###########"));
    while (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      printResult(result);
      driveBot(result);
      tampilan(result);
    }
  }
}

//Hasil pembacaan HuskyLens
void printResult(HUSKYLENSResult result) {
  //Memeriksa jenis data
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  }
  else if (result.command == COMMAND_RETURN_ARROW) { //Menandakan data hasil deteksi
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  }
  else { //Menandakan data tidak dikenali
    Serial.println("Object unknown!");
  }
}

  void driveBot(HUSKYLENSResult result)
  {
  //Posisi Koordinat Huskylens
  if ((result.xCenter > 170) && (result.ID == 1)) {
    mind_n_currentx -= 1;
    servo_9.angle(abs((constrain(mind_n_currentx, 0, 120))));
  }
  else if ((result.xCenter > 30) && (result.xCenter < 130) && (result.ID == 1)) {
    mind_n_currentx += 1;
    servo_9.angle(abs((constrain(mind_n_currentx, 0, 120))));
  }
  else if ((result.yCenter > 130) && (result.ID == 1)) {
    mind_n_currenty += 1;
    servo_10.angle(abs((constrain(mind_n_currenty, 0, 120))));
  }
  else if ((result.yCenter > 20) && (result.yCenter < 120) && (result.ID == 1)) {
    mind_n_currenty -= 1;
    servo_10.angle(abs((constrain(mind_n_currenty, 0, 120))));
  }
  }

void tampilan(HUSKYLENSResult result) {
  //Tampilan LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LEARNING ID:");
  lcd.setCursor(12, 0);
  lcd.print(result.ID);
  lcd.setCursor(0, 1);
  lcd.print ("OBJEK TERDETEKSI");
  delay(10);
}
