#include <TimeLib.h>
#include "./agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../languages/languages.hpp"

String AgregateLcdData::_localDate() {
    char buf[20];
    if(config.lang() == "en") sprintf(buf, "%s %02d %d", lang.monthShortName(month()), day(), year()); 
    else if(config.lang() == "de") sprintf(buf, "%02d. %s %d", day(), lang.monthShortName(month()), year());
    else sprintf(buf, "%02d %s %d", day(), lang.monthShortName(month()), year());

    return String(buf);
}