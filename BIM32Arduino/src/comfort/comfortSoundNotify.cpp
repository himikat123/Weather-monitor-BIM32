#include "./comfort.hpp"
#include "../agregateData/comfortData/agregateComfortData.hpp"
#include "../validation/validate.hpp"
#include "../config/config.hpp"

void Comfort::soundNotify() {
    if(config.comfort.temp.sound()) sound.tempNotify(_tempLevel);
    if(config.comfort.hum.sound()) sound.humNotify(_humLevel);
    if(config.comfort.iaq.sound()) sound.airNotify(_iaqLevel);
    else if(config.comfort.co2.sound()) sound.airNotify(_co2Level);
}