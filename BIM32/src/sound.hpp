class Sound {
    public:
        void init(void);
        void volume(unsigned int vol);
        void equalizer(unsigned int eq);
        void play(unsigned int folder, unsigned int track);
        void stopPlaying(void);
        void hourlySignal(void);
        void alarm(void);

    private:
        void _playHourlySignal();
        void _reset();
        uint16_t _chckSum(uint8_t *sdata);
        void _sendCommand(uint8_t command, uint8_t hByte, uint8_t lByte);

        unsigned int _alarm_rang = 60;
        unsigned int _hourly_rang = 0;
        boolean _mp3_found = false;
};

/**
 * DF player initialization
 */
void Sound::init(void) {
    Serial.println(SEPARATOR);
    Serial.println("Initialize DF Player...");

    if(digitalRead(MP3_BUSY_PIN)) {
        time_t mils = millis();
        _reset();
        while(1) {
            if(!global.mp3_busy) {
                global.mp3_busy = true;
                _mp3_found = true;
                break;
            }
            if(millis() - mils > 2000) break;
        }
    }
    Serial.printf("DFPlayer %sfound\r\n", _mp3_found ? "" : "NOT ");
}

/**
 * Change voilume
 */
void Sound::volume(unsigned int vol) {
    if(vol <= 30) _sendCommand(0x06, 0x00, vol);
}

/**
 * Change equalizer
 */
void Sound::equalizer(unsigned int eq) {
    if(eq <= 5) _sendCommand(0x07, 0x00, eq);
}

/**
 * Play a track
 * @param folder number
 * @param track number
 */
void Sound::play(unsigned int folder, unsigned int track) {
    equalizer(config.sound_eq());
    vTaskDelay(100);
    volume(config.sound_vol());
    vTaskDelay(100);
    if(folder==1 and track<=25) {
        if(String(config.lang()) == "de") folder = 2;
        else if(String(config.lang()) == "ru") folder = 3;
        else if(String(config.lang()) == "pl") folder = 4;
        else if(String(config.lang()) == "ua") folder = 5;
        _sendCommand(0x0F, folder, track == 0 ? 24 : track);
    }
    if(folder==2 and track<=32) _sendCommand(0x0F, 6, track + 1);
    if(folder==3 and track<=12) _sendCommand(0x0F, 7, track);
}

/**
 * Stop playing
 */
void Sound::stopPlaying(void) {
    _sendCommand(0x16, 0x00, 0x00);
}

/** 
 * Check if it is time to play hourly signal and playing
 */
void Sound::hourlySignal() {
    if(minute() == 0 and _hourly_rang != hour()) {
        _hourly_rang = hour();
        switch(config.sound_hourly()) {
            case 0: _playHourlySignal(); break;
            case 2: if(weather.get_isDay()) _playHourlySignal(); break;
            case 3: if(hour() >= config.sound_hour_from() and hour() <= config.sound_hour_to()) _playHourlySignal(); break;
            default: ; break;
        }
    }
}

/** 
 * Playing an hourly sound and speaking the time every hour
 */
void Sound::_playHourlySignal() {
    if(_mp3_found && digitalRead(MP3_BUSY_PIN)) {
        play(1, 25);
        vTaskDelay(1800);
        play(1, hour());
    }
}

/** 
 * Check if it's time to ring the alarm
 */
void Sound::alarm() {
    if(_mp3_found) {
        for(int i=0; i<ALARMS; i++) {
            if(config.alarm_state(i) and config.alarm_weekday(i, weekday() == 1 ? 6 : weekday() - 2)) {
                if(config.alarm_time(i, 0) == hour() and config.alarm_time(i, 1) == minute() and _alarm_rang != minute()) { 
                    time_t mils = millis();
                    play(2, config.alarm_melodie(i));
                    vTaskDelay(200);
                    while(1) {
                        if(!digitalRead(MP3_BUSY_PIN)) break;
                        if(millis() - mils > 5000) {
                            play(2, config.alarm_melodie(i));
                            break;
                        }
                    }
                    _alarm_rang = minute();
                }
            }
        }
        if(minute() == 0 and second() >= 0 and second() <= 5) _alarm_rang = 60;

        // Stop alarm ringing if the alarm button was pressed
        if(global.alarm_but_pressed) {
            stopPlaying();
            global.alarm_but_pressed = false;
        }
    }
}

void Sound::_reset() {
    _sendCommand(0x0C, 0x00, 0x00);
    _sendCommand(0x09, 0x00, 0x02);
    vTaskDelay(200);
    equalizer(config.sound_eq());
    vTaskDelay(100);
    volume(config.sound_vol());
    vTaskDelay(100);
}

uint16_t Sound::_chckSum(uint8_t *sdata) {
    uint16_t sum = 0;
    for(int i=1; i<7; i++) {
        sum += sdata[i];
    }
    return -sum;
}

void Sound::_sendCommand(uint8_t command, uint8_t hByte, uint8_t lByte) {
    uint8_t sdata[10] = {0x7E, 0xFF, 0x06, command, 0x00, hByte, lByte, 0x00, 0x00, 0xEF};
    uint16_t sum = _chckSum(sdata);
    sdata[7] = (uint8_t)(sum >> 8);
    sdata[8] = (uint8_t)(sum);
    Serial3.write(sdata, 10);
    delay(10);
}