#!/bin/bash
date=$(date +%Y-%m-%d_%H:%M:%S%z)
echo "DETECTED (on $date)" > ~/status
bash lib_event.sh "DETECTED" "alertalert"
