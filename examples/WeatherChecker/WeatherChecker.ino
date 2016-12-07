#include "OpenWiFi.h"
#include <ESP8266HTTPClient.h>

OpenWiFi hotspot;

void setup() {

  Serial.begin(115200);
  delay(10);

  hotspot.begin("HomeNetwork", "HomePassword");
}

void loop() {
  HTTPClient http;
  http.begin("http://gadgets.buienradar.nl/data/raintext?lat=52.3409792&lon=4.9077414");
  uint16_t httpCode = http.GET();
  String response;

  if (httpCode == 200) {
    response = http.getString();
    Serial.println(response);
  }
  else
     ESP.reset();

  http.end();

  
  delay(60*1000*5);  
}
