/*
  Nama           : Asrul Kadir
  NIM            : 1524042008
  Prodi          : PTE S1 2015
*/
//Arduino
#include <PZEM004Tv30.h>
#include <Wire.h>  // i2C Conection Library
#include <LiquidCrystal_I2C.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

PZEM004Tv30 pzem(11, 12);

int dataIn;
float tarif = 1444.70;
float tagihan;

void setup() {
  lcd.init();
  lcd.noCursor();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  pzem.setAddress(0x42);
}

void loop() {

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float tagihan = tarif * energy + tarif * energy * 0.1;

  Serial.print(voltage);
  Serial.print("\n");
  Serial.print(current);
  Serial.print("\n");
  Serial.print(power);
  Serial.print("\n");
  Serial.print(energy,3);
  Serial.print("\n");

  lcd.setCursor(2, 0);
  lcd.print(voltage); lcd.print("V / "); lcd.print(current); lcd.print("A");
  lcd.setCursor(0, 1);
  lcd.print("Daya= "); lcd.print(power); lcd.print("Watt");
  lcd.setCursor(0, 2);
  lcd.print("Energi= "); lcd.print(energy,3); lcd.print("kWh");
  lcd.setCursor(0, 3);
  lcd.print("Rp "); lcd.print(tagihan,2); 

  while(Serial.available() > 0) {
    dataIn = Serial.parseInt();
  }

  if (dataIn == 1) {
    pzem.resetEnergy();
    return;
  }

  delay(1000);
}
