#!/bin/bash
# 
# File:   fakemake.bash
# Author: majo32
#
# Created on Jun 7, 2016, 7:48:48 PM
#

set -x #echo on

sources=$(for file in `find ./lib ./src -name '*.cpp'`;do echo $file; done)


cc="arm-phytec-linux-gnueabi-g++"

flags="-Wall -std=c++11 -pthread"
libs="-I./lib -I./src"
rm -rf bin/objects/*
rm -rf bin/tests/*

$cc $sources debug/main.cpp $libs $flags -o main