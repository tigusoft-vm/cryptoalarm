#!/bin/bash
for f in  $(find ~/Alarm_data/ -name '*.sig*' -or -name '*.wav' -or -name '*.pub')
	do
	cp $f ~/Alarm_data/
done

cd $HOME/motion-alert/soundsimple/build/src/

for f in  ~/Alarm_data/*.sig
	do
	./simplesound -v "$f"
	if [ "$?" -ne "1" ] ; then
		echo "*********VERIFICATION ERROR*********"
		break
	fi
done
