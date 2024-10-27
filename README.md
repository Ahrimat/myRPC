# myRPC

## Описание проекта

Проект `myRPC` включает в себя консольную утилиту `myRPC-client` и программу-демона `myRPC-server`. Он позволяет выполнять команды bash удалённо через сокеты. Проект использует библиотеку `json-c` для работы с JSON и библиотеку `libmysyslog` для логирования.

## Структура проекта
myRPC/
├── client/
│   ├── myRPC-client.c
│   ├── Makefile
│   └── readme.md
├── server/
│   ├── myRPC-server.c
│   ├── Makefile
│   └── readme.md
├── config/
│   ├── myRPC.conf
│   └── users.conf
├── libmysyslog/
│   ├── mysyslog.c
│   ├── mysyslog.h
│   ├── Makefile
│   └── readme.md
├── Makefile
└── readme.md