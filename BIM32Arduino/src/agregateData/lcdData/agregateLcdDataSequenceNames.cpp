#include "agregateLcdData.hpp"

#include "../../config/config.hpp"

void AgregateLcdData::sequenceNames(String* nameSequence) {
    for(uint8_t i=0; i<4; i++) {
        nameSequence[i] = config.display.source.sequence.name(i);
    }
}