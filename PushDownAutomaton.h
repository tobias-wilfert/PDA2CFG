//============================================================================
// Name        : PushDownAutomaton.h
// Author      : Tobias Wilfert
//============================================================================

#ifndef PDA2CFG__PUSHDOWNAUTOMATON_H_
#define PDA2CFG__PUSHDOWNAUTOMATON_H_

#include <set>
#include <map>
#include <string>
#include <utility>
#include <unordered_set>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

/// The namespace used by the DPA to CFG conversion algorithm
namespace DPA2CFG {

/// Representation of an alphabet for the PDA
using Alphabet = std::unordered_set<std::string>;

/// A struct representing a state in the PDA
struct State {
 public:
  /// The name of the state
  std::string name;

  /// Construct a State with a specified name
  explicit State(std::string name);
};

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

 public:
  /**
   * Construct the PDA from a json containing a representation of the PDA
   * @param j A json containg the representation of the PDA
   */
  explicit PushDownAutomaton(json j);


};

} //namespace DPA2CFG

#endif//PDA2CFG__PUSHDOWNAUTOMATON_H_