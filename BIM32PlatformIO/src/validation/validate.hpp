#pragma once

class Validate {
    public:
        static Validate& getInstance() {
            static Validate instance;
            return instance;
        }

        Validate(const Validate&) = delete;
        void operator=(const Validate&) = delete;

        bool temp(float t);
        bool hum(float h);
        bool pres(float p);
        bool windSpeed(float w);
        bool windDir(int w);
        bool light(float l);
        bool volt(float v);
        bool iaq(float i);
        bool batVolt(float v);
        bool batLvl(int l);
        bool batPercent(int p);
        bool thingVolt(float v);
        bool hVolt(float v);
        bool current(float c);
        bool power(float p);
        bool energy(float e);
        bool frequence(float f);
        bool co2(float c);
        bool absoluteHum(float ah);
        bool dewPoint(float dp, float temp);

    private:
        Validate() {}
};

inline Validate& validate = Validate::getInstance();