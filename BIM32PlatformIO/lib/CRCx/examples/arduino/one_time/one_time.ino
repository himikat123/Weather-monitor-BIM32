#include <CRCx.h>

const uint8_t data[] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
const size_t size = sizeof(data);

void setup() {
    Serial.begin(115200);
    delay(2000);

    uint8_t result8 = crcx::crc8(data, size);
    uint16_t result16 = crcx::crc16(data, size);
    uint32_t result32 = crcx::crc32(data, size);

    Serial.print("crc8  = 0x");
    Serial.println(result8, HEX);
    Serial.print("crc16 = 0x");
    Serial.println(result16, HEX);
    Serial.print("crc32 = 0x");
    Serial.println(result32, HEX);
}

void loop() {
}

// Output:
// crc8  = 0x7
// crc16 = 0xB944
// crc32 = 0x87E5865B
