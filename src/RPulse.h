#ifndef RPulse_h
#define RPulse_h

#include "Arduino.h"
#include "GyverTimers.h"

#define COMMAND_START "p>start"    // команда для запуска эксперимента
#define READINGS_RESULUTION 200000 // период снятия показаний в микросекундах (сейчас 200 мс)

// перечисление типов пина
enum SensorType
{
    analog,
    digital
};

// информация об отслеживаемом пинет
struct SensorData
{
    int pin;         // номер пина
    String key;      // название
    SensorType type; // тип (аналоговый/цифровой)
};

class RPulse
{
public:
    RPulse();                                               // конструктор
    void init(int baudRate);                                // открываем последовательный порт
    void wait();                                            // ждём получения сообщения P>start;
    static void send();                                     // отправка отслеживаемых значений
    void watchSensor(int pin, SensorType type, String key); // отслеживать значение с заданного пина

private:
    static SensorData sensorsList[10];                  // массив отслеживаемых пинов
    static int sensorsListSize;                         // количество отслеживаемых пинов
    static String charArrayToString(char *a, int size); // преобразование массива символов в строку
};

#endif
