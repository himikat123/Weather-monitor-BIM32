#include <Arduino.h>
#include <WiFi.h>

#include "./taskData.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../network/network.hpp"

void TaskData::_networkState() {
    if(state.apMode) strlcpy(state.network.ssid, config.accessPoint.ssid(), sizeof(state.network.ssid));
    else WiFi.SSID().toCharArray(state.network.ssid, sizeof(state.network.ssid));

    state.network.ch = WiFi.channel();
    state.network.sig = WiFi.RSSI();

    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(state.network.mac, sizeof(state.network.mac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    IPAddress ip = WiFi.localIP();
    IPAddress mask = WiFi.subnetMask();
    IPAddress gw = WiFi.gatewayIP();
    IPAddress dns1 = WiFi.dnsIP();
    IPAddress dns2 = WiFi.dnsIP();
    snprintf(state.network.ip, sizeof(state.network.ip), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    snprintf(state.network.mask, sizeof(state.network.mask), "%u.%u.%u.%u", mask[0], mask[1], mask[2], mask[3]);
    snprintf(state.network.gw, sizeof(state.network.gw), "%u.%u.%u.%u", gw[0], gw[1], gw[2], gw[3]);
    snprintf(state.network.dns1, sizeof(state.network.dns1), "%u.%u.%u.%u", dns1[0], dns1[1], dns1[2], dns1[3]);
    snprintf(state.network.dns2, sizeof(state.network.dns2), "%u.%u.%u.%u", dns2[0], dns2[1], dns2[2], dns2[3]);

    state.network.updated = true;
}