#ifndef RPulse_h
#define RPulse_h

#include "Arduino.h"
#include "GyverTimers.h"

#define COMMAND_START "p>start"    // команда для запуска эксперимента
#define READINGS_RESULUTION 100000 // период снятия показаний в микросекундах (сейчас 100 мс)
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

class RPulse
{
public:
    RPulse();                // конструктор
    void init(int baudRate); // открываем последовательный порт
    void wait();             // ждём получения сообщения P>start;
    void start();
    static void send();                               // отправка отслеживаемых значений
    void watchPin(int pin, PinType type, String key); // отслеживать значение с заданного пина
    void watchVar(int &var, String key);              // отслеживать переменную целого типа
    void watchVar(float &var, String key);            // отслеживать переменную вещественного типа

private:
    static PinData pinList[MAX_LIST_SIZE];              // массив отслеживаемых пинов
    static int pinListSize;                             // количество отслеживаемых пинов
    static VarData varList[MAX_LIST_SIZE];              // массив отслеживаемых переменных
    static int varListSize;                             // количество переменных, за которыми нужно следить
    static String charArrayToString(char *a, int size); // преобразование массива символов в строку
};

#endif
