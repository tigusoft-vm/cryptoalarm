#!/bin/bash 


# usage ./send <message> 

message=$1 

m="$1 alertalert2-snd"
echo "$m"  | sendxmpp -s alarm happuri@mempo.org  
echo "$m"  | sendxmpp -s alarm abbys@dukgo.com  
echo "$m"  | sendxmpp -s alarm rfree.m@mempo.org  
echo "$m"  | sendxmpp -s alarm rfree.w@mempo.org  
echo "$m"  | sendxmpp -s alarm rfree@mempo.org  
echo "$m"  | sendxmpp -s alarm rfree.mobile@jit.si  
