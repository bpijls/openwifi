/*
  OpenWiFi.h - Library for using the HvA OpenWiFi accesspoint
*/
#ifndef OpenWifi_h
#define OpenWifi_h

#pragma once

#include <Arduino.h>


class OpenWiFi
{
public:
	
	bool connectWiFi();
	void connectHotspot();
	bool begin(String backupSSID, String backupPassword, bool forceBackup);
	void setConnectionTimeout(uint16_t connectionTimeout);
	bool isConnected();
    void debugMessage(String message);
  
private:
    String _performRequest(String host, uint16_t port, String request);

    // Organization can be HvA or UvA
    String _organization;
    // Connection Information
    String   _gatewayIP, _localIP, _subnetMask, _dns, _ssid, _password, _backupSSID, _backupPassword;
    bool _useBackup, _useHotspot;
    uint16_t _connectionTimeout;

};

#endif
