#!/bin/bash
date=$(date +%Y-%m-%d_%H:%M:%S%z)

echo "guarding-since-$date[$1]" > ~/stat

./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting..."
sleep 10
./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting..."
sleep 60

sleep 20

./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...3"
sleep 5
./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...2"
sleep 5
./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...1"
sleep 5
./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...GO"
sleep 5
motion
./lib_sendxmpp.sh "rfree.mobile@jit.si" "end of program"

