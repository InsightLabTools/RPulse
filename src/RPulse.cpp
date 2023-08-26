#include "Arduino.h"
#include "GyverTimers.h"
#include "RPulse.h"

int RPulse::pinListSize = 0;
PinData RPulse::pinList[MAX_LIST_SIZE];

int RPulse::varListSize = 0;
VarData RPulse::varList[MAX_LIST_SIZE];

/**
 * @brief Конструктор класса RPulse
 */
RPulse::RPulse()
{
    int paramsListSize = 0;
    Param paramsList[MAX_LIST_SIZE];
}

/**
 * @brief Инициализация последовательного подключения
 * @param baudRate скорость подключения
 */
void RPulse::init(int baudRate)
{
    Serial.begin(baudRate);
    Serial.setTimeout(100);
}

/**
 * @brief Ожидаем команды для продолжения работы.
 */
void RPulse::wait()
{
    String rawMessage;

    for (;;)
    {
        char message[128];

        // читаем байты до символа ;
        int numBytes = Serial.readBytesUntil(';', message, 128);
        // если есть прочитанные байты...
        if (numBytes > 0)
        {
            // ... преобразуем их в строку
            rawMessage = this->charArrayToString(message, numBytes);
            if (rawMessage.startsWith("p<"))
            {
                // парсим полученные параметры
                this->parseParams(rawMessage);
                break;
            }
        }
    }

    // запускаем отслеживание значений после команды
    this->start();
}

/**
 * @brief Старт обработки прерываний и отправки отслеживаемых значений
 */
void RPulse::start()
{
    // запускаем обработку прерывания по таймеру
    Timer1.setPeriod(READINGS_RESOLUTION);
    Timer1.enableISR(CHANNEL_A);
}

/**
 * @brief Отслеживать значения на указанном пине.
 * @param pin номер пина
 * @param type тип пина (цифровой/аналоговый)
 * @param key название для значения
 */
void RPulse::watchPin(int pin, PinType type, String key)
{
    if (this->pinListSize == MAX_LIST_SIZE)
        return;
    PinData sensor;
    sensor.pin = pin;
    sensor.key = key;
    sensor.type = type;

    this->pinList[this->pinListSize] = sensor;
    this->pinListSize++;
}

/**
 * @brief Отслеживать значение целочисленной переменной
 * @param var переменная
 * @param key название для значения
 */
void RPulse::watchVar(int &var, String key)
{
    if (this->varListSize == MAX_LIST_SIZE)
        return;
    VarData variable;
    variable.intValue = &var;
    variable.key = key;
    variable.type = intVar;

    this->varList[this->varListSize] = variable;
    this->varListSize++;
}

/**
 * @brief Отслеживать значение вещественной переменной
 * @param var переменная
 * @param key название для значения
 */
void RPulse::watchVar(float &var, String key)
{
    if (this->varListSize == MAX_LIST_SIZE)
        return;
    VarData variable;
    variable.floatValue = &var;
    variable.key = key;
    variable.type = floatVar;

    this->varList[this->varListSize] = variable;
    this->varListSize++;
}

/**
 * @brief Отправляем значения с датчиков через последовательный порт.
 */
void RPulse::send()
{
    String readings = "p>";

    // перебираем список пинов и считываем значения
    int sensorsSize = RPulse::pinListSize;
    for (int i = 0; i < sensorsSize; i++)
    {
        int sensorValue;
        PinData data = RPulse::pinList[i];

        // используем подходяющую функцию исходя из типа сигнала
        if (data.type == analog)
        {
            sensorValue = analogRead(data.pin);
        }
        if (data.type == digital)
        {
            sensorValue = digitalRead(data.pin);
        }
        readings += F("pin:");
        readings += data.key;
        readings += F(":");
        readings += sensorValue;
        readings += F("|");
    }

    // перебираем список отслеживаемых переменных
    int varsSize = RPulse::varListSize;
    for (int i = 0; i < varsSize; i++)
    {
        VarData data = RPulse::varList[i];
        if (data.type == intVar)
        {
            readings += F("var:");
            readings += data.key;
            readings += F(":");
            readings += *data.intValue;
            readings += F("|");
        }
        if (data.type == floatVar)
        {
            readings += F("var:");
            readings += data.key;
            readings += F(":");
            readings += *data.floatValue;
            readings += F("|");
        }
    }

    // отправляем строку с значениями
    Serial.println(readings);
}

/**
 * @brief Парсинг входных параметров эксперимента
 * Параметры приходят в формате p<ключ1:значени1|ключ2:значение2|
 * @param rawParams строка с параметрами
 */
void RPulse::parseParams(String rawParams)
{
    // нет параметров для парсинга
    if (rawParams.length() == 2)
        return;

    int start = 2;                                      // пропускаем строку "p<"
    int paramSeparator = rawParams.indexOf('|', start); // ищем первый разделитель |
    // повторяем, пока есть сепаратор |
    while (paramSeparator != -1)
    {
        String param = rawParams.substring(start, paramSeparator);                       // подстрока ключ:значение
        int valueSeparator = rawParams.indexOf(':', start);                              // позиция разделителя в паре ключ:значение
        String key = rawParams.substring(start, valueSeparator);                         // получаем ключ
        float value = rawParams.substring(valueSeparator + 1, paramSeparator).toFloat(); // получаем значение

        // если в массиве параметров ещё есть место...
        if (this->paramsListSize != MAX_LIST_SIZE)
        {
            Param p;
            p.key = key;
            p.value = value;

            // ... сохраняем параметр в массив
            this->paramsList[this->paramsListSize] = p;
            this->paramsListSize++;
        }

        // на следующей итерации начинаем поиск после полученных значений
        start = paramSeparator + 1;
        // ищем индекс следующего сепаратора |
        paramSeparator = rawParams.indexOf('|', start);
    }
}

float RPulse::get(String key, float defaultValue = 0.0)
{
    float value = defaultValue; // будет возвращать дефолтное значение, если не найдём указанный ключ

    // перебираем массив параметров, чтобы найти ключ key
    for (int i = 0; i < this->paramsListSize; i++)
    {
        Param p = this->paramsList[i];
        if (key == p.key)
        {
            value = p.value;
            break;
        }
    }

    return value;
}

/**
 * @brief Обработчик прерывания таймера
 */
ISR(TIMER1_A)
{
    RPulse::send();
}

/**
 * @brief Преобразуем массив символов в строку
 * @param a массив символов
 * @param size размер массива
 * @return String итоговая строка
 */
String RPulse::charArrayToString(char *a, int size)
{
    int i;
    String s = "";
    for (i = 0; i < size; i++)
    {
        s = s + a[i];
    }
    return s;
}