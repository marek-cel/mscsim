#!/bin/bash

################################################################################

./cpplint.py --filter=-whitespace/parens,-whitespace/braces --root=./src --extensions=h,cpp ./src/*.cpp

################################################################################

read -p "Press any key to continue... " -n1 -s
