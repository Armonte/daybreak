#!/usr/bin/bash

# TODO: check if these wgets fail or succeed properly

mkdir -p crescent mbaa

echo "Downloading newLISP into \`crescent/' ..."
wget -q --show-progress -O crescent/newlisp.dll \
    http://www.newlisp.org/downloads/Win32/newlisp.dll

wget -q --show-progress -O crescent/newlisp.exe \
    http://www.newlisp.org/downloads/Win32/newlisp.exe

echo "Downloading MBAACC into \`mbaa/' ..."
wget -q --show-progress -O mbaa/mbaacc.zip https://1g4i.short.gy/mbaacc
cd mbaa && unzip mbaacc.zip && rm -rf mbaacc.zip

echo "newLISP and MBAACC installed successfully!"
