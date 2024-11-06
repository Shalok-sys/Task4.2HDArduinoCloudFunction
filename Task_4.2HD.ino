#include "Firebase_Arduino_WiFiNINA.h"

#define DATABASE_URL "remotelights-cd22c-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "5QgiyE5Odu8MG6aTuWciqjF7vGd8GOPdxvDLFgKa"
#define WIFI_SSID "builder"
#define WIFI_PASSWORD "splicing"

//Define Firebase data object
FirebaseData fbdo;

void setup()
{
  pinMode(2, OUTPUT); // red
  pinMode(3, OUTPUT); // green
  pinMode(4, OUTPUT); // blue
  Serial.begin(9600);
  delay(100);
  Serial.println();

  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Provide the autntication data
  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

}

void loop()
{
  String path = "/LEDStatus";

  Serial.print("Get Values... ");

  if (Firebase.getString(fbdo, path + "/Status")) // also can use Firebase.get(fbdo, path)
  {
    Serial.println("ok");
    Serial.println("path: " + fbdo.dataPath());
    Serial.println("type: " + fbdo.dataType());
    Serial.print("value: ");
    if (fbdo.dataType() == "string"){
      Serial.println(fbdo.stringData());
      LEDToggle(fbdo.stringData());
    }
  }
  else
  {
    Serial.println("error, " + fbdo.errorReason());
  }

  Serial.println();

  // clear internal memory used
  fbdo.clear();
  delay(500);
}

void LEDToggle(String val){
  if(val == "red"){
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }
  if(val == "green"){
    digitalWrite(3,HIGH);
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
  }
    if(val == "blue"){
    digitalWrite(4,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
  }
  if(val == "none"){
    digitalWrite(4,LOW);
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
  }
}
