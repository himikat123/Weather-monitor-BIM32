#include "DFRobotDFPlayerMini.h" // v1.0.6 https://github.com/DFRobot/DFRobotDFPlayerMini
DFRobotDFPlayerMini mp3player;

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
    unsigned int _alarm_rang = 60;
    unsigned int _hourly_rang = 0;
};

/**
 * DF player initialization
 */
void Sound::init(void) {
  Serial.println(SEPARATOR);
  Serial.println("Initialize DF Player...");
  mp3player.begin(Serial3);
  if(mp3player.begin(Serial3)){
    Serial.println("DFPlayer found");
    mp3player.setTimeOut(500);
    mp3player.volume(config.sound_vol());
    mp3player.EQ(config.sound_eq());
    mp3player.outputDevice(DFPLAYER_DEVICE_SD);
  }
  else Serial.println("DFPlayer NOT found");
}

/**
 * Change voilume
 */
void Sound::volume(unsigned int vol) {
  if(vol<=30) mp3player.volume(vol);
}

/**
 * Change equalizer
 */
void Sound::equalizer(unsigned int eq) {
  if(eq<=5) mp3player.EQ(eq);
}

/**
 * Play a track
 * @param folder number
 * @param track number
 */
void Sound::play(unsigned int folder, unsigned int track) {
  mp3player.EQ(config.sound_eq());
  vTaskDelay(100);
  mp3player.volume(config.sound_vol());
  vTaskDelay(100);
  if(folder==1 and track<=25) mp3player.playFolder(folder, track == 0 ? 24 : track);
  if(folder==2 and track<=20) mp3player.playFolder(folder, track == 0 ? 1 : track);
}

/**
 * Stop playing
 */
void Sound::stopPlaying(void) {
  mp3player.stop();
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
  play(1, 25);
  vTaskDelay(2000);
  play(1, hour());
  vTaskDelay(2000);
}

/** 
 * Check if it's time to ring the alarm
 */
void Sound::alarm() {
  for(int i=0; i<ALARMS; i++) {
    if(config.alarm_state(i) and config.alarm_weekday(i, weekday() == 1 ? 6 : weekday() - 2)) {
      if(config.alarm_time(i, 0) == hour() and config.alarm_time(i, 1) == minute() and _alarm_rang != minute()) { 
        play(2, config.alarm_melodie(i));
        _alarm_rang = minute();
        vTaskDelay(5000);
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
