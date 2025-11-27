static const char* weatherDescription[16][7] PROGMEM = {
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

class Lang {
    private:
        /* English, German, Russian, Polish, Ukrainian, Bulgarian, Spanish */
        String _monthFullName[12][7] = {
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

        String _monthDay[12][7] {
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

        String _monthShortName[12][7] = {
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

        String _weekdayFullName[7][7] = {
            {"Sunday", "Sonntag", "Воскресенье", "Niedziela", "Неділя", "Неделя", "Domingo"},
            {"Monday", "Montag", "Понедельник", "Poniedziałek", "Понеділок", "Понеделник", "Lunes"},
            {"Tuesday", "Dienstag", "Вторник", "Wtorek", "Вівторок", "Вторник", "Martes"},
            {"Wednesday", "Mittwoch", "Среда", "Środa", "Середа", "Сряда", "Miércoles"},
            {"Thursday", "Donnerstag", "Четверг", "Czwartek", "Четвер", "Четвъртък", "Jueves"},
            {"Friday", "Freitag", "Пятница", "Piątek", "П'ятниця", "Петък", "Viernes"},
            {"Saturday", "Samstag", "Суббота", "Sobota", "Субота", "Събота", "Sábado"}
        };

        String _weekdayShortName[7][7] = {
            {"Su", "So", "Вс", "Nd", "Нд", "Не", "Do"},
            {"Mo", "Mo", "Пн", "Pn", "Пн", "По", "Lu"},
            {"Tu", "Di", "Вт", "Wt", "Вт", "Вт", "Ma"},
            {"We", "Mi", "Ср", "Śr", "Ср", "Ср", "Mi"},
            {"Th", "Do", "Чт", "Cz", "Чт", "Че", "Ju"},
            {"Fr", "Fr", "Пт", "Pi", "Пт", "Пе", "Vi"},
            {"Sa", "Sa", "Сб", "So", "Сб", "Съ", "Sá"}
        };

        String _comfort[9][7] = {
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

        String _airQuality[3][7] = {
            {"Clean Air", "Saubere Luft", "Воздух чистый", "Powietrze czyste", "Повітря чисте", "Въздухът е чист", "Aire limpio"},
            {"Air is polluted", "Luft ist verschmutzt", "Воздух загрязнен", "Powietrze zanieczyszczone", "Повітря забруднене", "Въздухът е замърсен", "Aire contaminado"},
            {"Air is heavily polluted", "Luft ist stark verschmutzt", "Воздух сильно загрязнен", "Powietrze silnie zanieczyszczone", "Повітря сильно забруднене", "Въздухът е силно замърсен", "Aire muy contaminado"}
        };


        String _alarm[7] = {"Alarm", "Wecker", "Будильник", "Budzik", "Будильник", "Будилник", "Alarma"};
        String _network[7] = {"Network", "Netzwerk", "Сеть", "Sieć", "Мережа", "Мрежа", "Red"};
        String _signalLevel[7] = {"Signal level", "Signalpegel", "Уровень сигнала", "Siła sygnału", "Рівень сигналу", "Ниво на сигнала", "Nivel de señal"};
        String _ipAddr[7] = {"IP address", "IP Adresse", "IP адрес", "Adres IP", "IP адреса", "IP адрес", "Dirección IP"};
        String _macAddr[7] = {"MAC address", "MAC Adresse", "MAC адрес", "MAC adres", "MAC адреса", "MAC адрес", "Dirección MAC"};
        String _esp32Temp[7] = {"ESP32 Temperature", "ESP32 Temperatur", "Температура ESP32", "Temperatura ESP32", "Температура ESP32", "Температура ESP32", "Temperatura ESP32"};
        String _firmware[7] = {"Firmware", "Firmware", "Прошивка", "Oprogramowanie", "Прошивка", "Фърмуер", "Firmware"};
        String _mm[7] = {"mm", "mm", "мм", "mm", "мм", "мм", "mm"};
        String _hpa[7] = {"hPa", "hPa", "гПа", "hPa", "гПа", "гПа", "hPa"};
        String _ms[7] = {"m/s", "m/s", "м/с", "m/s", "м/с", "м/с", "m/s"};
        String _gpm[7] = {"g/m³", "g/m³", "г/м³", "g/m³", "г/м³", "г/м³", "g/m³"};
        String _v[7] = {"V", "V", " В", "V", " В", " В", "V"};
        String _historyIn[7] = {"Weather indoor history", "Hauswettergeschichte", "История погоды в доме", "Historia pomiarów w pomieszczeniach", "Історія погоди у будинку", "История на времето в дома", "Historial clima interior"};
        String _historyOut[7] = {"Weather outdoor history", "Außenwettergeschichte", "История погоды на улице", "Historia pogody na zewnątrz", "Історія погоди на вулиці", "История на времето навън", "Historial clima exterior"};
        String _touchCalibrate[7] = {"Touch corners as indicated", "Berühren Sie die Ecken wie angegeben", "Прикоснитесь к углам, как указано", "Dotknij narożników zgodnie ze wskazaniem", "Торкніться кутів, як зазначено", "Докоснете ъглите, както е показано", "Toca esquinas según se indica"};
        String _calibrationDone[7] = {"Calibration complete", "Die Kalibrierung ist abgeschlossen", "Калибровка завершена", "Kalibracja została zakończona", "Калібрування закінчено", "Калибрирането е завършено", "Calibración terminada"};

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

        String hpa() {
            return _hpa[_lang()];
        }

        String ms() {
            return _ms[_lang()];
        }

        String gpm() {
            return _gpm[_lang()];
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

        const char* weatherDescription(unsigned int level) {
            if(level > 15) return "???";
            return ::weatherDescription[level][_lang()];
        }

        String touchCalibrate() {
            return _touchCalibrate[_lang()];
        }

        String calibrationDone() {
            return _calibrationDone[_lang()];
        }
};
