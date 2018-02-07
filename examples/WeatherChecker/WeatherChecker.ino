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
  http.begin("http://icu.feib.nl/get_connections.php");
  {
    uint16_t httpCode = http.GET();
    String response = http.getString();
    Serial.println(httpCode);
    Serial.println(response);

    if (httpCode != 200) ESP.reset();
  }
  http.end();

  // Wait for 5 minutes
  delay(60 * 1000 * 5);
}
