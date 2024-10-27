## libmysyslog

### libmysyslog.h

Этот файл содержит объявление функции `mysyslog` и определения констант для уровней журналирования, драйверов и форматов.

Функция `mysyslog` используется для вывода данных в журнал с различными параметрами, такими как сообщение, уровень журналирования, драйвер вывода, формат вывода и путь к файлу журнала.

Константы уровней журналирования:
- `DEBUG`
- `INFO`
- `WARN`
- `ERROR`
- `CRITICAL`

Константы драйверов вывода:
- `CONSOLE`
- `FILE_OUTPUT`

Константы форматов вывода:
- `PLAIN_TEXT`
- `JSON`

### libmysyslog.c

Этот файл содержит реализацию функции `mysyslog`, которая используется для вывода данных в журнал.

Функция `mysyslog` принимает следующие параметры:
- `msg`: Строка с сообщением для записи в журнал.
- `level`: Уровень журналирования (DEBUG, INFO, WARN, ERROR, CRITICAL).
- `driver`: Драйвер вывода (CONSOLE или FILE_OUTPUT).
- `format`: Формат вывода (PLAIN_TEXT или JSON).
- `path`: Путь к файлу журнала (только если `driver` установлен в FILE_OUTPUT).

Пример использования:

```c
#include "libmysyslog.h"

int main()
{
    mysyslog("This is an informational message", INFO, CONSOLE, PLAIN_TEXT, NULL);
    mysyslog("This is an error message", ERROR, FILE_OUTPUT, JSON, "logfile.txt");
    return 0;
}
```