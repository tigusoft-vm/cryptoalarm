#!/bin/bash 


# usage ./send <message> 
set -x
via=$1
mess=$2 
file=$3


source $HOME/motion-alert/soundsimple/scripts/sendXMPP.sh
source $HOME/motion-alert/soundsimple/scripts/send-mail.sh

echo $mess

if [[ $via == "xmpp"  ]]; then 
    sendXMPPInLoop "$mess"
elif [[ $via == "mail" ]]; then
    sign $file 
    send "$mess" $file
fi
