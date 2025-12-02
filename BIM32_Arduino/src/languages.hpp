/* English, German, Russian, Polish, Ukrainian, Bulgarian, Spanish */
static const char* monthFullNameTxt[12][7] PROGMEM = {
    {"January", "Januar", "Январь", "Styczeń", "Січень", "Януари", "Enero"},
    {"February", "Februar", "Февраль", "Luty", "Лютий", "Февруари", "Febrero"},
    {"March", "März", "Март", "Marsz", "Березень", "Март", "Marzo"},
    {"April", "April", "Апрель", "Kwiecień", "Квітень", "Април", "Abril"},
    {"May", "Mai", "Май", "Maj", "Травень", "Май", "Mayo"},
    {"June", "Juni", "Июнь", "Czerwiec", "Червень", "Юни", "Junio"},
    {"July", "Juli", "Июль", "Lipiec", "Липень", "Юли", "Julio"},
    {"August", "August", "Август", "Sierpień", "Серпень", "Август", "Agosto"},
    {"September", "September", "Сентябрь", "Wrzesień", "Вересень", "Септември", "Septiembre"},
    {"October", "Oktober", "Октябрь", "Październik", "Жовтень", "Октомври", "Octubre"},
    {"November", "November", "Ноябрь", "Listopad", "Листопад", "Ноември", "Noviembre"},
    {"December", "Dezember", "Декабрь", "Grudzień", "Грудень", "Декември", "Diciembre"}
};

static const char* monthDayTxt[12][7] PROGMEM = {
    {"January", "Januar", "января", "stycznia", "січня", "януари", "enero"},
    {"February", "Februar", "февраля", "lutego", "лютого", "февруари", "febrero"},
    {"March", "März", "марта", "marca", "березня", "март", "marzo"},
    {"April", "April", "апреля", "kwietnia", "квітня", "март", "abril"},
    {"May", "Mai", "мая", "maja", "травня", "май", "mayo"},
    {"June", "Juni", "июня", "czerwca", "червня", "юни", "junio"},
    {"July", "Juli", "июля", "lipca", "липня", "юли", "julio"},
    {"August", "August", "августа", "sierpnia", "серпня", "август", "agosto"},
    {"September", "September", "сентября", "września", "вересня", "септември", "septiembre"},
    {"October", "Oktober", "октября", "października", "жовтня", "октомври", "octubre"},
    {"November", "November", "ноября", "listopada", "листопада", "ноември", "noviembre"},
    {"December", "Dezember", "декабря", "grudnia", "грудня", "декември", "diciembre"}
};

static const char* monthShortNameTxt[12][7] PROGMEM = {
    {"Jan", "Jan", "янв", "sty", "січ", "яну", "ene"},
    {"Feb", "Feb", "фев", "lut", "лют", "фев", "feb"},
    {"Mar", "Mär", "мар", "mar", "бер", "мар", "mar"},
    {"Apr", "Apr", "апр", "kwi", "кві", "апр", "abr"},
    {"May", "Mai", "мая", "maj", "тра", "май", "may"},
    {"Jun", "Jun", "июн", "cze", "чер", "юни", "jun"},
    {"Jul", "Jul", "июл", "lip", "лип", "юли", "jul"},
    {"Aug", "Aug", "авг", "sie", "сер", "авг", "ago"},
    {"Sep", "Sep", "сен", "wrz", "вер", "сеп", "sep"},
    {"Oct", "Okt", "окт", "paź", "жов", "окт", "oct"},
    {"Nov", "Nov", "ноя", "lis", "лис", "ное", "nov"},
    {"Dec", "Dez", "дек", "gru", "гру", "дек", "dic"}
};

static const char* weekdayFullNameTxt[7][7] PROGMEM = {
    {"Sunday", "Sonntag", "Воскресенье", "Niedziela", "Неділя", "Неделя", "Domingo"},
    {"Monday", "Montag", "Понедельник", "Poniedziałek", "Понеділок", "Понеделник", "Lunes"},
    {"Tuesday", "Dienstag", "Вторник", "Wtorek", "Вівторок", "Вторник", "Martes"},
    {"Wednesday", "Mittwoch", "Среда", "Środa", "Середа", "Сряда", "Miércoles"},
    {"Thursday", "Donnerstag", "Четверг", "Czwartek", "Четвер", "Четвъртък", "Jueves"},
    {"Friday", "Freitag", "Пятница", "Piątek", "П'ятниця", "Петък", "Viernes"},
    {"Saturday", "Samstag", "Суббота", "Sobota", "Субота", "Събота", "Sábado"}
};

static const char* weekdayShortNameTxt[7][7] PROGMEM = {
    {"Su", "So", "Вс", "Nd", "Нд", "Не", "Do"},
    {"Mo", "Mo", "Пн", "Pn", "Пн", "По", "Lu"},
    {"Tu", "Di", "Вт", "Wt", "Вт", "Вт", "Ma"},
    {"We", "Mi", "Ср", "Śr", "Ср", "Ср", "Mi"},
    {"Th", "Do", "Чт", "Cz", "Чт", "Че", "Ju"},
    {"Fr", "Fr", "Пт", "Pi", "Пт", "Пе", "Vi"},
    {"Sa", "Sa", "Сб", "So", "Сб", "Съ", "Sá"}
};

static const char* weatherDescriptionTxt[16][7] PROGMEM = {
    {"Clear sky", "Klarer Himmel", "Ясно", "Czyste Niebo", "Ясно", "Ясно", "Despejado"},
    {"Mainly clear", "Überwiegend klar", "Преимущественно ясно", "Głównie jasne", "Переважно ясно", "Предимно ясно", "Mayormente despejado"},
    {"Partly cloudy", "Teilweise bewölkt", "Переменная облачность", "Częściowe zachmurzenie", "Мінлива хмарність", "Предимно облачно", "Parcialmente nublado"},
    {"Overcast", "Bedeckt", "Пасмурно", "Pochmurny", "Хмарно", "Облачно", "Nublado"},
    {"Fog", "Nebel", "Туман", "Mgła", "Туман", "Мъгла", "Neblina"},
    {"Depositing rime fog", "Ablagerung von Raureifnebel", "Туман с инеем", "Deponująca się mgła szronowa", "Туман з інеєм", "Мъгла със скреж", "Escarcha"},
    {"Drizzle", "Nieselregen", "Морось", "Mżawka", "Мряка", "Дъжд", "Llovizna"},
    {"Freezing drizzle", "Gefrierender Nieselregen", "Изморозь", "Marznąca mżawka", "Ізморозь", "Скреж", "Llovizna helada"},
    {"Rain", "Regen", "Дождь", "Deszcz", "Дощ", "Дъжд", "Lluvia"},
    {"Freezing rain", "Gefrierender Regen", "Ледяной дождь", "Marznący deszcz", "Крижаний дощ", "Леден дъжд", "Lluvia helada"},
    {"Snowfall", "Schneefall", "Снегопад", "Opady śniegu", "Снігопад", "Снеговалеж", "Nevada"},
    {"Snow grains", "Schneekörner", "Снежные зерна", "Ziarna śniegu", "Снігові зерна", "Скреж", "Copos de nieve"},
    {"Rain showers", "Regenschauer", "Ливень", "Przelotne opady deszczu", "Злива", "Ръмеж", "Chubascos"},
    {"Snow showers", "Schneeschauer", "Сильный снегопад", "Opady śniegu", "Сильний снігопад", "Обилен снеговалеж", "Tormenta de nieve"},
    {"Thunderstorm", "Gewitter", "Гроза", "Burza z piorunami", "Гроза", "Буря", "Tormenta eléctrica"},
    {"Thunderstorm with hail", "Gewitter mit Hagel", "Гроза с градом", "Burza z gradem", "Гроза з градом", "Буря с градушка", "Tormenta eléctrica con granizo"}
};

static const char* comfortTxt[9][7] PROGMEM = {
    {"Comfortable", "Gemütlich", "Комфортно", "Przyjemny", "Комфортно", "Комфортно", "Clima agradable"},
    {"Too hot", "Zu heiß", "Жарко", "Gorąco", "Жарко", "Горещо", "Muy caliente"},
    {"Too cold", "Zu kalt", "Холодно", "Zimno", "Холодно", "Студено", "Muy frío"},
    {"Too humid", "Zu feucht", "Слишком влажно", "Zbyt wilgotno", "Занадто волого", "Твърде влажно", "Muy húmedo"},
    {"Too dry", "Zu trocken", "Слишком сухо", "Zbyt sucho", "Занадто сухо", "Твърде сухо", "Muy seco"},
    {"Hot and humid", "Heiß und feucht", "Жарко и влажно", "Gorąco i wilgotno", "Жарко та волого", "Горещо и влажно", "Caliente y húmedo"},
    {"Hot and dry", "Heiß und trocken", "Жарко и сухо", "Gorąco i sucho", "Жарко та сухо", "Горещо и сухо", "Caliente y seco"},
    {"Cold and humid", "Kalt und feucht", "Холодно и влажно", "Zimno i wilgotno", "Холодно та волого", "Студено и влажно", "Frío y húmedo"},
    {"Cold and dry", "Kalt und trocken", "Холодно и сухо", "Zimno i sucho", "Холодно та сухо", "Студено и сухо", "Frío y seco"}
};

static const char* airQualityTxt[3][7] PROGMEM = {
    {"Clean Air", "Saubere Luft", "Воздух чистый", "Powietrze czyste", "Повітря чисте", "Въздухът е чист", "Aire limpio"},
    {"Air is polluted", "Luft ist verschmutzt", "Воздух загрязнен", "Powietrze zanieczyszczone", "Повітря забруднене", "Въздухът е замърсен", "Aire contaminado"},
    {"Air is heavily polluted", "Luft ist stark verschmutzt", "Воздух сильно загрязнен", "Powietrze silnie zanieczyszczone", "Повітря сильно забруднене", "Въздухът е силно замърсен", "Aire muy contaminado"}
};

static const char* alarmTxt[7] PROGMEM = {"Alarm", "Wecker", "Будильник", "Budzik", "Будильник", "Будилник", "Alarma"};
static const char* networkTxt[7] PROGMEM = {"Network", "Netzwerk", "Сеть", "Sieć", "Мережа", "Мрежа", "Red"};
static const char* signalLevelTxt[7] PROGMEM = {"Signal level", "Signalpegel", "Уровень сигнала", "Siła sygnału", "Рівень сигналу", "Ниво на сигнала", "Nivel de señal"};
static const char* ipAddrTxt[7] PROGMEM = {"IP address", "IP Adresse", "IP адрес", "Adres IP", "IP адреса", "IP адрес", "Dirección IP"};
static const char* macAddrTxt[7] PROGMEM = {"MAC address", "MAC Adresse", "MAC адрес", "MAC adres", "MAC адреса", "MAC адрес", "Dirección MAC"};
static const char* esp32TempTxt[7] PROGMEM = {"ESP32 Temperature", "ESP32 Temperatur", "Температура ESP32", "Temperatura ESP32", "Температура ESP32", "Температура ESP32", "Temperatura ESP32"};
static const char* firmwareTxt[7] PROGMEM = {"Firmware", "Firmware", "Прошивка", "Oprogramowanie", "Прошивка", "Фърмуер", "Firmware"};
static const char* mmTxt[7] PROGMEM = {"mm", "mm", "мм", "mm", "мм", "мм", "mm"};
static const char* hpaTxt[7] PROGMEM = {"hPa", "hPa", "гПа", "hPa", "гПа", "гПа", "hPa"};
static const char* msTxt[7] PROGMEM = {"m/s", "m/s", "м/с", "m/s", "м/с", "м/с", "m/s"};
static const char* gpmTxt[7] PROGMEM = {"g/m³", "g/m³", "г/м³", "g/m³", "г/м³", "г/м³", "g/m³"};
static const char* vTxt[7] PROGMEM = {"V", "V", " В", "V", " В", " В", "V"};
static const char* historyInTxt[7] PROGMEM = {"Weather indoor history", "Hauswettergeschichte", "История погоды в доме", "Historia pomiarów w pomieszczeniach", "Історія погоди у будинку", "История на времето в дома", "Historial clima interior"};
static const char* historyOutTxt[7] PROGMEM = {"Weather outdoor history", "Außenwettergeschichte", "История погоды на улице", "Historia pogody na zewnątrz", "Історія погоди на вулиці", "История на времето навън", "Historial clima exterior"};
static const char* touchCalibrateTxt[7] PROGMEM = {"Touch corners as indicated", "Berühren Sie die Ecken wie angegeben", "Прикоснитесь к углам, как указано", "Dotknij narożników zgodnie ze wskazaniem", "Торкніться кутів, як зазначено", "Докоснете ъглите, както е показано", "Toca esquinas según se indica"};
static const char* calibrationDoneTxt[7] PROGMEM = {"Calibration complete", "Die Kalibrierung ist abgeschlossen", "Калибровка завершена", "Kalibracja została zakończona", "Калібрування закінчено", "Калибрирането е завършено", "Calibración terminada"};
static const char* unknownTxt PROGMEM = "???";


class Lang {
    private:
        uint8_t _lang() {
            if(config.lang() == "de") return 1;
            if(config.lang() == "ru") return 2;
            if(config.lang() == "pl") return 3;
            if(config.lang() == "ua") return 4;
            if(config.lang() == "bg") return 5;
            if(config.lang() == "es") return 6;
            return 0;
        }

    public:
        const char* monthFullName(uint8_t num) {
            if(num < 1 || num > 12) return unknownTxt;
            return monthFullNameTxt[num - 1][_lang()];
        }

        const char* monthDay(uint8_t num) {
            if(num < 1 || num > 12) return unknownTxt;
            return monthDayTxt[num - 1][_lang()];
        }

        const char* monthShortName(uint8_t num) {
            if(num < 1 || num > 12) return unknownTxt;
            return monthShortNameTxt[num - 1][_lang()];
        }

        const char* weekdayShortName(uint8_t num) {
            if(num < 1 || num > 7) return unknownTxt;
            return weekdayShortNameTxt[num - 1][_lang()];
        }

        const char* weekdayFullName(uint8_t num) {
            if(num < 1 || num > 7) return unknownTxt;
            return weekdayFullNameTxt[num - 1][_lang()];
        }

        const char* alarm() {
            return alarmTxt[_lang()];
        }

        const char* network() {
            return networkTxt[_lang()];
        }

        const char* signalLevel() {
            return signalLevelTxt[_lang()];
        }

        const char* ipAddr() {
            return ipAddrTxt[_lang()];
        }

        const char* macAddr() {
            return macAddrTxt[_lang()];
        }

        const char* esp32Temp() {
            return esp32TempTxt[_lang()];
        }

        const char* firmware() {
            return firmwareTxt[_lang()];
        }

        const char* mm() {
            return mmTxt[_lang()];
        }

        const char* hpa() {
            return hpaTxt[_lang()];
        }

        const char* ms() {
            return msTxt[_lang()];
        }

        const char* gpm() {
            return gpmTxt[_lang()];
        }

        const char* v() {
            return vTxt[_lang()];
        }

        const char* historyIn() {
            return historyInTxt[_lang()];
        }

        const char* historyOut() {
            return historyOutTxt[_lang()];
        }

        const char* comfort(unsigned int level) {
            if(level < 1 || level > 9) return unknownTxt;
            return comfortTxt[level - 1][_lang()];
        }

        const char* airQuality(unsigned int level) {
            if(level < 1 || level > 3) return unknownTxt;
            return airQualityTxt[level - 1][_lang()];
        }

        const char* weatherDescription(unsigned int level) {
            if(level > 15) return unknownTxt;
            return weatherDescriptionTxt[level][_lang()];
        }

        const char* touchCalibrate() {
            return touchCalibrateTxt[_lang()];
        }

        const char* calibrationDone() {
            return calibrationDoneTxt[_lang()];
        }
};
