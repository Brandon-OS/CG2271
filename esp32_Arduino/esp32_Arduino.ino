#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

#define RXD2 16
#define TXD2 17

// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PS4.begin("80:45:dd:e4:c9:fa"); // change to esp32 mac address
  Serial.println("Ready.");

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);
}

void loop() {
  String response = "";
  // put your main code here, to run repeatedly:
  if (PS4.isConnected()) {
    if (PS4.Right()) {
      response = "right";
      Serial2.write(0x33);
    }
    if (PS4.Down()) {
      response = "backward";
      Serial2.write(0x31);
    }
    if (PS4.Up()) {
      response = "forward";
      Serial2.write(0x30);
    }
    if (PS4.Left()) {
      response = "left ";
      Serial2.write(0x32);
    }

    if (PS4.UpRight()) {
      response = "right forward";
      Serial2.write(0x35);
    }
    if (PS4.DownRight()) {
      response = "right backward";
      Serial2.write(0x37);
    }
    if (PS4.UpLeft()) {
      response = "left forward";
      Serial2.write(0x36);
    }
    if (PS4.DownLeft()) {
      response = "left backward";
      Serial2.write(0x38);
    }

    if (PS4.L2()) {
      response = "stop";
      Serial2.write(0x34);
    }
    if (PS4.L1()) {
      response = "stop sound";
      Serial2.write(0x40);
    }
    if (PS4.R2()) {
      response = "finish";
      Serial2.write(0x39);
    }
    if (PS4.R1()) {
      response = "finish sound";
      Serial2.write(0x41);
    }
    if (response != "") Serial.println(response);
  }
}
