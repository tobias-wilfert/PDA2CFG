//============================================================================
// Name        : ContextFreeGrammar.h
// Author      : Tobias Wilfert
//============================================================================

#ifndef PDA2CFG__CONTEXTFREEGRAMMAR_H_
#define PDA2CFG__CONTEXTFREEGRAMMAR_H_

#include <set>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iomanip>
#include <unordered_set>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

/// The namespace used by the DPA to CFG conversion algorithm
namespace DPA2CFG {

/// Forward declaration inorder to make the class a friend of Productions
class ContextFreeGrammar;
/// Representation of a single replacement a variable can have
using Replacement = std::vector<std::string>;

// A struct that represents the productions of a CFG
struct Productions {
  friend ContextFreeGrammar;
 private:
  /**
   * Represents the productions of a CFG
   * Maps replacement to the variables
   * The keys of the map are the variables in the CFG
   * The values are sets of replacements that the variable can have
   */
  std::map<std::string, std::set<Replacement>> productions;

 public:
  /**
   * Add a production to productions
   * @param variable The variable of the production
   * @param replacement The replacement of the production
   */
  void addProduction(const std::string &variable, const Replacement &replacement);

  /**
   * Get the replacements for the variable
   * @param variable The variable the replacements are for
   * @return A set of replacements
   */
  std::set<Replacement> getReplacementsFor(const std::string &variable);
};

/// A Class that represents a context free grammar (CFG)
class ContextFreeGrammar {
 private:
  /// The start symbol
  std::string startSymbol;

  /// The productions
  Productions productions;

  /// The finite set of variables
  std::unordered_set<std::string> variables;

  /// The finite set of terminals
  std::unordered_set<std::string> terminals;

 public:
  /**
   * Initializes the CFG from a json representation of the CFG
   * @param j a json representation of the CFG
   */
  explicit ContextFreeGrammar(const json &j);

  /**
   * Normal constructor of the CFG
   * @param start_symbol The start symbol
   * @param productions The productions
   * @param variables The Variables
   * @param terminals The Terminals
   */
  ContextFreeGrammar(std::string start_symbol,
                     Productions productions,
                     std::unordered_set<std::string> variables,
                     std::unordered_set<std::string> terminals);

  /**
   * Generates a json representation of the CFG
   * @return A json representing the CFG
   */
  json generateJsonRepresentation() const;

  /**
   * Writes the json representation to a json file
   * @param fileName The name of the file to write the json representation to
   */
  void generateJsonFile(const std::string &fileName) const;
};

} //namespace DPA2CFG

#endif//PDA2CFG__CONTEXTFREEGRAMMAR_H_
