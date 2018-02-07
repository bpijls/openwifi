#include "OpenWiFi.h"
#include <ESP8266WiFi.h>

#define NSUFFIX_CHARS 11

bool OpenWiFi::begin(String backupSSID, String backupPassword, bool forceBackup){

  // Assume not to use backup network
  _useBackup = false;
  _backupSSID = backupSSID;
  _backupPassword = backupPassword;

  // Stop trying to connect after 15000 milliseconds  
  _connectionTimeout = 20000;

  if (!forceBackup){
    // Scan for HvA or UvA networks
    int nNetworks= WiFi.scanNetworks();
    for (int iSSID = 0; iSSID < nNetworks; ++iSSID){
      String ssid = WiFi.SSID(iSSID);
      Serial.println(ssid);
      if (ssid.endsWith(" Open Wi-Fi")){
        _ssid = ssid; 
        _organization = ssid.substring(0, ssid.length() - NSUFFIX_CHARS);
      }
    }
    _useHotspot =  _ssid.length() > 0;
    _useBackup = _backupSSID.length() > 0 ;
  }
  else{
    _useBackup = true;
    _useHotspot = false;
  }
 
  
  // If HvA or UvA network has not been found, use backup network
  if (!_useHotspot){ // unless ssid and pw are empty
    if (_useBackup){
      _ssid = _backupSSID;
      _password = _backupPassword;
      _useBackup = true;
      Serial.println("No HVA or UVA hotspot found. Using backup network.");
    }
    else{
      Serial.println("No suitable networks found.");
      return false;
    }
  }  
  
  if (connectWiFi()){
    // only register to captive portal if on UvA or HvA network
    if (!_useBackup) {
      Serial.println("Refreshing HotSpot connection.");
      connectHotspot();
    }
  }
  
  return isConnected();
}

bool OpenWiFi::connectWiFi(){

  Serial.println(String("Connecting to ") + _ssid);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid.c_str(), _password.c_str());
  uint16_t timeout = 0, lastMillis = millis();

  // Try connecting for a while
  while (!isConnected() && (timeout < _connectionTimeout)) {
    delay(500);
    Serial.print(String(". ") + timeout + " ");
    timeout += millis() - lastMillis;
    lastMillis = millis();
  }

  // Success
  if (isConnected()){
    _gatewayIP = WiFi.gatewayIP().toString();
    _localIP = WiFi.localIP().toString();
    _subnetMask = WiFi.subnetMask().toString();
    _dns = WiFi.dnsIP().toString();
  }
  else {
    debugMessage("'Could not connect to Open WiFi or backup network.");
  }

  return isConnected();
}

void OpenWiFi::connectHotspot() {

  // Send a POST request to the gateway to register device on 
  // OpenWiFi network
  String redirHost = _gatewayIP + ":8002",
    request = String("POST / HTTP/1.1\r\n") +
    "Host: " + redirHost + "\r\n" +   
    "Origin: http://" + redirHost + "\r\n" +  
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "Referer: http://" + redirHost + "/index.php\r\n" +
    "Content-Length: 52\r\n\r\n" +
    "redirurl=http%3A%2F%2Fwww." + _organization + ".nl%2F&accept=Verbinden";
  
    // Perform the POST ("press the "connect" button)
  _performRequest(_gatewayIP, 8002, request);

  debugMessage(request);
}

void OpenWiFi::setConnectionTimeout(uint16_t connectionTimeout){
    _connectionTimeout = connectionTimeout;
}

bool OpenWiFi::isConnected(){
  return (WiFi.status() == WL_CONNECTED);
}
  

String OpenWiFi::_performRequest(String host, uint16_t port, String request)
{

  debugMessage(String("connecting to ") + host);

  WiFiClient client;
  String response;

  if (!client.connect(host.c_str(), port)) {
    debugMessage("connection failed");
    return "";
  }

  client.print(request);

  unsigned long timeout = millis();
  while (client.available() == 0)
    if (millis() - timeout > 5000) {
      debugMessage(">>> Client Timeout !");
      client.stop();
      return "";
    } 

  // Read all the lines of the reply from server and print them to Serial
  while (client.available())
    response += client.readStringUntil('\r');

  return response;

}

void OpenWiFi::debugMessage(String message){
  Serial.println(String("OW:" + message));
}