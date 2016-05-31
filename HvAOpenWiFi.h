
#pragma once

#include <Arduino.h>


class HvAOpenWiFi
{
public:
	void connectWiFi();
	void connectHotspot();
	void begin();
	void statusReport();
	
private:
    String performRequest(String host, uint16_t port, String request);

    bool _firstTimeConnected = false;
    String initialRequest;
    String postRequest;

    String _redirHost, _redirURI;
    uint16_t _redirPort;


};

