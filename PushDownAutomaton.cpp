//============================================================================
// Name        : PushDownAutomaton.cpp
// Author      : Tobias Wilfert
//============================================================================

#include "PushDownAutomaton.h"

DPA2CFG::State::State(std::string name) : name(std::move(name)) {}
