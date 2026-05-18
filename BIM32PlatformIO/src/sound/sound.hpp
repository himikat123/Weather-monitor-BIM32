#pragma once
#include <Arduino.h>

#include "../globals.hpp"

class Sound {
    public:
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
        bool _isAllowed();
        bool _hourlyCheck();
        void _playHourlySignal();
        void _reset();
        uint16_t _chckSum(uint8_t *sdata);
        void _sendCommand(uint8_t command, uint8_t hByte, uint8_t lByte);

        unsigned int _alarm_rang = 60;
        unsigned int _hourly_rang = 0;
        int _prevTempLevel = TEMP_COMFORTABLE;
        int _prevHumLevel = HUM_COMFORTABLE;
        unsigned int _prevAirLevel = AIR_CLEAN;
        boolean _mp3_found = false;
};

extern Sound sound;