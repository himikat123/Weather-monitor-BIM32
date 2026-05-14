#include "./comfort.hpp"
#include "../sensors.hpp"

void Comfort::devicesControl() {
    sensors.comfortDevices(_heater, _cooler, _humidifier, _dehumidifier, _purifier);
}