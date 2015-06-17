#!/bin/bash
cmd="./do"
if [[ $0 != $cmd ]]; then 
	echo -e "${w}Wrong path! You should run this script: ./do ${nc}"
	exit 1
fi

head=`pwd`
git submodule update --init --recursive

w="\033[0;33m"
c="\033[1;34m"
nc="\033[0m"

function sound_build() {
	echo -e "${c}Building soundsimple${nc}"
	cd soundsimple/
	mkdir -p build/ 
	cd build && cmake .. && make run
	cd $head
}

function chainsign_build() { 
	echo -e "${c}Building chainsign${nc}"

	cd chainsign/
	cmake . && make
	chain_path=`pwd`
	echo "$chain_path" > chainsign.conf

	# generate config 
	cd $head
}


function handle_param() { 
	if [[ "${1}" == "-h" || "${1}" == "--help" ]] ; then
		echo -e "Build all:\n\t $cmd"
		echo -e "Build video:\n\t $cmd video"
		echo -e "Build soundsimple:\n\t $cmd soundsimple "
		exit 1
	elif [[ "${1}" == "soundsimple" ]]; then
		chainsign_build
		sound_build

	elif [[ "${1}" == "video" ]]; then
		chainsign_build
	fi
}


if [[ $# > 0 ]]; then 
	handle_param ${1}
else 
	echo -e "${c}Build all${nc}"
	chainsign_build
	sound_build
fi
#chainsign_build
#sound_build
