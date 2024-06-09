/*
 *      Пример использования терминала Serial
 *      Источник: https://github.com/Alash-electronics/AlashSerialTerminal
 */

#include <AlashSerialTerminal.h>

// Пин светодиода
#define LED_PIN     LED_BUILTIN

// Символ новой строки '\r' или '\n'
char newlineChar = '\n';
// Символ-разделитель между командами и аргументами
char delimiterChar = ' ';

// Создание объекта терминала
SerialTerminal term(newlineChar, delimiterChar);

// Прототипы функций
void unknownCommand(const char *command);
void cmdHelp();
void cmdLedOn();
void cmdLedOff();
void cmdPrintHello();
void cmdPrintIntArgument();
void cmdPrintStringArguments();


void setup()
{
    // Задержка для инициализации последовательного порта
    delay(500);

    // Инициализация последовательного порта
    Serial.begin(115200);
    Serial.println(F("\nПример терминала Serial."));
    Serial.println(F("Введите 'help', чтобы увидеть инструкцию."));

    // Инициализация встроенного светодиода
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Установка обработчика по умолчанию для неизвестных команд
    term.setDefaultHandler(unknownCommand);

    // Добавление обработчиков команд
    term.addCommand("?", cmdHelp);
    term.addCommand("help", cmdHelp);
    term.addCommand("on", cmdLedOn);
    term.addCommand("off", cmdLedOff);
    term.addCommand("hello", cmdPrintHello);
    term.addCommand("i", cmdPrintIntArgument);
    term.addCommand("s", cmdPrintStringArguments);
}

void loop()
{
    // Чтение из последовательного порта и обработка обратных вызовов команд
    term.readSerial();
}

void unknownCommand(const char *command)
{
    // Вывод неизвестной команды
    Serial.print(F("Неизвестная команда: "));
    Serial.println(command);
}

void cmdHelp()
{
    // Вывод инструкции
    Serial.println(F("Инструкция по использованию терминала Serial:"));
    Serial.println(F("  help или ?          Вывести эту инструкцию"));
    Serial.println(F("  on                 Включить светодиод"));
    Serial.println(F("  off                Выключить светодиод"));
    Serial.println(F("  hello [name] [age] Вывести ваше имя и возраст"));
    Serial.println(F("  s <arg 1>..<arg N> Вывести строковые аргументы"));
    Serial.println(F("  i <arg>            Вывести десятичный или шестнадцатеричный аргумент"));
}

void cmdLedOn()
{
    // Включение светодиода
    Serial.println(F("Светодиод включен"));
    digitalWrite(LED_PIN, HIGH);
}

void cmdLedOff()
{
    // Выключение светодиода
    Serial.println(F("Светодиод выключен"));
    digitalWrite(LED_PIN, LOW);
}

void cmdPrintHello()
{
    char *arg;
    char *name = NULL;
    int age = 0;

    // Получение первого аргумента
    arg = term.getNext();
    if (arg != NULL) {
        // Попытка преобразования аргумента в int
        if (atoi(arg)) {
            // Имя не указано
            // Преобразование строки возраста в int
            age = atoi(arg);
        } else {
            // Сохранение первого аргумента (имени)
            name = arg;

            // Получение второго аргумента (возраста)
            arg = term.getNext();

            // Преобразование строки возраста в int
            age = atoi(arg);
        }
    }

    // Вывод имени
    if (name) {
        Serial.print(F("Привет "));
        Serial.print(name);
        Serial.println(F("!"));
    } else {
        Serial.println(F("Привет. Вы не указали свое имя."));
    }

    // Вывод возраста
    if (age) {
        Serial.print(F("Вам "));
        Serial.print(age, DEC);
        Serial.println(F(" лет."));
    } else {
        Serial.println(F("Вы не указали свой возраст."));
    }
}

void cmdPrintIntArgument()
{
    int val;
    char *arg;

    // Получение аргумента
    arg = term.getNext();
    if (arg == NULL) {
        Serial.println(F("Не указано десятичное или шестнадцатеричное значение."));
        return;
    }

    // Попытка преобразования символьной строки десятичного или шестнадцатеричного аргумента в int
    if (strncmp(arg, "0x", 2) == 0) {
        if (sscanf(arg, "0x%x", &val) != 1) {
            Serial.println(F("Не удалось преобразовать шестнадцатеричное значение."));
            return;
        }
    } else {
        if (sscanf(arg, "%d", &val) != 1) {
            Serial.println(F("Не удалось преобразовать десятичное значение."));
            return;
        }
    }

    // Вывод аргумента как строки и значения в шестнадцатеричном/десятичном формате
    Serial.print(F("Строка: "));
    Serial.print(arg);
    Serial.print(F("; "));

    Serial.print(F("Шестнадцатеричное: 0x"));
    if (val < 0x10) {
        Serial.print(F("0"));
    }
    Serial.print(val, HEX);
    Serial.print(F("; "));

    Serial.print(F("Десятичное: "));
    Serial.println(val, DEC);
}

void cmdPrintStringArguments()
{
    int argIndex;
    char *arg;
    char *remaining;

    // Вывод аргументов
    Serial.println(F("Аргументы [1..4]: "));
    for (argIndex = 0; argIndex < 4; argIndex++) {
        arg = term.getNext();
        if (arg != NULL) {
            Serial.print(F("  "));
            Serial.print(argIndex);
            Serial.print(F(": "));
            Serial.println(arg);
        } else {
            break;
        }
    }

    if (argIndex == 0) {
        // Аргументы не указаны
        Serial.println(F("  Нет"));
    } else {
        // Вывод оставшейся строки
        remaining = term.getRemaining();

        if (remaining) {
            Serial.print(F("Оставшееся: "));
            Serial.println(remaining);
        }
    }
}