#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"

String TaskServer::_webGetContentType(String filename) {
  if(_server.hasArg("download"))       return "application/octet-stream";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".json")) return "text/json";
  else if(filename.endsWith(".jpg"))  return "image/jpeg";
  else if(filename.endsWith(".png"))  return "image/png";
  else if(filename.endsWith(".css"))  return "text/css";
  else if(filename.endsWith(".js"))   return "application/javascript";
  else if(filename.endsWith(".gz"))   return "application/x-gzip";
  return "text/plain";
}

/**
 * Read a file
 */
bool TaskServer::_webFileRead(String path) {
    String contentType = _webGetContentType(path);
    String pathWithGz = path + ".gz";

    if(LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
        if(LittleFS.exists(pathWithGz)) path += ".gz";
        File file = LittleFS.open(path, "r");
        if(path.endsWith(".js.gz") || path.endsWith(".css.gz")) {
            _server.sendHeader("Cache-Control", "max-age=31536000");
        }
        _server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

/**
 * Check if the user is logged in and sends the requested file, otherwise sends the login page
 */
bool TaskServer::_webGetFile(String path) {
    if(path.endsWith(".json") or path.endsWith(".jpg") or path.endsWith(".png")) {
        if(!_webIsLogged(false)) {
            if(path.endsWith(".json")) {
                _server.send(200, "application/json", "{\"lang\": \"" + config.lang() + "\", \"state\": \"LOGIN\"}");
                return true;
            }
        }
    }
    else {
        if(!path.endsWith(".css") && !path.endsWith(".js")) path = "/index.html";
    }
    return _webFileRead(path);
}