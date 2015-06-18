#!/bin/bash
#$1 = video file

head=`pwd`

echo "EVENTSCRIPT: $0... will sign the file. head=$head"

currpwd="$PWD"
filepwd="$currpwd"
cd ../chainsign
echo "Calling chainsign - start"
./sign_file.sh "$filepwd/$1"
echo "Calling chainsign - done"
mv *.pub "$head/rec/"
mv *.sig "$head/rec/"
cd "$currpwd"

bash lib_event.sh "recdone" "info"

