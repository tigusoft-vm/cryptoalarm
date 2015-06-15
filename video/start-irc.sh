#!/bin/bash
nohup ii -f "$HOSTNAME" -n bot-a1 -i /tmp/irc -s 192.168.5.7 &
sleep 5
echo "/JOIN #tigu" > /tmp/irc/192.168.5.7/in 
echo "/JOIN #stat" > /tmp/irc/192.168.5.7/in 
sleep 2
tail /tmp/irc/192.168.5.7/out
