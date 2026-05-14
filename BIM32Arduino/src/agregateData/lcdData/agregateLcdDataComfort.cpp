#include "agregateLcdData.hpp"

#include "../../config.hpp"
#include "../../languages.hpp"
#include "../../state.hpp"

String AgregateLcdData::comfort() {
    String comfort = "";
    if(config.display.source.descr() == 1) {
        comfort = lang.comfort(state.comfort);

        if(state.iaq_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(state.iaq_level);
        }

        else if(state.co2_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(state.co2_level);
        }
    }
    return comfort;
}