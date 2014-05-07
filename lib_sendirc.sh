#!/bin/bash

chanfile=$1
shift

msg="$@"
# msg=`echo "$msg_notprocessed" | sed -e 's/ / /g'`
echo "$msg" > "$chanfile"
# "/tmp/ii2/192.168.5.7/#tigu/in"

