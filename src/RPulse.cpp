#include "Arduino.h"
#include "GyverTimers.h"
#include "RPulse.h"

int RPulse::sensorsListSize = 0;
SensorData RPulse::sensorsList[10];

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
void RPulse::watchSensor(int pin, SensorType type, String key)
{
    SensorData sensor;
    sensor.pin = pin;
    sensor.key = key;
    sensor.type = type;

    this->sensorsList[this->sensorsListSize] = sensor;
    this->sensorsListSize++;
}

/**
 * @brief Отправляем значения с датчиков через последовательный порт.
 */
void RPulse::send()
{
    String readings = "p>";
    int sensorsSize = RPulse::sensorsListSize;
    for (int i = 0; i < sensorsSize; i++)
    {
        int sensorValue;
        SensorData data = RPulse::sensorsList[i];
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