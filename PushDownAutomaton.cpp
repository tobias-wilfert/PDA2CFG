//============================================================================
// Name        : PushDownAutomaton.cpp
// Author      : Tobias Wilfert
//============================================================================

#include "PushDownAutomaton.h"

DPA2CFG::PushDownAutomaton::PushDownAutomaton(json j){

  startState = State(j["StartState"]);
  stackStartSymbol = j["StartStack"];
  inputAlphabet = j["Alphabet"].get<std::unordered_set<std::string>>();
  stackAlphabet = j["StackAlphabet"].get<std::unordered_set<std::string>>();

  for(auto& s: j["States"]){ states.insert(State{s});}
  for(auto& s: j["FinalStates"]){ acceptingStates.insert(State(s));}

  for (auto &element : j["Transitions"]) {
    transitions[{State(element["from"]), element["input"], element["stacktop"]}]
        .push_back({State(element["to"]),
                    element["replacement"].get<std::vector<std::string>>()});
  }
}

// TODO Work here
// TODO: Ask tom if we should filter useless states ?

DPA2CFG::ContextFreeGrammar DPA2CFG::PushDownAutomaton::convertPDAtoCFG() {
  // Need to make a new start symbol
  std::string start_symbol = "_" + startState;
  // Make all the variables
  std::unordered_set<std::string> variables = {start_symbol};
  // TODO DO this differently as it generates vars that might not even be used
  // Rather only add the ones we make up on the go
  for(auto& p: states){
    for(auto& q: states){
      for(auto& x: stackAlphabet){
        variables.insert("[" + p + x + q + "]");
        //std::cout << ("[" + p + x + q + "]") << std::endl;
      }
    }
  }
  // Terminals stay the same
  std::unordered_set<std::string> terminals = inputAlphabet;
  // Make all the productions
  // Productions from the start state
  Productions productions;
  for(auto& p: states){
    productions.addProduction(start_symbol,{"[" + startState+stackStartSymbol + p + "]"});
  }
  // Make the productions from the transitions
  for(auto& t: transitions){
    // Need to loop over the multiple transitions there could be
    for(auto& t2: t.second){
      if(std::get<1>(t2).empty()){ //(r,eps)
        std::vector<std::string> rep = {};
        if(std::get<1>(t.first) != ""){ rep = {std::get<1>(t.first)}; }
        productions.addProduction( "[" + std::get<0>(t.first) + std::get<2>(t.first) + std::get<0>(t2) + "]", rep);
      }else{ // (r,Y1Y2 ... Yk)
        for(auto& r: states){
          for(auto& v: getStatePermutations(std::get<1>(t2).size()-1)){
            std::vector<std::string> rep ={std::get<1>(t.first)};
            if(!v.empty()){
              rep.push_back("[" + std::get<0>(t2) + std::get<1>(t2).at(0) + v.at(0)  + "]");
            }else{
              rep.push_back("[" + std::get<0>(t2) + std::get<1>(t2).at(0) + r  + "]");
            }
            int size = std::get<1>(t2).size()-1;
            for(int i=1; i< size; ++i){
              rep.push_back("[" + v.at(i-1) + std::get<1>(t2).at(i) + v.at(i) + "]");
            }
            if(!v.empty()){
              rep.push_back("[" + v.at(size-1) + std::get<1>(t2).back() +  r + "]");
            }

            // TODO: Some times wrong
            // TODO: If the body between the body is empty insert an empty {} rather then an empty ""
            if(rep.at(0) == ""){
              std::cout << "sad" << std::endl;
            }
            productions.addProduction( "[" + std::get<0>(t.first) + std::get<2>(t.first) + r + "]", rep);
          }
        }
      }
    }
  }
  return ContextFreeGrammar(start_symbol, productions, variables, terminals);
}

std::vector<std::vector<std::string>>
DPA2CFG::PushDownAutomaton::getStatePermutations(int length) const {
  std::vector<std::vector<std::string>> permutations={{}};
  for(int i = 0; i < length; ++i){
    std::vector<std::vector<std::string>> newPermutations;
    for(std::vector<std::string> vec: permutations){
      for(const State& state:states){
        std::vector<std::string> n = vec;
        n.push_back(state);
        newPermutations.push_back(n);
      }
    }
    permutations = newPermutations;
  }
  return permutations;
}
