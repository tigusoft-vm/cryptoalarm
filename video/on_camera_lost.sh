#!/bin/bash
echo "X" >  /dev/shm/secret1/secret
echo "CAMERA-LOST" > ~/status
bash lib_event.sh "SENSOR LOST" "alertalert2"
