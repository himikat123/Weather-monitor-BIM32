# CRCx

CRC calculation for Arduino and other C++ programs.
This library is created to use crc calculation as same method for both Arduino and other C++ propgrams (like openFrameworks and so on).
CRCx is based on fast and efficient two great works, and CRCx is just a glue for these two great works.

- [FastCRC](https://github.com/FrankBoesing/FastCRC) by @FrankBoesing for Arduino
- [CRCpp](https://github.com/d-bahr/CRCpp) v1.1.0.0 by @d-bahr for other C++ programs


## Usage

### One-Time calculation

``` C++
#include <CRCx.h>

const uint8_t data[] = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
const size_t size = sizeof(data);

void setup() {
    Serial.begin(115200);
    delay(2000);

    uint8_t result8 = crcx::crc8(data, size);
    uint16_t result16 = crcx::crc16(data, size);
    uint32_t result32 = crcx::crc32(data, size);

    Serial.print("crc8  = 0x"); Serial.println(result8, HEX);
    Serial.print("crc16 = 0x"); Serial.println(result16, HEX);
    Serial.print("crc32 = 0x"); Serial.println(result32, HEX);
}

// Console Output:
// crc8  = 0x7
// crc16 = 0xB944
// crc32 = 0x87E5865B

```

### Continuous calculation

```C++
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

    Serial.print("crc8  = 0x"); Serial.println(result8, HEX);
    Serial.print("crc16 = 0x"); Serial.println(result16, HEX);
    Serial.print("crc32 = 0x"); Serial.println(result32, HEX);
}

// Output:
// crc8  = 0x7
// crc16 = 0xB944
// crc32 = 0x87E5865B
```

## APIs

```C++
inline uint8_t crc8(const uint8_t* data, const size_t size, const Crc8 type = Crc8::SMBUS);
inline uint16_t crc16(const uint8_t* data, const size_t size, const Crc16 type = Crc16::MODBUS);
inline uint32_t crc32(const uint8_t* data, const size_t size, const Crc32 type = Crc32::CRC32);
inline uint8_t crc8_update(const uint8_t* data, const size_t size, const Crc8 type = Crc8::SMBUS);
inline uint16_t crc16_update(const uint8_t* data, const size_t size, const Crc16 type = Crc16::MODBUS);
inline uint32_t crc32_update(const uint8_t* data, const size_t size, const Crc32 type = Crc32::CRC32);

// C++ (not Arduino) Only
inline uint64_t crc64(const uint8_t* data, const size_t size);
inline uint64_t crc64_update(const uint8_t* data, const size_t size);
```

## CRC Options

You can specify how to calculate by setting 3rd argument.

``` C++
uint32_t result32 = crcx::crc32(data, size, Crc32::POSIX);
```

Available options and default parameters are shown below.

``` C++
enum class Crc8 : uint8_t {
    SMBUS, // default
    MAXIM
};

enum class Crc16 : uint8_t {
    CCITT,
    KERMIT,
    MODBUS, // default
    XMODEM,
    X25
};

enum class Crc32 : uint8_t {
    CRC32, // default
    POSIX
};
```

## Use `FastCRC` or `CRCpp` directly

Of course, you can use `FastCRC` or `CRCpp` directly.

### FastCRC

``` C++
FastCRC32 fastcrc;
uint32_t crc32 = fastcrc.crc32(data, size);
```

Please refer to [FastCRC original page](https://github.com/FrankBoesing/FastCRC) for more information.

### CRCpp

NOTE: Following `CRCpp` options are enabled as default.

```C++
#define CRCPP_USE_CPP11
#define CRCPP_USE_NAMESPACE
#define CRCPP_INCLUDE_ESOTERIC_CRC_DEFINITIONS
```

and you can use as:

``` C++
uint32_t crc32 = CRCpp::CRC::Calculate(data, size, CRCpp::CRC::CRC_32());
```

Please refer to [CRCpp original page](https://github.com/d-bahr/CRCpp) for more information.

## License

MIT
