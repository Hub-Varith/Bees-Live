#include <Arduino.h>
#include <analogWrite.h>

//Buzzer pin
int BUZZERPIN = 18;

//PWM for the motor driver
int ENA = 4;
int ENB = 16;

// Motor pins
int IN1A = 27;
int IN1B = 26;
int IN2A = 25;
int IN2B = 33;
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

//Sensor pins




void setup() {
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ENB, ledChannel);
  


  // put your setup code here, to run once:
}

void toggleBuzz(String condition) {
  if (condition == "off") {
    digitalWrite(BUZZERPIN, LOW);
  } else {
    digitalWrite(BUZZERPIN, HIGH);
  }
}

void runFan(int fan, int fanPower0, int fanPower1) {
  if (fan == 0) {
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, HIGH);
  }
  if (fan == 1) {
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, HIGH);
    ledcWrite(ledChannel, fanPower0);
  }
}

void loop() {

  
  // runFan(1, 100, 0);
  // digitalWrite(IN2A, LOW);
  // digitalWrite(IN2B, HIGH);
  // digitalWrite(IN1A, LOW);
  // digitalWrite(IN1B, HIGH);
  // put your main code here, to run repeatedly:
}