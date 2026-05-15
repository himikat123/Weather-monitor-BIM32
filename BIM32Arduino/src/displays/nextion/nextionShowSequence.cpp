#include "./nextion.hpp"

#include "../validate.hpp"
#include "../config.hpp"

/**
 * Display sequence
 */
void Nextion::_showSequence() {
    if(_forced) _nextion.writeNum("Main.sequence.en", config.display.source.tempIn.sens() == 4 ? 1 : 0);
    for(unsigned int i=0; i<4; i++) {
        if(_prevTempSequence[i] != _tempSequence[i] or _forced) {
            _nextion.writeStr("Main.tempSeq" + String(i) + ".txt", validate.temp(_tempSequence[i]) 
                ? (String(int(round(_tempSequence[i]))) + "°C") : "--"
            );
            _prevTempSequence[i] = _tempSequence[i];
        }
        if(_prevHumSequence[i] != _humSequence[i] or _forced) {
            _nextion.writeStr("Main.humSeq" + String(i) + ".txt", validate.hum(_humSequence[i])
                ? (String(int(round(_humSequence[i]))) + "%") : "--"
            );
            _prevHumSequence[i] = _humSequence[i];
        }
        if(_prevNameSequence[i] != _nameSequence[i] or _forced) {
            _nextion.writeStr("Main.txtSeq" + String(i) + ".txt", _nameSequence[i]);
            _prevNameSequence[i] = _nameSequence[i];
        }
    }
}