#!/bin/bash
# Will sign and send message+attachment

echo "====================================================="
echo "====================================================="
echo " THIS FILE IS DEPRECATED " # tego pliku już nie potrzeba? @happuri TODO
echo "====================================================="
echo "====================================================="

# usage ./send <message> 
set -x

via=$1 # set it to "mail" or "xmpp" - which sending mechanism
mess=$2 # the text message
file=$3 # the file that should be attached

source $HOME/motion-alert/soundsimple/scripts/sendXMPP.sh || { echo 'Can not load lib'; exit 100; }
source $HOME/motion-alert/soundsimple/scripts/send-mail.sh || { echo 'Can not load lib'; exit 101; }

echo $mess

if [[ $via == "xmpp"  ]]; then 
    sendXMPPInLoop "$mess"
elif [[ $via == "mail" ]]; then
    sign $file 
    send "$mess" $file
fi
