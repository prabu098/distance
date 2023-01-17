#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
//waktu
#include <NTPClient.h>
#include <WiFiUdp.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Agrokantor"
#define WIFI_PASSWORD "agro5756"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCa33g1d73M7aV_-UhIuweBEpjKb3irnWs"
#define DATABASE_SECRET "UFrvjTl16RWqXpgEGg21YpCDIPGXD8aHYSJQT8GE"
/* 3. Define the RTDB URL */
#define DATABASE_URL "distanceview-dc1f0-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "prabu21.mhs@akb.ac.id"
#define USER_PASSWORD "prabumangku098"
//Membuat Variable
const int triggerPin=14;
const int echoPin=12;
long duration;
float distanceCm;
float distanceInch;


// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

//milis wifi
unsigned long previousMillis = 0;
unsigned long interval = 30000;

void wifi(){
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup()
{

  Serial.begin(115200);

pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);


wifi();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
//  config.database_url = DATABASE_URL;
//
//  /* Assign the callback function for the long running token generation task */
//  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;
  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in garden20056 Mode, see Authentications/garden20056Mode/garden20056Mode.ino

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  // otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop()
{
unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
      wifi();
    previousMillis = currentMillis;
  }

  long duration, jarak;
  digitalWrite(triggerPin, LOW);
  
