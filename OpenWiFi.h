
#pragma once

#include <Arduino.h>


class OpenWiFi
{
public:
	
	bool connectWiFi();
	void connectHotspot();
	bool begin(String backupSSID, String backupPassword);
	void setConnectionTimeout(uint16_t connectionTimeout);
	bool isConnected();

private:
    String _performRequest(String host, uint16_t port, String request);

    // Organization can be HvA or UvA
    String _organization;
    // Connection Information
    String   _gatewayIP, _localIP, _subnetMask, _dns, _ssid, _password, _backupSSID, _backupPassword;
    bool _useBackup;
    uint16_t _connectionTimeout;

};

