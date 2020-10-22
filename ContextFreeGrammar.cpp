//============================================================================
// Name        : ContextFreeGrammar.cpp
// Author      : Tobias Wilfert
//============================================================================

#include "ContextFreeGrammar.h"

void DPA2CFG::Productions::addProduction(
    const std::string &variable,const DPA2CFG::Replacement &replacement) {
  productions[variable].insert(replacement);
}

std::set<DPA2CFG::Replacement> DPA2CFG::Productions::getReplacementsFor(
    const std::string &variable) {
  return productions[variable];
}

DPA2CFG::ContextFreeGrammar::ContextFreeGrammar(const json &j) {
  startSymbol = j["Start"];
  variables = j["Variables"].get<std::unordered_set<std::string>>();
  terminals = j["Terminals"].get<std::unordered_set<std::string>>();
  for (auto &element : j["Productions"]) {
    productions.addProduction(element["head"],
                              element["body"]);
  }
}

DPA2CFG::ContextFreeGrammar::ContextFreeGrammar(
    std::string start_symbol,
    DPA2CFG::Productions productions,
    std::unordered_set<std::string> variables,
    std::unordered_set<std::string> terminals) : startSymbol(std::move(start_symbol)),
                                                        productions(std::move(productions)),
                                                        variables(std::move(variables)),
                                                        terminals(std::move(terminals)) {}

// TODO: Check if this works, as it has changed a little
json DPA2CFG::ContextFreeGrammar::generateJsonRepresentation() const {
  json j{
      {"Start", startSymbol},
      {"Terminals", terminals},
      {"Variables", variables}};

  for (auto& p: productions.productions) {
    for(auto& r: p.second){
      j["Productions"].push_back({{"head", p.first}, {"body", r}});
    }
  }
  return j;
}

void DPA2CFG::ContextFreeGrammar::generateJsonFile(
    const std::string &fileName) const {
    std::ofstream o(fileName + ".json");
    o << std::setw(2) << generateJsonRepresentation() << std::endl;
}
