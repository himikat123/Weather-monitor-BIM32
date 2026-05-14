#include "agregateLcdData.hpp"

uint8_t AgregateLcdData::windDirection(int deg) {
    uint8_t wind = 0;
    if((deg >= 338 && deg <= 360) || (deg >= 0 && deg < 22)) wind = 0; // north
    else if(deg >= 22 && deg < 67) wind = 1; // north-east
    else if(deg >= 67 && deg < 112) wind = 2; // east
    else if(deg >= 112 && deg < 157) wind = 3; // south-east
    else if(deg >= 157 && deg < 202) wind = 4; // south
    else if(deg >= 202 && deg < 247) wind = 5; // south-west
    else if(deg >= 247 && deg < 292) wind = 6; // west
    else if(deg >= 292 && deg < 338) wind = 7; // north-west
    return wind;
}