#include "./nextion.hpp"

void Nextion::showLogo() {
    _nextion.writeNum("sleep", 0);
    _nextion.writeNum("dim", 1);
    _nextion.writeStr("page Logo");
}