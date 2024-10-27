# Makefile

# all - собирает все цели
all: compile

# compile - компилирует клиент
compile:
    gcc -o myRPC-client myRPC-client.c -ljson-c

# clean - удаляет все временные и объектные файлы
clean:
    rm -f myRPC-client

# deb - собирает deb-пакет для программы
deb:
    dpkg-deb --build ./package_dir
