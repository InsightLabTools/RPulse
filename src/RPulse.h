#ifndef RPulse_h
#define RPulse_h

#include "Arduino.h"
#include "GyverTimers.h"

#define COMMAND_START "p>start" // команда для запуска эксперимента

class RPulse
{
public:
    RPulse();                // конструктор
    void init(int baudRate); // открываем последовательный порт
    void wait();             // ждём получения сообщения P>start;

private:
    static String charArrayToString(char *a, int size);
};

#endif
