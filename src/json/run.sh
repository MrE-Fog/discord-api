#!/bin/bash
icpc -g -Wall -w -O2 -o bin/json json.c
./bin/json
