#!/bin/bash
to=$1
shift
echo " $@" | sendxmpp -v $to
