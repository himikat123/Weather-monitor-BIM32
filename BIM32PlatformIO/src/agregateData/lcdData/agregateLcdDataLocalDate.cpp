#include <TimeLib.h>
#include "./agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../languages/languages.hpp"

String AgregateLcdData::_localDate() {
    char buf[20];
    if(config.lang() == "en") sprintf(buf, "%s %02d", lang.monthShortName(month()), day()); 
    else if(config.lang() == "de") sprintf(buf, "%02d. %s", day(), lang.monthShortName(month()));
    else sprintf(buf, "%02d %s", day(), lang.monthShortName(month()));

    return String(buf);
}