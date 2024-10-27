# Makefile

# all - собирает все цели
all: compile

# compile - компилирует сервер
compile:
    gcc -o myRPC-server myRPC-server.c -ljson-c -lmysyslog

# clean - удаляет все временные и объектные файлы
clean:
    rm -f myRPC-server

# deb - собирает deb-пакет для программы
deb:
    dpkg-deb --build ./package_dir
