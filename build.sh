#!/bin/bash
git submodule update --init --recursive
cd soundsimple/
cmake .
make
