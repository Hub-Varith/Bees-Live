#include <Arduino.h>
#include <DHT.h>

// #define DHTTYPE DHT21
//Buzzer pin
int BUZZERPIN = 18;

#define DHTTYPE DHT21 // AM2301
#define DHTPIN 15
DHT dht(DHTPIN, DHTTYPE);

// DHTNEW mySensor(34);

// uint32_t count = 0;
// uint32_t start, stop;
// uint32_t errors[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//PWM for the motor driver
int ENA = 17;
int ENB = 16;

// Motor pins
int IN1A = 27;
int IN1B = 26;
int IN2A = 25;
int IN2B = 33;

//Const pwm values
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

//Sensor pins
const int sensor = 34;

void setup()
{
  Serial.begin(9600);
  //Declaring the pins
  pinMode(BUZZERPIN, OUTPUT);
  //Motor Driver pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);
  // pinMode(sensor, INPUT);
  dht.begin();

  //Figure out the PWM for both motors
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ENB, ledChannel);
  ledcAttachPin(ENA, ledChannel);
}

void toggleBuzz(String condition)
{
  if (condition == "off")
  {
    digitalWrite(BUZZERPIN, LOW);
  }
  else
  {
    digitalWrite(BUZZERPIN, HIGH);
  }
}

void runFan(int fan, int fanPower0, int fanPower1)
{
  if (fan == 0)
  {
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, HIGH);
    ledcWrite(ledChannel, fanPower0);
  }
  if (fan == 1)
  {
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, HIGH);
    ledcWrite(ledChannel, fanPower1);
  }
}

void showSensorData()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
 {
   Serial.print("Humidity: "); 
   Serial.print(h);
   Serial.print(" %\t");
   Serial.print("Temperature: "); 
   Serial.print(t);
   Serial.println(" *C");
   delay(2000);
 }
}

void loop()
{
  showSensorData();
}