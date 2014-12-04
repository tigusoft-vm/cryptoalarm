#!/bin/bash 


# usage ./send <message> 


source $HOME/.alarm_config/XMPP_accounts.conf

function sendXMPP() {
    acc=$1
    message=$2
    echo "$message"  | sendxmpp -s $acc
}

function sendXMPPInLoop() { 
    mess=$1
    for acc in ${accounts[*]}
    do
        sendXMPP $acc $mess
        printf "sending to   %s\n" $acc
    done 

}

