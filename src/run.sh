#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/unazed/intel/oneapi/compiler/2021.1.2/linux/compiler/lib/intel64_lin
icpc -g -I ../include -o bin/main main.c discord.c net/websocket.c net/tcp.c net/ssl.c -lssl -lcrypto; ./bin/main "asd"
