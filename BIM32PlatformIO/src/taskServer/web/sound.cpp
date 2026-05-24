#include <Arduino.h>
#include "../taskServer.hpp"

#include "../../config/config.hpp"
#include "../../sound/sound.hpp"

/**
 * Change sound volume
 */
void TaskServer::_webSoundVolume() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("vol")) {
            config.sound.setVol(_server.arg("vol").toInt());
            sound.volume(config.sound.vol());
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Change equalizer settings of the mp3 player
 */
void TaskServer::_webSoundEqualizer() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("eq")) {
            config.sound.setEq(_server.arg("eq").toInt());
            sound.equalizer(config.sound.eq());
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Play a sound track
 */
void TaskServer::_webSoundPlay() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("folder") and _server.hasArg("track")) {
            sound.play(_server.arg("folder").toInt(), _server.arg("track").toInt());
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Stop playing a sound track
 */
void TaskServer::_webSoundStop() {
    if(_webIsLogged(true)) {
        sound.stopPlaying();
        _server.send(200, "text/plain", "OK");
    }
}