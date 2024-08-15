1. Добавляем в **Arduino IDE** поддержку **ESP32**, находим [инструкцию](https://www.google.ru/search?newwindow=1&sxsrf=ALeKk01SY0YVvecPGZL1p6_dmI2_zcbuzQ%3A1604686083566&ei=A5GlX-mXIoeckwX0lbiYCw&q=esp32+arduino+ide+%D1%83%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0&oq=arduino+%D1%83%D1%81%D1%82esp32&gs_lcp=CgZwc3ktYWIQAxgAMggIABAIEAcQHjoECAAQRzoHCCMQsAIQJzoECAAQDToECCMQJzoFCAAQywE6BggAEAcQHlDcpgFY89oBYPzmAWgAcAJ4AIABb4gBzwaSAQM2LjOYAQCgAQGqAQdnd3Mtd2l6yAEIwAEB&sclient=psy-ab)
2. Добавляем в **Ардуино** плагин для заливки **SPIFFS** находим [инструкцию](https://www.google.ru/search?newwindow=1&sxsrf=ALeKk01Btxvm4RWeH8qgpglopKEEPCEwiw%3A1604686179645&ei=Y5GlX_bkJsHUkwXEqpSICg&q=esp32+arduino+%D1%83%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0+sketch+upload&oq=esp32+arduino+%D1%83%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0+sketch+upload&gs_lcp=CgZwc3ktYWIQAzoECAAQRzoHCCMQsAIQJzoICAAQCBANEB46BAgjECc6BggAEAgQHlC7Slj5dGDdeWgGcAJ4AIABiAGIAa0LkgEDNy43mAEAoAEBqgEHZ3dzLXdpesgBCMABAQ&sclient=psy-ab&ved=0ahUKEwi2otrtwe7sAhVB6qQKHUQVBaEQ4dUDCA0&uact=5)
3. Распаковываем библиотеки из файла **libraries.zip** в папку `C:/Users/Username/Documents/Arduino/libraries`
4. Открываем файл `C:/Users/Username/AppData/Local/Arduino15/packages/esp32/hardware/esp32/1.0.5/platform.txt` и находим (поиском) строчку
`compiler.elf2hex.extra_flags=`, добавьте новую строку: `compiler.libraries.ldflags=`

<img src="img/bsec1.png" alt="BSEC flags">

Потом, в этом же файле, находим `{compiler.c.elf.libs}` и добавляем за ним, в ту же строку: `{compiler.libraries.ldflags}`

<img src="img/bsec2.png" alt="BSEC ldflags">

5. Прошиваем плату скетчем
6. Прошиваем файлы файловой системы **SPIFFS**, в Ардуино нужно выбрать **Инструмены --> ESP32 Sketch Data Upload**