class TimeNTP {
  public:
    unsigned int get_time(void);
    boolean is_summertime(void);
};

/**
 * Time synchronization with NTP server
 */
unsigned int TimeNTP::get_time(void) {
  if(global.net_connected) {
    Serial.println(SEPARATOR);
    Serial.print("NTP synchronization... ");
    configTime(config.clock_utc() * 3600, 0, config.clock_ntp());
    struct tm tmstruct;
    vTaskDelay(1000);
    tmstruct.tm_year = 0;
    getLocalTime(&tmstruct, 5000);
    if(tmstruct.tm_year >= 121) {
      setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon + 1, tmstruct.tm_year + 1900);
      unsigned int summertime = config.clock_dlst() ? is_summertime() ? 3600 : 0 : 0;
      unsigned int t = now() + summertime;
      setTime(t);
      global.clockSynchronize = true;
      global.clockSynchronized = true;
      Serial.print("successfull: ");
      Serial.printf("%d:%02d:%02d %02d-%02d-%d\r\n", hour(), minute(), second(), day(), month(), year());
    }
    else {
      Serial.println("failed");
      global.clockSynchronized = false;
    }
  }
  else global.clockSynchronized = false;
}

/**
 * Check the time and date for daylight saving time
 * (works with an error, changes the time at 1:00)
 */
boolean TimeNTP::is_summertime(void) {
  if(month() < 3 || month() > 10) return false;
  if(month() > 3 && month() < 10) return true;
  if((month() == 3 && (hour() + 24 * day()) >= (1 + 24 * (31 - (5 * year() / 4 + 4) % 7))) || (month() == 10 && (hour() + 24 * day()) < (1 + 24 * (31 - (5 * year() / 4 + 1) % 7)))) return true;
  else return false;
}
