
#pragma once

#include <Arduino.h>


class HvAOpenWiFi
{
public:
	void connectWiFi();
	void connectHotspot();
	void begin();
		
private:
    String _performRequest(String host, uint16_t port, String request);

    String   _gatewayIP, _localIP, _subnetMask, _dns;

};

