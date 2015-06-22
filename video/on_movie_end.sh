#!/bin/bash
#$1 = video file

head=`pwd`

echo "EVENTSCRIPT: $0... will sign the file. head=$head"

currpwd="$PWD"
filepwd="$currpwd"
cd ../chainsign
echo "send SIGN-NEXTKEY"
./send_msg/send_msg fifo SIGN-NEXTKEY
echo "send $1"
./send_msg/send_msg fifo "$1"
cd "$currpwd"

bash lib_event.sh "recdone" "info"

