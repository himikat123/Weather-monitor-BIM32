#pragma once
#include <Arduino.h>
#include "driver/rmt.h"

#include "../globals.hpp"

class Sound {
    public:
        static Sound& getInstance() {
            static Sound instance;
            return instance;
        }

        Sound(const Sound&) = delete;
        void operator=(const Sound&) = delete;

        void init(void);
        void volume(unsigned int vol);
        void equalizer(unsigned int eq);
        void play(unsigned int folder, unsigned int track);
        void stopPlaying(void);
        void hourlySignal(void);
        void alarm(void);
        void tempNotify(int tempLevel);
        void humNotify(int humLevel);
        void airNotify(unsigned int airLevel);

    private:
        Sound() {}

        bool _isAllowed();
        bool _hourlyCheck();
        void _playHourlySignal();
        void _reset();
        uint16_t _chckSum(uint8_t *sdata);
        void _sendCommand(uint8_t command, uint8_t hByte, uint8_t lByte);
        void _sendRmt(const uint8_t *data, size_t len);

        unsigned int _alarm_rang = 60;
        unsigned int _hourly_rang = 0;
        int _prevTempLevel = TEMP_COMFORTABLE;
        int _prevHumLevel = HUM_COMFORTABLE;
        unsigned int _prevAirLevel = AIR_CLEAN;
        boolean _mp3_found = false;
};

inline Sound& sound = Sound::getInstance();