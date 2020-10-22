//============================================================================
// Name        : PushDownAutomaton.cpp
// Author      : Tobias Wilfert
//============================================================================

#include "PushDownAutomaton.h"

DPA2CFG::State::State(std::string name) : name(std::move(name)) {}

DPA2CFG::State::State() = default;

DPA2CFG::PushDownAutomaton::PushDownAutomaton(json j) {
  startState = State(j["StartState"]);
  stackStartSymbol = j["StartStack"];
  inputAlphabet = j["Alphabet"].get<std::unordered_set<std::string>>();
  stackAlphabet = j["StackAlphabet"].get<std::unordered_set<std::string>>();

  for(auto& s: j["States"]){ states.insert(State(s));}
  for(auto& s: j["FinalStates"]){ acceptingStates.insert(State(s));}
  for (auto &element : j["Transitions"]) {
    transitions[{State(element["from"]), element["input"], element["stacktop"]}]
        .push_back({State(element["to"]),
                    element["replacement"].get<std::vector<std::string>>()});
  }
}

// TODO Work here
/*
DPA2CFG::ContextFreeGrammar DPA2CFG::PushDownAutomaton::convertPDAtoCFG() {

}
*/