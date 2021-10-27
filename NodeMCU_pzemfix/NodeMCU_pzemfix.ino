/*
  Nama           : Asrul Kadir
  NIM            : 1524042008
  Prodi          : PTE S1 2015
*/
//NodeMcu
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int pinBuzzer = 4;

float tegangan;
float arus;
float daya;
float energi;

float datain1;
float datain2;
float datain3;
float datain4;

float limit = 99;
int be;
int resetEnergi;
int timer1;

float tarif = 1444.70;
float tagihan;

char auth[] = ""; // insert auth token

char ssid[] = "wifi";
char pass[] = "password";

BLYNK_WRITE(V10) {
  limit = param.asFloat();
  be = 1;
}

BLYNK_WRITE(V11) {
  resetEnergi = param.asInt();
  Serial.print(resetEnergi);
}

BLYNK_WRITE(V12) {
  timer1 = param.asInt();
}

BLYNK_READ(V13) {
  tagihan = tarif * energi + tarif * energi * 0.1;
  Blynk.virtualWrite(V13, tagihan);
}

BLYNK_READ(V2) {
  Blynk.virtualWrite(V2, tegangan);
}

BLYNK_READ(V3) {
  Blynk.virtualWrite(V3, arus);
}

BLYNK_READ(V4) {
  Blynk.virtualWrite(V4, daya);
}

BLYNK_READ(V5) {
  Blynk.virtualWrite(V5, energi);
}


void setup() {
  Serial.begin(9600);
  //  Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    datain1 = Serial.parseFloat();
    datain2 = Serial.parseFloat();
    datain3 = Serial.parseFloat();
    datain4 = Serial.parseFloat();

    if (Serial.read() == '\n') {
      tegangan = datain1;
      arus = datain2;
      daya = datain3;
      energi = datain4;
    } else if (Serial.read() != datain1 && Serial.read() != datain2 && Serial.read() != datain3) {
      tegangan = 0;
      arus = 0;
      daya = 0;
    }

    Serial.write(resetEnergi);
  }

  Blynk.run();
  if (energi > limit && be == 1) {
    Blynk.notify("Peringatan: Pemakaian Listrik Telah Melewati Batas Wajar, Segera Hemat Energi");
    delay(1000);
    be = 2;
  } else if (energi > limit && be == 2) {
    digitalWrite(pinBuzzer, HIGH);
    delay(500);
    digitalWrite(pinBuzzer, LOW);
    delay(1000);
  }

  if (timer1 == 1) {
    Blynk.notify(String("Total Penggunaan Energi Listrik Sampai Hari Ini Sebesar ") + energi + String(" kWh"));
  }

  if (resetEnergi == 1 && energi == 0) {
    Blynk.notify("Energi Telah Direset");
  }
}
