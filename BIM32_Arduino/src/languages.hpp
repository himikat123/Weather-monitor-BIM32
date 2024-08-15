class Lang {
    private:
        String _day() {
            if(config.lang() == "en") return "day";
            if(config.lang() == "de") return "Tag";
            if(config.lang() == "pl") return "dzień";
            return "день";
        }

        String _days2() {
            if(config.lang() == "de") return "Tage";
            if(config.lang() == "ru") return "дня";
            if(config.lang() == "pl") return "dni";
            if(config.lang() == "ua") return "дні";
            return "days";  
        }

        String _days5() {
            if(config.lang() == "de") return "Tage";
            if(config.lang() == "ru") return "дней";
            if(config.lang() == "pl") return "dni";
            if(config.lang() == "ua") return "днів";
            return "days";  
        }

        String _hour() {
            if(config.lang() == "de") return "Stunde";
            if(config.lang() == "ru") return "час";
            if(config.lang() == "pl") return "godzina";
            if(config.lang() == "ua") return "година";
            return "hour";  
        }

        String _hours2() {
            if(config.lang() == "de") return "Stunden";
            if(config.lang() == "ru") return "часа";
            if(config.lang() == "pl") return "godziny";
            if(config.lang() == "ua") return "години";
            return "hours";  
        }

        String _hours5() {
            if(config.lang() == "de") return "Stunden";
            if(config.lang() == "ru") return "часов";
            if(config.lang() == "pl") return "godzin";
            if(config.lang() == "ua") return "годин";
            return "hours";  
        }

        String _minute() {
            if(config.lang() == "de") return "Minute";
            if(config.lang() == "ru") return "минута";
            if(config.lang() == "pl") return "minuta";
            if(config.lang() == "ua") return "хвилина";
            return "minute";
        }

        String _minutes2() {
            if(config.lang() == "de") return "Minuten";
            if(config.lang() == "ru") return "минуты";
            if(config.lang() == "pl") return "minuty";
            if(config.lang() == "ua") return "хвилини";
            return "minutes";  
        }

        String _minutes5() {
            if(config.lang() == "de") return "Minuten";
            if(config.lang() == "ru") return "минут";
            if(config.lang() == "pl") return "minut";
            if(config.lang() == "ua") return "хвилин";
            return "minutes";  
        }

        String _lessThanAMinute() {
            if(config.lang() == "de") return "Weniger als einer Minute";
            if(config.lang() == "ru") return "Меньше минуты";
            if(config.lang() == "pl") return "Mniej niż minutę";
            if(config.lang() == "ua") return "Менше хвилини";
            return "Less than a minute";  
        }

        String _and() {
            if(config.lang() == "de") return "und";
            if(config.lang() == "ru") return "и";
            if(config.lang() == "pl") return "i";
            if(config.lang() == "ua") return "та";
            return "and";  
        }

    public:
        String january() {
            if(config.lang() == "de") return "Januar";
            if(config.lang() == "ru") return "января";
            if(config.lang() == "pl") return "stycznia";
            if(config.lang() == "ua") return "січня";
            return "January";  
        }

        String february() {
            if(config.lang() == "de") return "Februar";
            if(config.lang() == "ru") return "февраля";
            if(config.lang() == "pl") return "lutego";
            if(config.lang() == "ua") return "лютого";
            return "February";  
        }

        String march() {
            if(config.lang() == "de") return "März";
            if(config.lang() == "ru") return "марта";
            if(config.lang() == "pl") return "marca";
            if(config.lang() == "ua") return "березня";
            return "March";  
        }

        String april() {
            if(config.lang() == "ru") return "апреля";
            if(config.lang() == "pl") return "kwietnia";
            if(config.lang() == "ua") return "квітня";
            return "April";  
        }

        String may() {
            if(config.lang() == "de") return "Mai";
            if(config.lang() == "ru") return "мая";
            if(config.lang() == "pl") return "maja";
            if(config.lang() == "uy") return "травня";
            return "May";
        }

        String june() {
            if(config.lang() == "de") return "Juni";
            if(config.lang() == "ru") return "июня";
            if(config.lang() == "pl") return "czerwca";
            if(config.lang() == "ua") return "червня";
            return "June";
        }

        String july() {
            if(config.lang() == "de") return "Juli";
            if(config.lang() == "ru") return "июля";
            if(config.lang() == "pl") return "lipca";
            if(config.lang() == "ua") return "липня";
            return "July";  
        }

        String august() {
            if(config.lang() == "ru") return "августа";
            if(config.lang() == "pl") return "sierpnia";
            if(config.lang() == "ua") return "серпня";
            return "August";
        }

        String september() {
            if(config.lang() == "ru") return "сентября";
            if(config.lang() == "pl") return "września";
            if(config.lang() == "ua") return "вересня";
            return "September";
        }

        String october() {
            if(config.lang() == "de") return "Oktober";
            if(config.lang() == "ru") return "октября";
            if(config.lang() == "pl") return "października";
            if(config.lang() == "ua") return "жовтня";
            return "October";
        }

        String november() {
            if(config.lang() == "ru") return "ноября";
            if(config.lang() == "pl") return "listopada";
            if(config.lang() == "ua") return "листопада";
            return "November";  
        }

        String december() {
            if(config.lang() == "de") return "Dezember";
            if(config.lang() == "ru") return "декабря";
            if(config.lang() == "pl") return "grudnia";
            if(config.lang() == "ua") return "грудня";
            return "December";  
        }

        String january_() {
            if(config.lang() == "de") return "Januar";
            if(config.lang() == "ru") return "Январь";
            if(config.lang() == "pl") return "Styczeń";
            if(config.lang() == "ua") return "Січень";
            return "January";  
        }

        String february_() {
            if(config.lang() == "de") return "Februar";
            if(config.lang() == "ru") return "Февраль";
            if(config.lang() == "pl") return "Luty";
            if(config.lang() == "ua") return "Лютий";
            return "February";  
        }

        String march_() {
            if(config.lang() == "de") return "März";
            if(config.lang() == "ru") return "Март";
            if(config.lang() == "pl") return "Marsz";
            if(config.lang() == "ua") return "Березень";
            return "March";  
        }

        String april_() {
            if(config.lang() == "ru") return "Апрель";
            if(config.lang() == "pl") return "Kwiecień";
            if(config.lang() == "ua") return "Квітень";
            return "April";  
        }

        String may_() {
            if(config.lang() == "de") return "Mai";
            if(config.lang() == "ru") return "Май";
            if(config.lang() == "pl") return "Maj";
            if(config.lang() == "ua") return "Травень";
            return "May";
        }

        String june_() {
            if(config.lang() == "de") return "Juni";
            if(config.lang() == "ru") return "Июнь";
            if(config.lang() == "pl") return "Czerwiec";
            if(config.lang() == "ua") return "Червень";
            return "June";
        }

        String july_() {
            if(config.lang() == "de") return "Juli";
            if(config.lang() == "ru") return "Июль";
            if(config.lang() == "pl") return "Lipiec";
            if(config.lang() == "ua") return "Липень";
            return "July";  
        }

        String august_() {
            if(config.lang() == "ru") return "Август";
            if(config.lang() == "pl") return "Sierpień";
            if(config.lang() == "ua") return "Серпень";
            return "August";
        }

        String september_() {
            if(config.lang() == "ru") return "Сентябрь";
            if(config.lang() == "pl") return "Wrzesień";
            if(config.lang() == "ua") return "Вересень";
            return "September";
        }

        String october_() {
            if(config.lang() == "de") return "Oktober";
            if(config.lang() == "ru") return "Октябрь";
            if(config.lang() == "pl") return "Październik";
            if(config.lang() == "ua") return "Жовтень";
            return "October";
        }

        String november_() {
            if(config.lang() == "ru") return "Ноябрь";
            if(config.lang() == "pl") return "Listopad";
            if(config.lang() == "ua") return "Листопад";
            return "November";  
        }

        String december_() {
            if(config.lang() == "de") return "Dezember";
            if(config.lang() == "ru") return "Декабрь";
            if(config.lang() == "pl") return "Grudzień";
            if(config.lang() == "ua") return "Грудень";
            return "December";  
        }

        String jan() {
            if(config.lang() == "ru") return "янв";
            if(config.lang() == "pl") return "sty";
            if(config.lang() == "ua") return "січ";
            return "Jan";  
        }

        String feb() {
            if(config.lang() == "ru") return "фев";
            if(config.lang() == "pl") return "lut";
            if(config.lang() == "ua") return "лют";
            return "Feb";
        }

        String mar() {
            if(config.lang() == "de") return "Mär";
            if(config.lang() == "ru") return "мар";
            if(config.lang() == "pl") return "mar";
            if(config.lang() == "ua") return "бер";
            return "Mar";
        }

        String apr() {
            if(config.lang() == "ru") return "апр";
            if(config.lang() == "pl") return "kwi";
            if(config.lang() == "ua") return "кві";
            return "Apr";  
        }

        String mai() {
            if(config.lang() == "de") return "Mai";
            if(config.lang() == "ru") return "мая";
            if(config.lang() == "pl") return "maj";
            if(config.lang() == "ua") return "тра";
            return "May";  
        }

        String jun() {
            if(config.lang() == "ru") return "июн";
            if(config.lang() == "pl") return "cze";
            if(config.lang() == "ua") return "чер";
            return "Jun";  
        }

        String jul() {
            if(config.lang() == "ru") return "июл";
            if(config.lang() == "pl") return "lip";
            if(config.lang() == "ua") return "лип";
            return "Jul";  
        }

        String aug() {
            if(config.lang() == "ru") return "авг";
            if(config.lang() == "pl") return "sie";
            if(config.lang() == "ua") return "сер";
            return "Aug";
        }

        String sep() {
            if(config.lang() == "ru") return "сен";
            if(config.lang() == "pl") return "wrz";
            if(config.lang() == "ua") return "вер";
            return "Sep";  
        }

        String oct() {
            if(config.lang() == "de") return "Okt";
            if(config.lang() == "ru") return "окт";
            if(config.lang() == "pl") return "paź";
            if(config.lang() == "ua") return "жов";
            return "Oct";  
        }

        String nov() {
            if(config.lang() == "ru") return "ноя";
            if(config.lang() == "pl") return "lis";
            if(config.lang() == "ua") return "лис";
            return "Nov";
        }

        String dec() {
            if(config.lang() == "de") return "Dez";
            if(config.lang() == "ru") return "дек";
            if(config.lang() == "pl") return "gru";
            if(config.lang() == "ua") return "гру";
            return "Dec";  
        }

        String monday() {
            if(config.lang() == "de") return "Montag";
            if(config.lang() == "ru") return "Понедельник";
            if(config.lang() == "pl") return "Poniedziałek";
            if(config.lang() == "ua") return "Понеділок";
            return "Monday";  
        }

        String tuesday() {
            if(config.lang() == "de") return "Dienstag";
            if(config.lang() == "ru") return "Вторник";
            if(config.lang() == "pl") return "Wtorek";
            if(config.lang() == "ua") return "Вівторок";
            return "Tuesday";  
        }

        String wednesday() {
            if(config.lang() == "de") return "Mittwoch";
            if(config.lang() == "ru") return "Среда";
            if(config.lang() == "pl") return "Środa";
            if(config.lang() == "ua") return "Середа";
            return "Wednesday";  
        }

        String thursday() {
            if(config.lang() == "de") return "Donnerstag";
            if(config.lang() == "ru") return "Четверг";
            if(config.lang() == "pl") return "Czwartek";
            if(config.lang() == "ua") return "Четвер";
            return "Thursday";  
        }

        String friday() {
            if(config.lang() == "de") return "Freitag";
            if(config.lang() == "ru") return "Пятница";
            if(config.lang() == "pl") return "Piątek";
            if(config.lang() == "ua") return "П'ятниця";
            return "Friday";  
        }

        String saturday() {
            if(config.lang() == "de") return "Samstag";
            if(config.lang() == "ru") return "Суббота";
            if(config.lang() == "pl") return "Sobota";
            if(config.lang() == "ua") return "Субота";
            return "Saturday";  
        }

        String sunday() {
            if(config.lang() == "de") return "Sonntag";
            if(config.lang() == "ru") return "Воскресенье";
            if(config.lang() == "pl") return "Niedziela";
            if(config.lang() == "ua") return "Неділя";
            return "Sunday";  
        }

        String mo() {
            if(config.lang() == "ru") return "Пн";
            if(config.lang() == "pl") return "Pn";
            if(config.lang() == "ua") return "Пн";
            return "Mo";  
        }

        String tu() {
            if(config.lang() == "de") return "Di";
            if(config.lang() == "ru") return "Вт";
            if(config.lang() == "pl") return "Wt";
            if(config.lang() == "ua") return "Вт";
            return "Tu";  
        }

        String we() {
            if(config.lang() == "de") return "Mi";
            if(config.lang() == "ru") return "Ср";
            if(config.lang() == "pl") return "Śr";
            if(config.lang() == "ua") return "Ср";
            return "We";  
        }

        String th() {
            if(config.lang() == "de") return "Do";
            if(config.lang() == "ru") return "Чт";
            if(config.lang() == "pl") return "Cz";
            if(config.lang() == "ua") return "Чт";
            return "Th";  
        }

        String fr() {
            if(config.lang() == "ru") return "Пт";
            if(config.lang() == "pl") return "Pi";
            if(config.lang() == "ua") return "Пт";
            return "Fr";  
        }

        String sa() {
            if(config.lang() == "ru") return "Сб";
            if(config.lang() == "pl") return "So";
            if(config.lang() == "ua") return "Сб";
            return "Sa";  
        }

        String su() {
            if(config.lang() == "de") return "So";
            if(config.lang() == "ru") return "Вс";
            if(config.lang() == "pl") return "Nd";
            if(config.lang() == "ua") return "Нд";
            return "Su";  
        }

        String alarm() {
            if(config.lang() == "de") return "Wecker";
            if(config.lang() == "ru") return "Будильник";
            if(config.lang() == "pl") return "Budzik";
            if(config.lang() == "ua") return "Будильник";
            return "Alarm";  
        }

        String network() {
            if(config.lang() == "de") return "Netzwerk";
            if(config.lang() == "ru") return "Сеть";
            if(config.lang() == "pl") return "Sieć";
            if(config.lang() == "ua") return "Мережа";
            return "Network";  
        }

        String signalLevel() {
            if(config.lang() == "de") return "Signalpegel";
            if(config.lang() == "ru") return "Уровень сигнала";
            if(config.lang() == "pl") return "Siła sygnału";
            if(config.lang() == "ua") return "Рівень сигналу";
            return "Signal level";  
        }

        String ipAddr() {
            if(config.lang() == "de") return "IP Adresse";
            if(config.lang() == "ru") return "IP адрес";
            if(config.lang() == "pl") return "Adres IP";
            if(config.lang() == "ua") return "IP адреса";
            return "IP address";  
        }

        String macAddr() {
            if(config.lang() == "de") return "MAC Adresse";
            if(config.lang() == "ru") return "MAC адрес";
            if(config.lang() == "pl") return "MAC adres";
            if(config.lang() == "ua") return "MAC адреса";
            return "MAC address";  
        }

        String esp32Temp() {
            if(config.lang() == "de") return "ESP32 Temperatur";
            if(config.lang() == "ru") return "Температура ESP32";
            if(config.lang() == "pl") return "Temperatura ESP32";
            if(config.lang() == "ua") return "Температура ESP32";
            return "ESP32 Temperature";  
        }

        String firmware() {
            if(config.lang() == "ru") return "Прошивка";
            if(config.lang() == "pl") return "Oprogramowanie";
            if(config.lang() == "ua") return "Прошивка";
            return "Firmware";  
        }

        String mm() { // millimeters
            if(config.lang() == "ru") return "мм";
            if(config.lang() == "ua") return "мм";
            return "mm";  
        }

        String ms() { // meters per second
            if(config.lang() == "ru") return "м/с";
            if(config.lang() == "ua") return "м/с";
            return "m/s";  
        }

        String v() { // volt
            if(config.lang() == "ru") return " В";
            if(config.lang() == "ua") return " В";
            return "V";
        }

        String historyIn() {
            if(config.lang() == "de") return "Hauswettergeschichte";
            if(config.lang() == "ru") return "История погоды в доме";
            if(config.lang() == "pl") return "Historia pomiarów w pomieszczeniach";
            if(config.lang() == "ua") return "Історія погоди у будинку";
            return "Weather indoor history";  
        }

        String historyOut() {
            if(config.lang() == "de") return "Außenwettergeschichte";
            if(config.lang() == "ru") return "История погоды на улице";
            if(config.lang() == "pl") return "Historia pogody na zewnątrz";
            if(config.lang() == "ua") return "Історія погоди на вулиці";
            return "Weather outdoor history";  
        }

        unsigned int plural(int num) {
            if(num > 10 && num < 20) return 0;
            if(num % 10 == 1) return 1;
            if(num % 10 > 1 && num % 10 < 5) return 2;
            return 0;
        }

        String comfort(unsigned int level) {
            switch(level) {
                case 1: {
                    if(config.lang() == "de") return "Gemütlich";
                    if(config.lang() == "ru") return "Комфортно";
                    if(config.lang() == "pl") return "Przyjemny";
                    if(config.lang() == "ua") return "Комфортно";
                    return "Comfortable";
                }; break;
                case 2: {
                    if(config.lang() == "de") return "Zu heiß";
                    if(config.lang() == "ru") return "Жарко";
                    if(config.lang() == "pl") return "Gorąco";
                    if(config.lang() == "ua") return "Жарко";
                    return "Too hot";
                }; break;
                case 3: {
                    if(config.lang() == "de") return "Zu kalt";
                    if(config.lang() == "ru") return "Холодно";
                    if(config.lang() == "pl") return "Zimno";
                    if(config.lang() == "ua") return "Холодно";
                    return "Too cold";
                }; break;
                case 4: {
                    if(config.lang() == "de") return "Zu feucht";
                    if(config.lang() == "ru") return "Слишком влажно";
                    if(config.lang() == "pl") return "Zbyt wilgotno";
                    if(config.lang() == "ua") return "Занадто волого";
                    return "Too humid";
                }; break;
                case 5: {
                    if(config.lang() == "de") return "Zu trocken";
                    if(config.lang() == "ru") return "Слишком сухо";
                    if(config.lang() == "pl") return "Zbyt sucho";
                    if(config.lang() == "ua") return "Занадто сухо";
                    return "Too dry";
                }; break;
                case 6: {
                    if(config.lang() == "de") return "Heiß und feucht";
                    if(config.lang() == "ru") return "Жарко и влажно";
                    if(config.lang() == "pl") return "Gorąco i wilgotno";
                    if(config.lang() == "ua") return "Жарко та волого";
                    return "Hot and humid";
                }; break;
                case 7: {
                    if(config.lang() == "de") return "Heiß und trocken";
                    if(config.lang() == "ru") return "Жарко и сухо";
                    if(config.lang() == "pl") return "Gorąco i sucho";
                    if(config.lang() == "ua") return "Жарко та сухо";
                    return "Hot and dry";
                }; break;
                case 8: {
                    if(config.lang() == "de") return "Kalt und feucht";
                    if(config.lang() == "ru") return "Холодно и влажно";
                    if(config.lang() == "pl") return "Zimno i wilgotno";
                    if(config.lang() == "ua") return "Холодно та волого";
                    return "Cold and humid";
                }; break;
                case 9: {
                    if(config.lang() == "de") return "Kalt und trocken";
                    if(config.lang() == "ru") return "Холодно и сухо";
                    if(config.lang() == "pl") return "Zimno i sucho";
                    if(config.lang() == "ua") return "Холодно та сухо";
                    return "Cold and dry";
                }; break;
                default: return ""; break;
            }
        }

        String airQuality(unsigned int level) {
            switch(level) {
                case 1: {
                    if(config.lang() == "de") return "Saubere Luft";
                    if(config.lang() == "ru") return "Воздух чистый";
                    if(config.lang() == "pl") return "Powietrze czyste";
                    if(config.lang() == "ua") return "Повітря чисте";
                    return "Clean Air";
                }; break;
                case 2: {
                    if(config.lang() == "de") return "Luft ist verschmutzt";
                    if(config.lang() == "ru") return "Воздух загрязнен";
                    if(config.lang() == "pl") return "Powietrze zanieczyszczone";
                    if(config.lang() == "ua") return "Повітря забруднене";
                    return "Air is polluted";
                }; break;
                case 3: {
                    if(config.lang() == "de") return "Luft ist stark verschmutzt";
                    if(config.lang() == "ru") return "Воздух сильно загрязнен";
                    if(config.lang() == "pl") return "Powietrze silnie zanieczyszczone";
                    if(config.lang() == "ua") return "Повітря сильно забруднене";
                    return "Air is heavily polluted";
                }; break;
                default: return ""; break;
            }
        }

        String weekdayShortName(unsigned int day) {
            switch(day) {
                case 1: return su();
                case 2: return mo();
                case 3: return tu();
                case 4: return we();
                case 5: return th();
                case 6: return fr();
                case 7: return sa();
                default: return "??"; break;
            }
        }

        String weekdayFullName(unsigned int day) {
            switch(day) {
                case 1: return sunday();
                case 2: return monday();
                case 3: return tuesday();
                case 4: return wednesday();
                case 5: return thursday();
                case 6: return friday();
                case 7: return saturday();
                default: return "??"; break;
            }
        }

        String monthName(unsigned int month) {
            switch(month) {
                case 1: return january();
                case 2: return february();
                case 3: return march();
                case 4: return april();
                case 5: return may();
                case 6: return june();
                case 7: return july();
                case 8: return august();
                case 9: return september();
                case 10: return october();
                case 11: return november();
                case 12: return december();
                default: return "???"; break;
            }
        }

        String clearSky() {
            if(config.lang() == "de") return "Klarer Himmel";
            if(config.lang() == "pl") return "Czyste Niebo";
            if(config.lang() == "ru" || config.lang() == "ua") return "Ясно";
            return "Clear sky";
        }

        String mainlyClear() {
            if(config.lang() == "de") return "Überwiegend klar";
            if(config.lang() == "pl") return "Głównie jasne";
            if(config.lang() == "ru") return "Преимущественно ясно";
            if(config.lang() == "ua") return "Переважно ясно";
            return "Mainly clear";
        }

        String partlyCloudy() {
            if(config.lang() == "de") return "Teilweise bewölkt";
            if(config.lang() == "pl") return "Częściowe zachmurzenie";
            if(config.lang() == "ru") return "Переменная облачность";
            if(config.lang() == "ua") return "Мінлива хмарність";
            return "Partly cloudy";
        }

        String overcast() {
            if(config.lang() == "de") return "Bedeckt";
            if(config.lang() == "pl") return "Pochmurny";
            if(config.lang() == "ru") return "Пасмурно";
            if(config.lang() == "ua") return "Хмарно";
            return "Overcast";
        }

        String fog() {
            if(config.lang() == "de") return "Nebel";
            if(config.lang() == "pl") return "Mgła";
            if(config.lang() == "ru" || config.lang() == "ua") return "Туман";
            return "Fog";
        }

        String deposRimeFog() {
            if(config.lang() == "de") return "Ablagerung von Raureifnebel";
            if(config.lang() == "pl") return "Deponująca się mgła szronowa";
            if(config.lang() == "ru") return "Туман с инеем";
            if(config.lang() == "ua") return "Туман з інеєм";
            return "Depositing rime fog";
        }

        String drizzle() {
            if(config.lang() == "de") return "Nieselregen";
            if(config.lang() == "pl") return "Mżawka";
            if(config.lang() == "ru") return "Морось";
            if(config.lang() == "ua") return "Мряка";
            return "Drizzle";
        }

        String freezingDrizzle() {
            if(config.lang() == "de") return "Gefrierender Nieselregen";
            if(config.lang() == "pl") return "Marznąca mżawka";
            if(config.lang() == "ru") return "Изморозь";
            if(config.lang() == "ua") return "Ізморозь";
            return "Freezing drizzle";
        }

        String rain() {
            if(config.lang() == "de") return "Regen";
            if(config.lang() == "pl") return "Deszcz";
            if(config.lang() == "ru") return "Дождь";
            if(config.lang() == "ua") return "Дощ";
            return "Rain";
        }

        String freezingRain() {
            if(config.lang() == "de") return "Gefrierender Regen";
            if(config.lang() == "pl") return "Marznący deszcz";
            if(config.lang() == "ru") return "Ледяной дождь";
            if(config.lang() == "ua") return "Крижаний дощ";
            return "Freezing rain";
        }

        String snowFall() {
            if(config.lang() == "de") return "Schneefall";
            if(config.lang() == "pl") return "Opady śniegu";
            if(config.lang() == "ru") return "Снегопад";
            if(config.lang() == "ua") return "Снігопад";
            return "Snowfall";
        }

        String snowGrains() {
            if(config.lang() == "de") return "Schneekörner";
            if(config.lang() == "pl") return "Ziarna śniegu";
            if(config.lang() == "ru") return "Снежные зерна";
            if(config.lang() == "ua") return "Снігові зерна";
            return "Snow grains";
        }

        String rainShowers() {
            if(config.lang() == "de") return "Regenschauer";
            if(config.lang() == "pl") return "Przelotne opady deszczu";
            if(config.lang() == "ru") return "Ливень";
            if(config.lang() == "ua") return "Злива";
            return "Rain showers";
        }

        String snowShowers() {
            if(config.lang() == "de") return "Schneeschauer";
            if(config.lang() == "pl") return "Opady śniegu";
            if(config.lang() == "ru") return "Сильный снегопад";
            if(config.lang() == "ua") return "Сильний снігопад";
            return "Snow showers";
        }

        String thunderstorm() {
            if(config.lang() == "de") return "Gewitter";
            if(config.lang() == "pl") return "Burza z piorunami";
            if(config.lang() == "ru" || config.lang() == "ua") return "Гроза";
            return "Thunderstorm";
        }

        String thunderstormWithHail() {
            if(config.lang() == "de") return "Gewitter mit Hagel";
            if(config.lang() == "pl") return "Burza z gradem";
            if(config.lang() == "ru") return "Гроза с градом";
            if(config.lang() == "ua") return "Гроза з градом";
            return "Thunderstorm with hail";
        }
};
