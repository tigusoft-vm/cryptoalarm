#!/bin/bash 


# usage ./send <message> 

message=$1 

echo "$1"  | sendxmpp -s alarm happuri@mempo.org  
echo "$1"  | sendxmpp -s alarm abbys@dukgo.com  
echo "$1"  | sendxmpp -s alarm rfree.m@mempo.org  
echo "$1"  | sendxmpp -s alarm rfree@mempo.org  
echo "$1"  | sendxmpp -s alarm rfree.mobile@jit.si  
