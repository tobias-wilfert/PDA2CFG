cmake_minimum_required(VERSION 3.17)
project(PDA2CFG)

set(CMAKE_CXX_STANDARD 14)

add_executable(PDA2CFG main.cpp PushDownAutomaton.cpp PushDownAutomaton.h ContextFreeGrammar.cpp ContextFreeGrammar.h)