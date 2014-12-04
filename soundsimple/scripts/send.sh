#!/bin/bash 


# usage ./send <message> 

via=$1
mess=$2 

source sendXMPP.sh

if [[ $via == "xmpp"  ]]; then 
    sendXMPPInLoop $mess
fi
