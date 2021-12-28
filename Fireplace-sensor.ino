#include <ESP8266HTTPClient.h>

#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>



const int LDR_PIN = A0;

const int reads = 50;
const int timeBetweenReads = 100;

int readings[reads];
int highestVarience;
int currentVarience;
int lowestValue;
int highestValue;

const char* serverName = "http://10.1.0.202:5002/fire/";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin("ZNett", "kattlada");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  highestVarience = 0;
  lowestValue = 2000;
  highestValue = 0;
  
  //Get readings for (reads * timeBetweenReads) milliseconds
  for (int i = 0; i < reads; i++)
  {
    readings[i] = analogRead(LDR_PIN);
    delay(timeBetweenReads);
  }

  //Get highest varience between readings (highest 'flicker')
  for(int i = 0; i < reads-1; i++)
  {
    currentVarience = abs(readings[i] - readings[i + 1]);
    if(currentVarience > highestVarience)
    {
      highestVarience = currentVarience;
    }
    if(readings[i] > highestValue)
    {
      highestValue = readings[i];
    }
    if(readings[i] < lowestValue){
      lowestValue = readings[i];
    }
  }

  Serial.print(highestVarience);
  Serial.print(" ");
  Serial.print(highestValue);
  Serial.print(" ");
  Serial.println(lowestValue);  


  //Send data to HTTP
  WiFiClient client;
  HTTPClient http;

  String httpGET = "?var=" + String(highestVarience) + "&high=" + String(highestValue) + "&low=" + String(lowestValue);
  http.begin(client, serverName + httpGET);
  http.GET();
  http.end();
  
}
