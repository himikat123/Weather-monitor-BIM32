#include "./nextion.hpp"

void Nextion::showLogo() {
    _nextion.writeNum("sleep", 0);
    _nextion.writeNum("dim", 100);
    _nextion.writeStr("page Logo");
}