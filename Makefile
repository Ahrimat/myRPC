# Makefile

# all - собирает все цели
all: compile_client compile_server clean deb

# compile_client - компилирует клиент
compile_client:
    gcc -I C:/path/to/json-c/include -I C:/path/to/libmysyslog/include -o client/myRPC-client client/myRPC-client.c -ljson-c -lmysyslog

# compile_server - компилирует сервер
compile_server:
    gcc -I C:/path/to/json-c/include -I C:/path/to/libmysyslog/include -o server/myRPC-server server/myRPC-server.c -ljson-c -lmysyslog

# clean - удаляет все временные и объектные файлы
clean:
    rm -f client/myRPC-client server/myRPC-server

# deb - собирает deb-пакет для программы
deb:
    dpkg-deb --build client/package_dir
    dpkg-deb --build server/package_dir
