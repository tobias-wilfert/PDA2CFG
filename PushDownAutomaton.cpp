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
        std::cout << ("[" + p + x + q + "]") << std::endl;
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
        productions.addProduction( "[" + std::get<0>(t.first) + std::get<2>(t.first) + std::get<0>(t2) + "]", { std::get<1>(t.first)});
      }else{ // (r,Y1Y2 ... Yk)
        //TODO: These can be quite a lot
        for(auto& r: states){
          std::string reps = "";
          for(auto& sr: std::get<1>(t2)){
            reps += sr;
            // TODO here need to be all permutations od r
            // TODO: Should export the entire logic
          }
          productions.addProduction( "[" + std::get<0>(t.first) + std::get<2>(t.first) + r + "]", { std::get<1>(t.first) + "..." + reps});
          // for the length of Y we need to find combinations of rs
        }
      }
    }
  }


  return ContextFreeGrammar(start_symbol, productions, variables, terminals);
}
