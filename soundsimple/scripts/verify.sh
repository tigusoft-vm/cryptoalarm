#!/bin/bash
for f in  $(find ~/Alarm_data/ -name '*.sig*' -or -name '*.wav' -or -name '*.pub')
	do cp $f ~/Alarm_data/
done