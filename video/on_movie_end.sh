#!/bin/bash
#$1 = video file
head=`pwd`
cd ../chainsign
./sign_file.sh $1
mv *.pub "$head/rec"
mv *.sig "$head/rec"

bash lib_event.sh "recdone" "info"