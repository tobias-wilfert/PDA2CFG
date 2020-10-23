//============================================================================
// Name        : PushDownAutomaton.h
// Author      : Tobias Wilfert
//============================================================================

#ifndef PDA2CFG__PUSHDOWNAUTOMATON_H_
#define PDA2CFG__PUSHDOWNAUTOMATON_H_

#include <set>
#include <map>
#include <tuple>
#include <string>
#include <utility>
#include <iostream>
#include <unordered_set>

#include "ContextFreeGrammar.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

/// The namespace used by the DPA to CFG conversion algorithm
namespace DPA2CFG {

/// Representation of an alphabet for the PDA
using Alphabet = std::unordered_set<std::string>;
/// Representation of a state in a PDA
using State = std::string;

/// A Class that represents a push down automata (PDA)
class PushDownAutomaton {
 private:
  /// The start state
  State startState;

  /// The finite set of states
  std::set<State> states;

  /// The input alphabet
  Alphabet inputAlphabet;

  /// The stack alphabet
  Alphabet stackAlphabet;

  /// The start symbol for the stack
  std::string stackStartSymbol;

  /// The set of accepting states
  std::set<State> acceptingStates;

  /**
   * The DataStructure representing the Transitions
   * The tuple that is the Key contains 3 items in the following order:
   * 1 - The current state of the PDA
   * 2 - The input that should be used for the transition (epsilon = "")
   * 3 - The top of the stack
   * The the tuple that is the value has consists of a string and set of strings
   * The string represents the state to PDA is moving to using the transition
   * The set of strings represents the replacement of the stack with the top
   * the left most item. (Thus need to be pushed in reverse order)
   */
  std::map<std::tuple<State, std::string, std::string>,
           std::vector<std::tuple<State, std::vector<std::string>>>> transitions;

  /**
   * Get permutation of states
   * @param length The length the permutations should have
   * @return Vector of vectors with size length containing strings
   * representing states
   */
  std::vector<std::vector<std::string>> getStatePermutations(int length)const;

  /**
   * Creat the productions for the CFG representation of the PDA
   * @return All the internal productions of the CFG
   */
  Productions createProductions() const;

  /**
   * Create productions from the new start state to variables [q0Z0p]
   * Where q0 is the old start state Z0 is the stack top and p is a state
   * @param startSymbol The new start symbol
   * @param productions A set of productions that the new productions are
   * inserted in
   */
  void createProductionsFromStartSymbol(
      const std::string& startSymbol, Productions& productions) const;

  /// Create the productions of the for [qXr] -> a
  static void createEpsilonProductions(const std::string& fromState,
                                const std::string& onInput,
                                const std::string& topOfStack,
                                const std::string& toState,
                                Productions& productions) ;

  /// Create all the normal productions for the CFG representation
  void createNormalProductions(const std::string& fromState,
                               const std::string& onInput,
                               const std::string& topOfStack,
                               const std::string& toState,
                               const std::vector<std::string>& stackReplacement,
                               Productions& productions) const;

  /// Create all the variables for the CFG representation of the PDA
  std::unordered_set<std::string> createVariables() const;

 public:
  /**
   * Construct the PDA from a json containing a representation of the PDA
   * @param j A json containg the representation of the PDA
   */
  explicit PushDownAutomaton(json j);

  /// Converts the PDA into a CFG
  ContextFreeGrammar convertPDAtoCFG();
};

} //namespace DPA2CFG

#endif//PDA2CFG__PUSHDOWNAUTOMATON_H_
