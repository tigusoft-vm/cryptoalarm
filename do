#!/bin/bash
git submodule update --init --recursive
cd soundsimple/
mkdir -p build/ 
cd build && cmake .. && make run
