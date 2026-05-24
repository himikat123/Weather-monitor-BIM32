#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Check if the user is logged in
 */
bool TaskServer::_webIsLogged(bool answer) {
    bool logged = false;
    if(!config.account.required()) logged = true;
    else {
        if(_server.hasArg("code")) {
            String auth = _server.arg("code");
            if(auth == String(state.secretCode)) logged = true;
            else logged = false;
        }
        else logged = false;
    }

    if(!logged && answer) _server.send(200, "text/plain", "NOT LOGGED IN");
    return logged;
}

/**
 * Login
 * Username and password validation 
 */
void TaskServer::_webLogin() {
    String user = "", pass = "";
    if(_server.hasArg("name")) user = _server.arg("name");
    if(_server.hasArg("pass")) pass = _server.arg("pass");
    bool loged = false;
    if(
        user == String(config.account.name()) and 
        pass == String(config.account.pass())
    ) {
        loged = true;
        state.secretCode = esp_random();
    }
    _server.send(200, "text/plain", loged ? ("OK:" + String(round(state.secretCode))) : "error:1");
}