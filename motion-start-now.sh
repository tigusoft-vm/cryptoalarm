#!/bin/bash
date=$(date +%Y-%m-%d_%H:%M:%S%z)

echo "guarding-since-$date[$1]" > ~/stat

./lib_sendxmpp.sh "rfree.m@mempo.org" "starting...GO"
sleep 3
motion
./lib_sendxmpp.sh "rfree.m@mempo.org" "end of program"

