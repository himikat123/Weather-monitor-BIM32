# Weather-monitor-BIM32
## Weather monitor based on ESP32

<center>
  <img src="/img/20200928_221642.jpg">
</center> 

Краткий список возможностей монитора погоды:<br><br>

Подключение к домашней WiFi сети 2.4 гГц<br>
Отображение текущей погоды и прогноза погоды на 3 дня<br>
Отображение почасового прогноза погоды на 5 дней<br>
Отображение истории погоды (почасово) на 1 день назад<br>
Посторение графиков изменения температуры, влажности и вероятности осадков<br>
Отображение температуры и влажности дома<br>
Отображение фазы луны<br>
Часы (маленькие/большие/с секундами/стрелочные) с синхронизацией NTP<br>
Календарь с возможностью перелистывания на до плюс/минус бесконечности<br>
Отправка и/или прием данных по протоколу MQTT<br>
Отправка и/или прием данных с/на сервис thingspeak<br>
Отправка данных на народный мониторинг<br>
Беспроводной датчик температуры/давлени/влажности/освещенности<br>
Поддержка проводных датчиков температуры/давлени/влажности/освещенности<br>
Авторегулировка яркости подсветки экрана (по датчику освещенности, по времени или по факту рассвета и заката)<br>
Цветной сенсорный дисплей 3,5 дюйма<br><br>
 

Схема монитора погоды<br>

Благодаря применению модулей, схема прибора получилась простой как совковый утюг. Применен WiFi модуль Nodemcu 32S 30pin. 
Это сердце и душа прибора, вся основная логика работы монитора погоды выполняется этим модулем. Данный модуль представляет 
собой микросхему SoC ESP32 со всей необходимой обвзякой, флеш памятью, USB->UART конвертером и стабилизатором напряжения питания. <br>

nodemcu32s<br><br>

Модуль дисплея Nextion enhanced 3.5 дюймов. Представляет собой дисплей HMI TFT, со встроенным графическим процессором, 
с 16 Мб флэш-памяти, 1024 байт EEPROM и ОЗУ 3584 байт. Дисплей берет на себя задачи прорисовки графики, и обработки 
тапов по тачскрину, а так же сомостоятельно обеспечивает ход часов, календаря, анимации и прокрутки текста, что позволяет 
существенно разгрузить основной (ESP32) процессор. Связь дисплея с основным процессором осуществляется посредством интерфейса UART.<br>

display<br><br>

Модуль радиоканала HC-12 предназначен здесь для связи с беспроводным датчиком. Рабочая частота – 433,4 – 473,0 МГц. 
Дальность передачи информации – до 1 км на открытом пространстве. Количество каналов передачи данных – 100. 
В данный модуль встроен микроконтроллер, который самостоятельно решает все вопросы приема или передачи данных. 
Связь этого модуля с основным процессором осуществляется посредством интерфейса UART. <br>
Если применение беспроводного датчика не планируется, то можно этот модуль не устанавливать.<br>

HC-12<br><br>

Монитор погоды умеет считывать датчики температуры, давления, влажности и освещенности.<br>
Поддерживаются следующие датчики:<br>
BME280<br>
BMP180<br>
DHT22<br>
SHT21<br>
BH1750<br>
MAX44009<br>
Можно подключить один, ниодного, несколько или все сразу из этого списка.<br><br>

Печатную плату заказывал в Китае, так как возиться с лазерным утюгом не было никакого желания. Платы у меня плохо получаются.<br>

Плата монитора погоды<br><br>

Посмотрите видеообзор монитора погоды, это поможет не только ознакомиться с ним, но так же внесет ясность в методику настройки параметров прибора.<br>

Видеообзор монитора погоды

Для прошивки монитора погоды понадобится micro-sd карта, micro-USB кабель и компьютер.<br>
Чтоб прошить дисплей, нужно скопировать файл прошивки дисплея (файл с расширением .tft) на micro-sd карту 
(карта должна быть отформатирована в файловой системе FAT32). Затем вставить флешку в слот micro-sd дисплея 
и подать ему питание. Дисплей сам прошьется с флешки, процесс прошивки можно будет наблюдать на экране.<br>

Чтоб прошить ESP32 скачайте программу Flash Download Tools и настройте все как на скриншоте. То есть, в верхней 
строке укажите файл BIM32_ESP32.bin, а в строке ниже - BIM32_ESP32_SPIFFS.bin. COM-порт укажите тот, на 
котором реально сидит ваша ESP32. <br>

download tool<br><br>

Ну и конечно, на последок, парочка фотографий устройства в деле.<br>

1<br>
2<br>
3<br>
4<br>
5<br>
6<br><br>