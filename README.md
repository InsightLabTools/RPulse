# RPulse

Библиотека необходима для управления роботом и отслеживания показаний датчиков.

## Установка

### Через менеджер библиотек

Этот способ предпочтительнее, потому что Arduino IDE самостоятельно будет следить за обновлениями библиотеки и предложит их загрузить.

Откройте **Arduino IDE**. Откройте **Менеджер библиотек**. Для этого выберите пункт меню **Инструменты - Управлять библиотеками** или нажмите комбинацию клавиш <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>I</kbd>. В появившемся окне наберите в строке поиска название библиотеки и нажмите кнопку **Установка**. Через некоторое время последняя версии библиотеки будет загружена и установлена.

### Установка с помощью zip-архива

Если на компьютере нет доступа в интернет, нужно использовать этот способ.

По ссылке перейдите на [страницу релиза последней версии](https://github.com/skyfroger/RPlatform/releases/latest). В разделе **Assets** скачайте архив с названием **Source code (zip)**. Файл будет называться **RPulse-[версия].zip**.

> Перед установкой новой версии библиотеки из zip-файла, удалите предыдущую версию!

Откройте **Arduino IDE**. Выберите пункт меню **Скетч - Подключить библиотеку - Добавить .ZIP библиотеку...** . В появившемся диалоговом окне выберите сохранённый ранее архив.

Перезапустите **Arduino IDE**.

Библиотека установлена.

## Примеры скетчей

Примеры можно найти в меню **Файл - Примеры - RPulse**.

## Подключение

Для импорта библиотеки в скетч, нужно добавить в начале следующую строку:

```c++
#include <RPulse.h>
```

Добавить эту строку можно вручную или с помощью меню **Скетч - Подключить библиотеку - RPulse**.

## Документация

