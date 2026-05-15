#pragma once
#include <Arduino.h>

class Config;

struct Account {
    private:
        char _name[32] = ""; // Web interface username
        char _pass[64] = ""; // Web interface password
        bool _required = false; // Require username and password to access the web interface
        friend class Config;

    public:
        const char* name() const { return _name; }
        const char* pass() const { return _pass; }
        bool required() const { return _required; }
};