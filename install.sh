#!/bin/bash

touch PID
mkdir -p modules

echo "\nCompiling Core...\n"
cd src/core
make
mv main ../../main

echo "\n\nCompiling Module1...\n"
cd ../manager/modules/module1
make
mv module.so ../../../../modules/module1.so

echo "\n\nCompiling Module2...\n"
cd ../module2
make
mv module.so ../../../../modules/module2.so

#echo "\n\nCompiling Module3...\n"
#cd ../module3
#make
#mv module.so ../../../../modules/module3.so

echo "\n"
