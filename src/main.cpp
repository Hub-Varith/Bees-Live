#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Wifi credentials
const char *ssid = "InfiniteX Office";
const char *password = "6msme99qas";
String serverName = "https://us-central1-beeslive-17ec1.cloudfunctions.net/webApi/sensor_data/new";
String fanStatusServer = "https://us-central1-beeslive-17ec1.cloudfunctions.net/webApi/fan_status/abc123";
String jsonOutput;

// #define DHTTYPE DHT21
//Buzzer pin
int BUZZERPIN = 18;

bool fan0Stat;
bool fan1Stat;

int fanPower0;
int fanPower1;

float humidity;
float temperature;

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
double fanPowerL;
double fanPowerR;

HTTPClient client;

void postDummyData()
{
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

void runFan()
{

  //There will be left and right fans 0 = left, 1 = right
  

  if (fan0Stat == true)
  {
    Serial.println("Running fan 0");
    //Move the left motor
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, HIGH);
    //Setting the motor speed
    ledcWrite(ledChannel, fanPower0);
  }
  else if (fan0Stat == false)
  {
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, LOW);
  }

  if (fan1Stat == true)
  {
    Serial.println("Running fan 1");
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, HIGH);
    ledcWrite(ledChannel, fanPower1);
  }
  else if (fan1Stat == false)
  {
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, LOW);
  }

  if (fan0Stat && fan1Stat == true)
  {
    Serial.println("Running both fans");
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, HIGH);
    ledcWrite(ledChannel, fanPower0);
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, HIGH);
    ledcWrite(ledChannel, fanPower1);
  }
  else if (fan0Stat and fan1Stat == false)
  {
    Serial.println("Stopping both fans");
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, LOW);
    digitalWrite(IN2A, LOW);
    digitalWrite(IN2B, LOW);
  }
}

void stopFan()
{
  if (fan0Stat == false)
  {
    digitalWrite(IN1A, LOW);
    digitalWrite(IN1B, LOW);
  }
  else if (fan1Stat == false)
  {
    digitalWrite(IN1B, LOW);
    digitalWrite(IN2B, LOW);
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
  doc["fan1Speed"] = fanSpeedL;
  doc["fan2Speed"] = fanSpeedR;
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
    temperature = t;
    humidity = h;
    if (t >= 95)
    {
      fanPower0 = 255;
      fanPower1 = 255;
      toggleBuzz("on");
      // fanPowerL = fanPower0 / 255;
      // fanPowerR = fanPower1 / 255;
    }
    else if (t > 80)
    {
      fanPower0 = 230;
      fanPower1 = 230;
      // fanPowerL = fanPower0 / 255;
      // fanPowerR = fanPower1 / 255;
    }
    else if (t > 70)
    {
      fanPower0 = 210;
      fanPower1 = 210;
      // fanPowerL = fanPower0 / 255;
      // fanPowerR = fanPower1 / 255;
    }
    else if (t < 65)
    {
      digitalWrite(IN1A, LOW);
      digitalWrite(IN1B, LOW);
      digitalWrite(IN2A, LOW);
      digitalWrite(IN2B, LOW);
    }
    fanPowerL = (fanPower0/ 255.0) * 100.0;
    fanPowerR = (fanPower1/ 255.0) * 100.0;
    postSensorData(t, h, String(fanPowerL), String(fanPowerR));
    Serial.println(fanPowerL);
    Serial.println(fanPowerR);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *F");
    // delay(2000);
  }
}

void getFanData()
{
  HTTPClient client;
  // client.useHTTP10(true);
  client.begin(fanStatusServer);
  client.GET();

  String payload = client.getString();
  String realPayload = payload.substring(9, 71);
  // Serial.println(realPayload);

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, realPayload);
  fan0Stat = doc["isFan1Active"];
  fan1Stat = doc["isFan2Active"];

  // if (fan0Stat == true)
  // {
  //   runFan(0, 255, 0);
  //   Serial.println("Run L fan");
  // }
  // else
  // {
  //   stopFan(0);
  // }

  // if (fan1Stat == true)
  // {
  //   runFan(1, 0, 255);
  // }
  // else
  // {
  //   stopFan(1);
  // }

  // if (httpCode > 0)
  // {
  //   String payload = client.getString();
  //   const int CAPACITY = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4);
  //   DynamicJsonDocument doc(1024);
  //   deserializeJson(doc, payload);
  //   String thing = doc["sensorId"];

  //   // if (err)
  //   // {
  //   //   Serial.print(F("deserializeJson() failed with code "));
  //   //   Serial.println(err.c_str());
  //   // }

  //   Serial.println(thing);

  //   // char json[500];
  //   // payload.replace("data:", "");
  //   // payload.replace(" ", "");
  //   // payload.replace("\n", "");
  //   // payload.trim();
  //   // payload.remove(0, 1);
  //   // payload.toCharArray(json, 500);
  //   // // Serial.println(payload);

  //   // // const int CAP = JSON_OBJECT_SIZE(4);
  //   // StaticJsonDocument<200> doc;
  //   // DeserializationError err = deserializeJson(doc, payload);
  //   // String isFan1Active = doc["sensorId"];
  //   // Serial.println(isFan1Active);

  //   // if (err)
  //   // {
  //   //   Serial.print(F("deserializeJson() failed with code "));
  //   //   Serial.println(err.c_str());
  //   // }

  //   // Serial.println(payload);
  //   // // Serial.println(isFan2Active);
  // }
}

void loop()
{
  getFanData();
  sendSensorData();
  runFan();
}