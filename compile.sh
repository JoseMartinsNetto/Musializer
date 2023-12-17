#!/bin/sh

g++ main.cpp -o musializer -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

./musializer
