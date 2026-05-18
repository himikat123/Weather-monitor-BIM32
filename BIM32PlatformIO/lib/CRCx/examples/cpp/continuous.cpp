#include <iostream>
#include "../../src/CRCx.h"

const uint8_t data[11] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
const size_t size = sizeof(data);

int main() {
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

    std::cout << "crc8  = 0x" << std::hex << (int)result8 << std::endl;
    std::cout << "crc16 = 0x" << std::hex << result16 << std::endl;
    std::cout << "crc32 = 0x" << std::hex << result32 << std::endl;

    return 0;
}

// Output:
// crc8  = 0x7
// crc16 = 0xB944
// crc32 = 0x87E5865B
