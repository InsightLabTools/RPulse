#ifndef RPulse_h
#define RPulse_h

#include "Arduino.h"
#include "GyverTimers.h"

#define READINGS_RESOLUTION 100000 // период снятия показаний в микросекундах (сейчас 100 мс)
#define MAX_LIST_SIZE 10           // максимальное количество элементов в массивах

// перечисление типов пина
enum PinType
{
    analog,
    digital
};

// перечисление типов переменных, которые можно отслеживать
enum VarType
{
    floatVar,
    intVar
};

// перечисление таймеров
enum Timer
{
    T0,
    T1,
    T2
};

// информация об отслеживаемом пине
struct PinData
{
    unsigned int pin; // номер пина
    String key;       // название
    PinType type;     // тип (аналоговый/цифровой)
};

// информация об отслеживаемой переменной
struct VarData
{
    float *floatValue; // указатель на вещественное значение
    int *intValue;     // указатель на целое значение
    String key;        // название
    VarType type;      // тип (целая/вещественная)
};

// параметр эксперимента, получаемый через последовательный порт
struct Param
{
    float value;
    String key;
};

class RPulse
{
public:
    RPulse();                                         // конструктор
    void init(int baudRate = 9600, Timer timer = T1); // открываем последовательный порт
    void wait();                                      // ждём получения сообщения P>start;
    void start();                                     // запуск отслеживания значений по таймеру
    static void send();                               // отправка отслеживаемых значений
    void watchPin(int pin, PinType type, String key); // отслеживать значение с заданного пина
    void watchVar(int &var, String key);              // отслеживать переменную целого типа
    void watchVar(float &var, String key);            // отслеживать переменную вещественного типа
    float get(String key, float defaultValue = 0.0);  // возвращает параметр, полученный через последовательйны порт
    static Timer currentTimer;                        // используемый таймер

private:
    void parseParams(String);                           // парсинг и сохранение полученных параметров
    static PinData pinList[MAX_LIST_SIZE];              // массив отслеживаемых пинов
    static int pinListSize;                             // количество отслеживаемых пинов
    static VarData varList[MAX_LIST_SIZE];              // массив отслеживаемых переменных
    static int varListSize;                             // количество переменных, за которыми нужно следить
    Param paramsList[MAX_LIST_SIZE];                    // массив полученных параметров
    int paramsListSize;                                 // количество полученных параметров
    static String charArrayToString(char *a, int size); // преобразование массива символов в строку
};

#endif
