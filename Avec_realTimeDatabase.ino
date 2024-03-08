// inclusons nos differents librairies
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "EPT_PERSONNEL"
#define WIFI_PASSWORD "40@ept2015"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCCQyIJhvYH_JTO-h8fdBC1BnfZXJ9GX9k"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://gnaa-76c6b-default-rtdb.europe-west1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

// valeur initiale des coordonnees
float latitude = 14.757185;  
float longitude = -17.351424;

bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(2000);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Assign the api key (required) 
  config.api_key = API_KEY;

 //Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK ){
    
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setFloat(&fbdo, "coordonnees_1/latitude", latitude)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "coordonnees_1/longitude",longitude )){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    latitude+=0.000001;
    longitude+=0.000001;
  }
  delay(1000);
}
