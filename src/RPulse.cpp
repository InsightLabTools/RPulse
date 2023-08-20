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
}

/**
 * @brief Инициализация последовательного подключения
 * @param baudRate скорость подключения
 */
void RPulse::init(int baudRate)
{
    Serial.begin(baudRate);
}

/**
 * @brief Ожидаем команды для продолжения работы.
 */
void RPulse::wait()
{
    String command;

    while (true)
    {
        char message[10];

        // читаем байты до символа ;
        int numBytes = Serial.readBytesUntil(';', message, 10);
        // если есть прочитанные байты...
        if (numBytes > 0)
        {
            // ... преобразуем их в строку
            command = this->charArrayToString(message, numBytes);
            if (command == COMMAND_START)
            {
                // пришла подходящая команда
                // можно продолжать
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
    Timer1.setPeriod(READINGS_RESULUTION);
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
        readings += data.key + ":" + String(sensorValue) + ";";
    }

    // перебираем список отслеживаемых переменных
    int varsSize = RPulse::varListSize;
    for (int i = 0; i < varsSize; i++)
    {
        VarData data = RPulse::varList[i];
        if (data.type == intVar)
        {
            readings += data.key + ":" + String(*data.intValue) + ";";
        }
        if (data.type == floatVar)
        {
            readings += data.key + ":" + String(*data.floatValue) + ";";
        }
    }

    // отправляем строку с значениями
    Serial.println(readings);
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