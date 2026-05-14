#include "./nextion.hpp"

#include "../validate.hpp"

/**
 * Sending data to Network page
 */
void Nextion::_networkPage() {
    if(_prevNetLogo != _netLogo or _forced) {
        _nextion.writeStr("Network.Logo.txt", _netLogo);
        _prevNetLogo = _netLogo;
    }
    if(_prevNetSsid != _netSsid or _forced) {
        _nextion.writeStr("Network.ssid.txt", _netSsid);
        _prevNetSsid = _netSsid;
    }
    if(_prevNetRssi != _netRssi or _forced) {
        _nextion.writeStr("Network.rssi.txt", _netRssi);
        _prevNetRssi = _netRssi;
    }
    if(_prevNetIp != _netIp or _forced) {
        _nextion.writeStr("Network.ip.txt", _netIp);
        _prevNetIp = _netIp;
    }
    if(_prevNetMac != _netMac or _forced) {
        _nextion.writeStr("Network.mac.txt", _netMac);
        _prevNetMac = _netMac;
    }
    if(_prevNetTemp != _netTemp or _forced) {
        _nextion.writeStr("Network.temp.txt", validate.temp(_netTemp) ? String(int(round(_netTemp))) + "°C" : "--");
        _prevNetTemp = _netTemp;
    }
    if(_prevNetFw != _netFw or _forced) {
        _nextion.writeStr("Network.frmw.txt", _netFw);
        _prevNetFw = _netFw;
    }
}