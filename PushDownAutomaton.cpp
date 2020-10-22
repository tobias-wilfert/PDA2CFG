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

DPA2CFG::ContextFreeGrammar DPA2CFG::PushDownAutomaton::convertPDAtoCFG() {
  Productions productions;
  std::string start_symbol = "_" + startState; // Make new start symbol
  std::unordered_set<std::string> terminals = inputAlphabet;
  std::unordered_set<std::string> variables = {start_symbol};

  createProductionsFromStartSymbol(start_symbol, productions,variables);
  createProductions(productions, variables);
  return ContextFreeGrammar(start_symbol, productions, variables, terminals);
}

std::vector<std::vector<std::string>>
DPA2CFG::PushDownAutomaton::getStatePermutations(int length) const {
  std::vector<std::vector<std::string>> permutationsSoFar={{}};
  for(int currentLength = 0; currentLength < length; ++currentLength){
    std::vector<std::vector<std::string>> newPermutations;
    for(const std::vector<std::string>& permutation: permutationsSoFar){
      for(const State& state:states){
        std::vector<std::string> newPermutation = permutation;
        newPermutation.push_back(state);
        newPermutations.push_back(newPermutation);
      }
    }
    permutationsSoFar = newPermutations;
  }
  return permutationsSoFar;
}

void DPA2CFG::PushDownAutomaton::createProductionsFromStartSymbol(
    const std::string &startSymbol, DPA2CFG::Productions &productions,
    std::unordered_set<std::string> &variables) const {
  for(auto& p: states){
    std::string var = "[" + startState+stackStartSymbol + p + "]";
    productions.addProduction(startSymbol,{var});
    variables.insert(var);
  }
}

void DPA2CFG::PushDownAutomaton::createProductions(
    DPA2CFG::Productions &productions,
    std::unordered_set<std::string> &variables) const {

  for(auto& t: transitions){
    for(auto& t2: t.second){
      std::string fromState = std::get<0>(t.first);
      std::string onInput = std::get<1>(t.first);
      std::string topOfStack = std::get<2>(t.first);
      std::string toState = std::get<0>(t2);;
      std::vector<std::string> stackReplacement = std::get<1>(t2);

      if(stackReplacement.empty()){ //(r,eps)
        createEpsilonProductions(
            fromState,onInput,topOfStack,toState,productions,variables);
      }else{ // (r,Y1Y2 ... Yk)
        createNormalProductions(
            fromState,onInput,topOfStack,toState,stackReplacement,
            productions,variables);
      }
    }
  }
}

void DPA2CFG::PushDownAutomaton::createEpsilonProductions(
    const std::string &fromState,
    const std::string &onInput,
    const std::string &topOfStack,
    const std::string &toState,
    DPA2CFG::Productions &productions,
    std::unordered_set<std::string> &variables) const {

  std::vector<std::string> rep = {};
  if(!onInput.empty()){ rep = {onInput};}
  std::string var = "[" + fromState + topOfStack + toState + "]";
  productions.addProduction(var, rep);
  variables.insert(var);
}

void DPA2CFG::PushDownAutomaton::createNormalProductions(
    const std::string &fromState,
    const std::string &onInput,
    const std::string &topOfStack,
    const std::string &toState,
    const std::vector<std::string> &stackReplacement,
    DPA2CFG::Productions &productions,
    std::unordered_set<std::string> &variables) const {

  for(auto& r: states){
    int size = stackReplacement.size()-1;
    for(auto& v: getStatePermutations(size)){
      std::vector<std::string> rep ={onInput};

      if(!v.empty()){ // The head of the replacement
        std::string var = "[" + toState + stackReplacement.at(0) + v.at(0)  + "]";
        rep.push_back(var);
        variables.insert(var);
      }else{
        std::string var = "[" + toState + stackReplacement.at(0) + r + "]";
        rep.push_back(var);
        variables.insert(var);
      }
      for(int i=1; i< size; ++i){ // The main body of the replacement
        std::string var = "[" + v.at(i-1) + stackReplacement.at(i) + v.at(i) + "]";
        rep.push_back(var);
        variables.insert(var);
      }
      if(!v.empty()){ // The end of the replacement
        std::string var = "[" + v.back() + stackReplacement.back() +  r + "]";
        rep.push_back(var);
        variables.insert(var);
      }
      // Conclusion
      std::string var = "[" + fromState + topOfStack + r + "]";
      productions.addProduction(var , rep);
      variables.insert(var);
    }
  }
}
