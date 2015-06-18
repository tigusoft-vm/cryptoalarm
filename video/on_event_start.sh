#!/bin/bash
echo "EVENT in $0, options are: $*"
echo "EVENT in $1, options are also: $1 $2 $3 $4"

date=$(date +%Y-%m-%d_%H:%M:%S%z)
echo "DETECTED (on $date)" > ~/status
bash lib_event.sh "DETECTED" "alertalert"
