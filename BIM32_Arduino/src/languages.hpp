class Lang {
    private:
        /* English, German, Russian, Polish, Ukrainian, Bulgarian */
        String _monthFullName[12][6] = {
            {"January", "Januar", "Январь", "Styczeń", "Січень", "Януари"},
            {"February", "Februar", "Февраль", "Luty", "Лютий", "Февруари"},
            {"March", "März", "Март", "Marsz", "Березень", "Март"},
            {"April", "April", "Апрель", "Kwiecień", "Квітень", "Април"},
            {"May", "Mai", "Май", "Maj", "Травень", "Май"},
            {"June", "Juni", "Июнь", "Czerwiec", "Червень", "Юни"},
            {"July", "Juli", "Июль", "Lipiec", "Липень", "Юли"},
            {"August", "August", "Август", "Sierpień", "Серпень", "Август"},
            {"September", "September", "Сентябрь", "Wrzesień", "Вересень", "Септември"},
            {"October", "Oktober", "Октябрь", "Październik", "Жовтень", "Октомври"},
            {"November", "November", "Ноябрь", "Listopad", "Листопад", "Ноември"},
            {"December", "Dezember", "Декабрь", "Grudzień", "Грудень", "Декември"}
        };

        String _monthDay[12][6] {
            {"January", "Januar", "января", "stycznia", "січня", "януари"},
            {"February", "Februar", "февраля", "lutego", "лютого", "февруари"},
            {"March", "März", "марта", "marca", "березня", "март"},
            {"April", "April", "апреля", "kwietnia", "квітня", "март"},
            {"May", "Mai", "мая", "maja", "травня", "май"},
            {"June", "Juni", "июня", "czerwca", "червня", "юни"},
            {"July", "Juli", "июля", "lipca", "липня", "юли"},
            {"August", "August", "августа", "sierpnia", "серпня", "август"},
            {"September", "September", "сентября", "września", "вересня", "септември"},
            {"October", "Oktober", "октября", "października", "жовтня", "октомври"},
            {"November", "November", "ноября", "listopada", "листопада", "ноември"},
            {"December", "Dezember", "декабря", "grudnia", "грудня", "декември"}
        };

        String _monthShortName[12][6] = {
            {"Jan", "Jan", "янв", "sty", "січ", "яну"},
            {"Feb", "Feb", "фев", "lut", "лют", "фев"},
            {"Mar", "Mär", "мар", "mar", "бер", "мар"},
            {"Apr", "Apr", "апр", "kwi", "кві", "апр"},
            {"May", "Mai", "мая", "maj", "тра", "май"},
            {"Jun", "Jun", "июн", "cze", "чер", "юни"},
            {"Jul", "Jul", "июл", "lip", "лип", "юли"},
            {"Aug", "Aug", "авг", "sie", "сер", "авг"},
            {"Sep", "Sep", "сен", "wrz", "вер", "сеп"},
            {"Oct", "Okt", "окт", "paź", "жов", "окт"},
            {"Nov", "Nov", "ноя", "lis", "лис", "ное"},
            {"Dec", "Dez", "дек", "gru", "гру", "дек"}
        };

        String _weekdayFullName[7][6] = {
            {"Sunday", "Sonntag", "Воскресенье", "Niedziela", "Неділя", "Неделя"},
            {"Monday", "Montag", "Понедельник", "Poniedziałek", "Понеділок", "Понеделник"},
            {"Tuesday", "Dienstag", "Вторник", "Wtorek", "Вівторок", "Вторник"},
            {"Wednesday", "Mittwoch", "Среда", "Środa", "Середа", "Сряда"},
            {"Thursday", "Donnerstag", "Четверг", "Czwartek", "Четвер", "Четвъртък"},
            {"Friday", "Freitag", "Пятница", "Piątek", "П'ятниця", "Петък"},
            {"Saturday", "Samstag", "Суббота", "Sobota", "Субота", "Събота"}
        };

        String _weekdayShortName[7][6] = {
            {"Su", "So", "Вс", "Nd", "Нд", "Не"},
            {"Mo", "Mo", "Пн", "Pn", "Пн", "По"},
            {"Tu", "Di", "Вт", "Wt", "Вт", "Вт"},
            {"We", "Mi", "Ср", "Śr", "Ср", "Ср"},
            {"Th", "Do", "Чт", "Cz", "Чт", "Че"},
            {"Fr", "Fr", "Пт", "Pi", "Пт", "Пе"},
            {"Sa", "Sa", "Сб", "So", "Сб", "Съ"}
        };

        String _comfort[9][6] = {
            {"Comfortable", "Gemütlich", "Комфортно", "Przyjemny", "Комфортно", "Комфортно"},
            {"Too hot", "Zu heiß", "Жарко", "Gorąco", "Жарко", "Горещо"},
            {"Too cold", "Zu kalt", "Холодно", "Zimno", "Холодно", "Студено"},
            {"Too humid", "Zu feucht", "Слишком влажно", "Zbyt wilgotno", "Занадто волого", "Твърде влажно"},
            {"Too dry", "Zu trocken", "Слишком сухо", "Zbyt sucho", "Занадто сухо", "Твърде сухо"},
            {"Hot and humid", "Heiß und feucht", "Жарко и влажно", "Gorąco i wilgotno", "Жарко та волого", "Горещо и влажно"},
            {"Hot and dry", "Heiß und trocken", "Жарко и сухо", "Gorąco i sucho", "Жарко та сухо", "Горещо и сухо"},
            {"Cold and humid", "Kalt und feucht", "Холодно и влажно", "Zimno i wilgotno", "Холодно та волого", "Студено и влажно"},
            {"Cold and dry", "Kalt und trocken", "Холодно и сухо", "Zimno i sucho", "Холодно та сухо", "Студено и сухо"}
        };

        String _airQuality[3][6] = {
            {"Clean Air", "Saubere Luft", "Воздух чистый", "Powietrze czyste", "Повітря чисте", "Въздухът е чист"},
            {"Air is polluted", "Luft ist verschmutzt", "Воздух загрязнен", "Powietrze zanieczyszczone", "Повітря забруднене", "Въздухът е замърсен"},
            {"Air is heavily polluted", "Luft ist stark verschmutzt", "Воздух сильно загрязнен", "Powietrze silnie zanieczyszczone", "Повітря сильно забруднене", "Въздухът е силно замърсен"}
        };

        String _weatherDescription[16][6] = {
            {"Clear sky", "Klarer Himmel", "Ясно", "Czyste Niebo", "Ясно", "Ясно"},
            {"Mainly clear", "Überwiegend klar", "Преимущественно ясно", "Głównie jasne", "Переважно ясно", "Предимно ясно"},
            {"Partly cloudy", "Teilweise bewölkt", "Переменная облачность", "Częściowe zachmurzenie", "Мінлива хмарність", "Предимно облачно"},
            {"Overcast", "Bedeckt", "Пасмурно", "Pochmurny", "Хмарно", "Облачно"},
            {"Fog", "Nebel", "Туман", "Mgła", "Туман", "Мъгла"},
            {"Depositing rime fog", "Ablagerung von Raureifnebel", "Туман с инеем", "Deponująca się mgła szronowa", "Туман з інеєм", "Мъгла със скреж"},
            {"Drizzle", "Nieselregen", "Морось", "Mżawka", "Мряка", "Дъжд"},
            {"Freezing drizzle", "Gefrierender Nieselregen", "Изморозь", "Marznąca mżawka", "Ізморозь", "Скреж"},
            {"Rain", "Regen", "Дождь", "Deszcz", "Дощ", "Дъжд"},
            {"Freezing rain", "Gefrierender Regen", "Ледяной дождь", "Marznący deszcz", "Крижаний дощ", "Леден дъжд"},
            {"Snowfall", "Schneefall", "Снегопад", "Opady śniegu", "Снігопад", "Снеговалеж"},
            {"Snow grains", "Schneekörner", "Снежные зерна", "Ziarna śniegu", "Снігові зерна", "Скреж"},
            {"Rain showers", "Regenschauer", "Ливень", "Przelotne opady deszczu", "Злива", "Ръмеж"},
            {"Snow showers", "Schneeschauer", "Сильный снегопад", "Opady śniegu", "Сильний снігопад", "Обилен снеговалеж"},
            {"Thunderstorm", "Gewitter", "Гроза", "Burza z piorunami", "Гроза", "Буря"},
            {"Thunderstorm with hail", "Gewitter mit Hagel", "Гроза с градом", "Burza z gradem", "Гроза з градом", "Буря с градушка"}
        };

        String _alarm[6] = {"Alarm", "Wecker", "Будильник", "Budzik", "Будильник", "Будилник"};
        String _network[6] = {"Network", "Netzwerk", "Сеть", "Sieć", "Мережа", "Мрежа"};
        String _signalLevel[6] = {"Signal level", "Signalpegel", "Уровень сигнала", "Siła sygnału", "Рівень сигналу", "Ниво на сигнала"};
        String _ipAddr[6] = {"IP address", "IP Adresse", "IP адрес", "Adres IP", "IP адреса", "IP адрес"};
        String _macAddr[6] = {"MAC address", "MAC Adresse", "MAC адрес", "MAC adres", "MAC адреса", "MAC адрес"};
        String _esp32Temp[6] = {"ESP32 Temperature", "ESP32 Temperatur", "Температура ESP32", "Temperatura ESP32", "Температура ESP32", "Температура ESP32"};
        String _firmware[6] = {"Firmware", "Firmware", "Прошивка", "Oprogramowanie", "Прошивка", "Фърмуер"};
        String _mm[6] = {"mm", "mm", "мм", "mm", "мм", "мм"};
        String _ms[6] = {"m/s", "m/s", "м/с", "m/s" "м/с", "м/с"};
        String _v[6] = {"V", "V", " В", "V", " В", " В"};
        String _historyIn[6] = {"Weather indoor history", "Hauswettergeschichte", "История погоды в доме", "Historia pomiarów w pomieszczeniach", "Історія погоди у будинку", "История на времето в дома"};
        String _historyOut[6] = {"Weather outdoor history", "Außenwettergeschichte", "История погоды на улице", "Historia pogody na zewnątrz", "Історія погоди на вулиці", "История на времето навън"};

        uint8_t _lang() {
            if(config.lang() == "de") return 1;
            if(config.lang() == "ru") return 2;
            if(config.lang() == "pl") return 3;
            if(config.lang() == "ua") return 4;
            if(config.lang() == "bg") return 5;
            return 0;
        }


    public:
        String monthFullName(uint8_t num) {
            if(num < 1 || num > 12) return "???";
            return _monthFullName[num - 1][_lang()];
        }

        String monthDay(uint8_t num) {
            if(num < 1 || num > 12) return "???";
            return _monthDay[num - 1][_lang()];
        }

        String monthShortName(uint8_t num) {
            if(num < 1 || num > 12) return "???";
            return _monthShortName[num - 1][_lang()];
        }

        String weekdayShortName(uint8_t num) {
            if(num < 1 || num > 7) return "???";
            return _weekdayShortName[num - 1][_lang()];
        }

        String weekdayFullName(uint8_t num) {
            if(num < 1 || num > 7) return "???";
            return _weekdayFullName[num - 1][_lang()];
        }

        String alarm() {
            return _alarm[_lang()];
        }

        String network() {
            return _network[_lang()];
        }

        String signalLevel() {
            return _signalLevel[_lang()];
        }

        String ipAddr() {
            return _ipAddr[_lang()];
        }

        String macAddr() {
            return _macAddr[_lang()];
        }

        String esp32Temp() {
            return _esp32Temp[_lang()];
        }

        String firmware() {
            return _firmware[_lang()];
        }

        String mm() {
            return _mm[_lang()];
        }

        String ms() {
            return _ms[_lang()];
        }

        String v() {
            return _v[_lang()];
        }

        String historyIn() {
            return _historyIn[_lang()];
        }

        String historyOut() {
            return _historyOut[_lang()];
        }

        String comfort(unsigned int level) {
            if(level < 1 || level > 9) return "";
            return _comfort[level - 1][_lang()];
        }

        String airQuality(unsigned int level) {
            if(level < 1 || level > 3) return "";
            return _airQuality[level - 1][_lang()];
        }

        String weatherDescription(unsigned int level) {
            if(level > 15) return "???";
            return _weatherDescription[level][_lang()];
        }
};
