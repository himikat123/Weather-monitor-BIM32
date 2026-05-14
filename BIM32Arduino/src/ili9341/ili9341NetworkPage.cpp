#include "./ili9341.hpp"

#include <WiFi.h>
#include "../languages.hpp"
#include "../config.hpp"

/**
 * Display Netoerk page
 */
void ILI9341::_networkPage() {
    const uint8_t x = 160, y = 2, sl = 6, w = 150;
    const uint8_t sr = x + sl;

    if(!_networkSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _networkSkeleton = true;
        _forced = true;
        tft.fillRect(48, 10, 226, 39, FRAME1_COLOR);
        tft.fillRect(50, 12, 222, 35, GROUND_COLOR);
        _printText(50, 17, 222, 28, "WiFi", FONT3, CENTER, TEXT_COLOR, GROUND_COLOR);

        for(uint8_t i=0; i<2; i++) {
            for(uint8_t k=0; k<6; k++) {
                tft.fillRect(i * x + 4, k * 30 + 60, 154, 24, FRAME1_COLOR);
                tft.fillRect(i * x + sl, k * 30 + 62, w, 20, GROUND_COLOR);
            }
        }
        _printText(sl, 64 + y, w, 12, lang.network(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 94 + y, w, 12, lang.signalLevel(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 124 + y, w, 12, lang.ipAddr(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 154 + y, w, 12, lang.macAddr(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 185 + y, w, 12, lang.esp32Temp(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 214 + y, w, 12, lang.firmware(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    }

    String ssid = state.apMode ? config.accessPoint.ssid() : WiFi.SSID();;
    String rssi = state.apMode ? "100%" : String(_rssi) + "dBm";
    String ip = state.apMode ? config.accessPoint.ip() : WiFi.localIP().toString();;
    String mac = state.apMode ? WiFi.softAPmacAddress() : WiFi.macAddress();
    float esp32Temp = sensors.get_esp32_temp();
    String fw = FW;

    if(_prevNetSsid != ssid || _forced) 
        _printText(sr, 64 + y, w, 12, ssid, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetRssi != rssi || _forced) 
        _printText(sr, 94 + y, w, 12, rssi, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetIp != ip || _forced) 
        _printText(sr, 124 + y, w, 12, ip, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetMac != mac || _forced) 
        _printText(sr, 154 + y, w, 12, mac, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetTemp != esp32Temp || _forced) {
        String buf = String((int)round(esp32Temp));
        buf += "°C"; 
        _printText(sr, 184 + y, w, 12, buf, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    }
    if(_prevNetFw != fw || _forced) 
        _printText(sr, 214 + y, w, 12, fw, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);

    _prevNetSsid = ssid;
    _prevNetRssi = rssi;
    _prevNetIp = ip;
    _prevNetMac = mac;
    _prevNetTemp = esp32Temp;
    _prevNetFw = fw;
    _forced = false;
}