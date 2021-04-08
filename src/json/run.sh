#!/bin/bash
icpc -g -Wall -w -O0 -o bin/json json.c
./bin/json
