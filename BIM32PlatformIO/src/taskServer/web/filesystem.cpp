#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include "../taskServer.hpp"
#include "../../state/state.hpp"

/**
 * Upload a file
 */
void TaskServer::_webFileUpload() {
    HTTPUpload& upload = _server.upload();
    if(upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/" + filename;
        fsUploadFile = LittleFS.open(filename, "w");
    }
    else if(upload.status == UPLOAD_FILE_WRITE) {
        if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
    }
    else if(upload.status == UPLOAD_FILE_END) {
        state.filesystem.fsInfoUpdate = true;
        if(fsUploadFile) fsUploadFile.close();
    }
}

/**
 * Delete a file
 */
void TaskServer::_webFileDelete() {
    if(_webIsLogged(true)) {
        String path = "/" + _server.arg("file");
        if(!LittleFS.exists(path)) return _server.send(404, "text/plain", "FileNotFound");
        else {
            _server.send(200, "text/plain", LittleFS.remove(path) ? "OK" : "ERROR");
            state.filesystem.fsInfoUpdate = true;
        }
    }
}

/**
 * Rename a file
 */
void TaskServer::_webFileRename() {
    if(_webIsLogged(true)) {
        String alt = "/" + _server.arg("old");
        String neu = "/" + _server.arg("new");
        if(!LittleFS.exists(alt)) return _server.send(404, "text/plain", "FileNotFound");
        else {
            _server.send(200, "text/plain", LittleFS.rename(alt, neu) ? "OK" : "ERROR");
            state.filesystem.fsInfoUpdate = true;
        }
    }
}