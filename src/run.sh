#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/unazed/intel/oneapi/compiler/2021.1.2/linux/compiler/lib/intel64_lin
icpc -O2 -Wall -Werror=implicit-function-declaration -D_FORTIFY_SOURCE=2 -g         \
      -fstack-protector-all -fasynchronous-unwind-tables -I ../include -o bin/main  \
      main.c dict.c discord.c websocket_util.c base64.c sha1.c                      \
      net/websocket.c net/tcp.c net/ssl.c                                           \
        -lssl -lcrypto
./bin/main "asd"
