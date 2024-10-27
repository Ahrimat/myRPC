#include "libmysyslog.h"
#include <stdio.h>
#include <time.h>

// Функция для вывода данных в журнал
int mysyslog(const char* msg, int level, int driver, int format, const char* path)
{
    // Определяем уровень журналирования
    const char* level_str;
    switch (level)
    {
        case DEBUG:
            level_str = "DEBUG";
            break;
        case INFO:
            level_str = "INFO";
            break;
        case WARN:
            level_str = "WARN";
            break;
        case ERROR:
            level_str = "ERROR";
            break;
        case CRITICAL:
            level_str = "CRITICAL";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    // Определяем формат вывода
    const char* format_str;
    switch (format)
    {
        case PLAIN_TEXT:
            format_str = "PLAIN_TEXT";
            break;
        case JSON:
            format_str = "JSON";
            break;
        default:
            format_str = "UNKNOWN";
            break;
    }

    // Определяем место вывода
    FILE* output;
    if (driver == CONSOLE)
    {
        output = stdout;
    }
    else if (driver == FILE_OUTPUT)
    {
        output = fopen(path, "a");
        if (!output)
        {
            perror("Unable to open log file");
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "Unknown driver\n");
        return -1;
    }

    // Получаем текущее время
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Формируем строку для вывода
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);
    
    // Выводим сообщение в журнал
    if (format == PLAIN_TEXT)
    {
        fprintf(output, "[%s] %s: %s\n", timestamp, level_str, msg);
    } else if (format == JSON)
    {
        fprintf(output, "{\"timestamp\": \"%s\", \"level\": \"%s\", \"message\": \"%s\"}\n", timestamp, level_str, msg);
    }

    // Закрываем файл, если он открыт
    if (driver == FILE_OUTPUT)
    {
        fclose(output);
    }

    return 0;
}
