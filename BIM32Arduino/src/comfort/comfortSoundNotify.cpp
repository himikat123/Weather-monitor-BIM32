#include "./comfort.hpp"
#include "../agregate/agregateComfortData.hpp"
#include "../validate.hpp"
#include "../config.hpp"

void Comfort::soundNotify() {
    if(config.comfort.temp.sound()) sound.tempNotify(_tempLevel);
    if(config.comfort.hum.sound()) sound.humNotify(_humLevel);
    if(config.comfort.iaq.sound()) sound.airNotify(_iaqLevel);
    else if(config.comfort.co2.sound()) sound.airNotify(_co2Level);
}