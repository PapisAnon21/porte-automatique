#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif


#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include <addons/TokenHelper.h>

//insert our inforamtions networks
const char* ssid = "EPT_PERSONNEL";
const char* password = "40@ept2015";

//la clé d'api du pprojet firebase
#define API_KEY "AIzaSyCCQyIJhvYH_JTO-h8fdBC1BnfZXJ9GX9k"

// Si on souhaite ajouter un systeme d'authentification
//pour le moment on fait un test sans une  authentification

#define USER_EMAIL "diengm@ept.sn"
#define USER_PASSWORD "Mouhamed"

// la nous mettons l'ID bucket qui est en quelque sorte l'identifiant de la
// base de donnee
#define STORAGE_BUCKET_ID "gnaa-76c6b.appspot.com"


//Creation des services de fireabse tel que la base de donne l'authentif et la conf
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;





void initWiFi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}




void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  //Firebase
  // Assign the api key
  configF.api_key = API_KEY;
  //Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  //Assign the callback function for the long running token generation task
  configF.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  /*if (Firebase.signUp(&configF, &auth, "", "")){
    Serial.println("ok");
   
  }*/
  
  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  
  
    //MIME type should be valid to avoid the download problem.
    //The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
    //il y'a deux type de stockage memoire mem_storage_type_flash, mem_storage_type_sd
    //if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID , FILE_PHOTO , mem_storage_type_flash , mem_storage_type_flash and mem_storage_type_sd */, FILE_PHOTO /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */)){
     // Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
    //}
    if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, "donnee", mem_storage_type_sd, "data/donnee", "text/str")){
      Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
    }
    else{
      Serial.println(fbdo.errorReason());
      Serial.println("c ici");
    }
    delay(5000); 
  }
