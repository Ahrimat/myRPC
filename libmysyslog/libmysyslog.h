#ifndef LIBMYSYSLOG_H
#define LIBMYSYSLOG_H

// Уровни журналирования
#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3
#define CRITICAL 4

// Драйверы вывода
#define CONSOLE 0
#define FILE_OUTPUT 1

// Форматы вывода
#define PLAIN_TEXT 0
#define JSON 1

// Функция для вывода данных в журнал
int mysyslog(const char* msg, int level, int driver, int format, const char* path);

#endif /* LIBMYSYSLOG_H */
