#include "FS.h"
#include <LittleFS.h>
#include "./taskServer.hpp"
#include "../state/state.hpp"

void TaskServer::_fsInfoUpdate() {
    if(state.filesystem.fsInfoUpdate) {
        state.filesystem.list[0] = '\0';
        _listAllFilesInDir("/");
        state.filesystem.total = LittleFS.totalBytes();
        state.filesystem.free = state.filesystem.total - LittleFS.usedBytes();
        state.filesystem.fsInfoUpdate = false;
        state.filesystem.updated = true;
    }
}