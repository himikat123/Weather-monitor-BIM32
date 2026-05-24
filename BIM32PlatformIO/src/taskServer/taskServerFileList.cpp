#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include "./taskServer.hpp"
#include "../state/state.hpp"

void TaskServer::_appendToFsList(const char* text) {
    strncat(state.filesystem.list, text, sizeof(state.filesystem.list) - strlen(state.filesystem.list) - 1);
}

void TaskServer::_listAllFilesInDir(const char* dirname) {
    File root = LittleFS.open(dirname);
    if(!root) return;
    if(!root.isDirectory()) return;

    File file = root.openNextFile();
    while(file) {
        if(file.isDirectory()) {
            char sub[128];
            snprintf(sub, sizeof(sub), "%s/", file.path());
            _listAllFilesInDir(sub);
        } 
        else {
            const char* filename = file.name();
            if(strcmp(filename, "user.us") != 0) {
                _appendToFsList(dirname);
                _appendToFsList(filename);
                _appendToFsList(":");
                char sizeBuf[16];
                snprintf(sizeBuf, sizeof(sizeBuf), "%u", file.size());
                _appendToFsList(sizeBuf);
                _appendToFsList(",");
            }
        }
        file = root.openNextFile();
    }
}