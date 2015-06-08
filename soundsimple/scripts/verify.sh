#!/bin/bash
for f in  $(find ~/Alarm_data/ -name '*.sig*' -or -name '*.wav')
	do mv $f ~/Alarm_data/
done