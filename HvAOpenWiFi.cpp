#include "HvAOpenWiFi.h"
#include <ESP8266WiFi.h>

void HvAOpenWiFi::connectWiFi(){

  WiFi.mode(WIFI_STA);
  WiFi.begin("HvA Open Wi-Fi", "");    

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }   
 

  _gatewayIP = WiFi.gatewayIP().toString();
  _localIP = WiFi.localIP().toString();
  _subnetMask = WiFi.subnetMask().toString();
  _dns = WiFi.dnsIP().toString();
}

void HvAOpenWiFi::begin(){

  Serial.println("Refreshing WiFi connection.");
  connectWiFi();


  Serial.println("Refreshing HotSpot connection.");
  connectHotspot();
  
}

void HvAOpenWiFi::connectHotspot() {

  String response;
  
  //_firstTimeConnected = true;
 // String host = "captive.apple.com";

  //     // Connect to random address (Using default applecaptive portal here)
  // response = performRequest(host, 80,
  //  String("GET /hotspot_detect.html HTTP/1.1\r\n") +
  //  "Host: " + host + ":80\r\n" +  
  //  "Connection: close\r\n" +   
  //  "\r\n");

  //     // If response code is OK, we're probably already connected
  // uint16_t responseCode = response.substring(9, 12).toInt();

  // if (responseCode == 200)
  //   return;

  //     // Fetch the redirection IP, port and URL from the headers
  // response = response.substring(response.indexOf("Location: http://") + 17 , response.length());
  // response = response.substring(0, response.indexOf("\n"));
  // String redirHost = response.substring(0, response.indexOf(":"));
  // String redirURI = response.substring(response.indexOf("/"), response.length());
  // uint16_t redirPort = response.substring(response.indexOf(":") + 1, response.indexOf("/")).toInt();  
  
  String redirHost = _gatewayIP,
    redirURI = "/index.php?zone=hva";
    uint16_t redirPort = 8002;

    // Perform the POST ("press the "connect" button)
  response = _performRequest(redirHost, redirPort,
    String("POST / HTTP/1.1\r\n") +
    "Host: " + redirHost + ":" + redirPort + "\r\n" +   
    "Origin: http://" + redirHost + ":" + redirPort + "\r\n" +  
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "Referer: http://" + redirHost + ":" + redirPort + "/index.php\r\n" +
    "Content-Length: 52\r\n\r\n" +
    "redirurl=http%3A%2F%2Fwww.hva.nl%2F&accept=Verbinden");

}

String HvAOpenWiFi::_performRequest(String host, uint16_t port, String request)
{

  Serial.println("connecting to ");
  Serial.println(host);

  WiFiClient client;
  String response;

  if (!client.connect(host.c_str(), port)) {
    Serial.println("connection failed");
    return "";
  }

  client.print(request);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "";
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    response += client.readStringUntil('\r');
  }

  return response;
}