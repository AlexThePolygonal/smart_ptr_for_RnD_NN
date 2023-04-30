#!/bin/bash


g++ -std=c++17 -Werror -Wall -pedantic -O0 -fsanitize=leak main.cpp -o __run__


endl=""
args=""

if test -f __run__
then
echo "It COMPILES! MUAHAHAHAHAHAHA"
chmod 777 __run__
echo "$args"
echo "$endl"
echo "$args" | ./__run__
rm __run__;
fi