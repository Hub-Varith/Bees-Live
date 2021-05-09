#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Wifi credentials
const char *ssid = "InfiniteX Office";
const char *password = "6msme99qas";
String serverName = "https://us-central1-beeslive-17ec1.cloudfunctions.net/webApi/sensor_data/new";
String jsonOutput;

// #define DHTTYPE DHT21
//Buzzer pin
int BUZZERPIN = 18;

//Define constants for AM2301
#define DHTTYPE DHT21 // AM2301
#define DHTPIN 15
DHT dht(DHTPIN, DHTTYPE);

//PWM for the motor driver
const int ENA = 17;
const int ENB = 16;

// Motor pins
const int IN1A = 27;
const int IN1B = 26;
const int IN2A = 25;
int IN2B = 33;

//Const pwm values
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

//Sensor pins
const int sensor = 34;

//Fan power
float fanPowerL;
float fanPowerR;

HTTPClient client;

void postDummyData() {
  HTTPClient client;
  client.begin(serverName);
  client.addHeader("Content-Type", "application/json");

  //Creating a json object
  const size_t CAP = JSON_OBJECT_SIZE(5);
  StaticJsonDocument<CAP> doc;
  JsonObject obj = doc.to<JsonObject>();
  //Json object structure
  obj["temperature"] = 25;
  obj["humidity"] = 28;
  obj["fanSpeed"] = "0.75";
  obj["userId"] = "qwe123";
  obj["sensorId"] = "AM2301";

  //Posting the data to the server
  
  serializeJson(doc, jsonOutput);
  client.POST(jsonOutput);
  client.end();
  Serial.println(jsonOutput);
  jsonOutput = "";
}

void setup()
{
  Serial.begin(9600);

  //Declaring the pins
  pinMode(BUZZERPIN, OUTPUT);

  //Motor Driver PWM pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Motor Driver Control Pins
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);

  //Start connecting to WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WIFI CONNECTED SUCCESSFULLY");
    // postDummyData();
  }
  Serial.println("");
  Serial.print(WiFi.localIP());

  client.begin(serverName);
  client.addHeader("Content-Type", "application/json");

  //Start up the sensor
  dht.begin();

  //Figure out the PWM for both motors
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ENB, ledChannel);
  ledcAttachPin(ENA, ledChannel);
}

// Toggle buzzer function
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
  //There will be left and right fans 0 = left, 1 = right
  fanPowerL = fanPower0/255;
  fanPowerR = fanPower1/255;

  if (fan == 0)
  {
    //Move the left motor
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, HIGH);
    //Setting the motor speed
    ledcWrite(ledChannel, fanPower0);
  }
  if (fan == 1)
  {
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, HIGH);
    ledcWrite(ledChannel, fanPower1);
  }
}

void postSensorData(float temperature, float humidity, String fanSpeedL, String fanSpeedR)
{
  //Instantiating the HTTP CLIENT
  HTTPClient client;
  client.begin(serverName);
  client.addHeader("Content-Type", "application/json");

  //Creating a json object
  DynamicJsonDocument doc(1024);

  //Json object structure
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["fanSpeed"] = fanSpeedL;
  doc["userId"] = "esp";
  doc["sensorId"] = "AM2301";

  //Posting the data to the server
  
  serializeJson(doc, jsonOutput);
  client.POST(jsonOutput);
  client.end();
  Serial.println(jsonOutput);
  jsonOutput = "";
  


  // int httpCode = client.POST();
}

void sendSensorData()
{
  //get the sensor data
  float h = dht.readHumidity();
  float celcius = dht.readTemperature();

  //Converting Celcius into Fahrenheit
  float t = (celcius * 9 / 5) + 32;

  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    postSensorData(t, h, String(fanPowerL), String(fanPowerR));
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *F");
    delay(2000);
  }
}

void loop()
{
  sendSensorData();
  // postDummyData();
  // delay(2000);
  // Serial.println("sent");

}