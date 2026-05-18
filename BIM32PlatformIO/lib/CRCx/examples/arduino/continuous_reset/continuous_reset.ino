#include <CRCx.h>

const uint8_t data[11] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
const size_t size = sizeof(data);

void setup() {
    Serial.begin(115200);
    delay(2000);

    // first
    uint8_t result8 = crcx::crc8_update(data, 5);
    uint16_t result16 = crcx::crc16_update(data, 5);
    uint32_t result32 = crcx::crc32_update(data, 5);

    // second
    result8 = crcx::crc8_update(data + 5, 3);
    result16 = crcx::crc16_update(data + 5, 3);
    result32 = crcx::crc32_update(data + 5, 3);

    // third
    result8 = crcx::crc8_update(data + 8, 3);
    result16 = crcx::crc16_update(data + 8, 3);
    result32 = crcx::crc32_update(data + 8, 3);

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
