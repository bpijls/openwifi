#include "HvAOpenWiFi.h"
#include <ESP8266HTTPClient.h>

HvAOpenWiFi hotspot;

void setup() {

  Serial.begin(115200);
  delay(10);

  hotspot.begin();
}

void loop() {
  HTTPClient http;
  http.begin("http://gps.buienradar.nl/getrr.php?lat=52.3592959&lon=4.9081197");
  uint16_t httpCode = http.GET();
  String response;

  if (httpCode == 200) {
    response = http.getString();
    Serial.println(response);
  }
  else
    hotspot.begin();

  http.end();

  
  delay(60*1000*5);  
}

