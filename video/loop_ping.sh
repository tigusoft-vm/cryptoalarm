#!/bin/bash

while true
do
	stat1=`ps aux | grep motion | egrep -v "grep"`
	stat1b=`ps aux | grep motion | wc -l`
	stat2=`cat ~/status`
	
	secfile="/dev/shm/secret1/secret"
	date=$(date +%Y-%m-%d_%H:%M:%S%z) ; salt="$date" ; s=`cat $secfile` ; hash=`echo "$s$salt" | sha1deep -q`; 
	size=$(wc -c $secfile | cut -d" " -f1 )
	stat0=`echo "{$size $salt $hash}"`
	
	
	./lib_sendirc.sh "var/chan1" "ping | $stat0 | $stat1b(-1) processes | $stat1 | $stat2"
	sleep 20

done
