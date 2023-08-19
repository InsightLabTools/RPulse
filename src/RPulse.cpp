#include "Arduino.h"
#include "GyverTimers.h"
#include "RPulse.h"

RPulse::RPulse()
{
}

/**
 * Инициализация соединения
 */
void RPulse::init(int baudRate)
{
    Serial.begin(baudRate);
}

/**
 * Ожидание команды для запуска робота
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
            command = charArrayToString(message, numBytes);
            if (command == COMMAND_START)
            {
                // пришла подходящая команда
                break;
            }
        }
    }
}

/**
 * Преобразование массива символов в строку
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