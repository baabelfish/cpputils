#!/bin/bash
clear
cd tests
while true; do
    clear
	clang++ -o ytests *.cpp -std=gnu++11 && clear && ./ytests

    if [[ $(which inotifywait) == "" ]]; then
        sleep 5
    else
        change=$(inotifywait -r -e close_write,moved_to,create . 2> /dev/null)
    fi
done
