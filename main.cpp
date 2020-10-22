#include <iostream>

#include "PushDownAutomaton.h"

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    json j;
    std::ifstream ifs(argv[i]);
    ifs >> j;
    DPA2CFG::PushDownAutomaton PDA{j};
    // TODO Do converting and outputting here
    //
    std::cout << "debug" << std::endl;
  }
  return 0;
}
