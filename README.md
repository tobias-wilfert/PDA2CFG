# PDA2CFG

[![C/C++ CI (Ubuntu)](https://github.com/tobias-wilfert/PDA2CFG/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/tobias-wilfert/PDA2CFG/actions/workflows/c-cpp.yml) [![CMake (Windows)](https://github.com/tobias-wilfert/PDA2CFG/actions/workflows/cmake.yml/badge.svg)](https://github.com/tobias-wilfert/PDA2CFG/actions/workflows/cmake.yml)

## Description:

A C++ implementation of the PDA to CFG conversion that will generate a JSON representation of the CFG.

## How to use:

A script ```compile.sh``` is provided to build the PDA2CFG aswell as a script ```test.sh``` to test the PDA2CFG.

The paramter of the program should be the path to the PDA to use in form of a json file (for an example see ```./InputAndOutput/ExamplePDA.json```).

For example outputs see ```./InputAndOutput``` these were generated using the ```test.sh``` script.
