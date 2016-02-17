# Soundsimple
### Dependencies
Needed packages for distributions based on Debian. Tested on Debian (Wheezy and Jessie) and Linux Mint 17.1 Rebecca.
``
sudo aptitude install libxcb-image0-dev libx11-xcb-dev libopenal-dev libflac++-dev libvorbis-dev libalut-dev libudev-dev libjpeg-dev libxcb-randr0-dev libfreetype6-dev libfftw3-3 mffm-fftw1  fftw2  fftw3 libfftw3-dev libcrypto++-dev g++ cmake libqt4-opengl-dev libssl-dev

`` 
and boost-dev library, version >= 1.49

### Building 
Run script
``
./do-build
``

### Usage 
After build:
``
~/motion-alert/soundsimple/scripts/run-alarm.sh
``

###  Verify
1) Compare your key and ~/Alarm_data/key_1.pub file
2) run
``
~/motin-alert/soundsimple/scripts/verify.sh
``
3) rm -rf ~/Alarm_data and run again
``
~/motion-alert/soundsimple/scripts/run-alarm.sh
``
or just run above script for continue chain

All above commands run as normal user

### Testing
building working on:
    
    Distributor ID: Ubuntu
    Description:    Ubuntu 15.04
    Release:    15.04
    Codename:   vivid
