#ifndef LESLIE_LICHESS_SHELL_H_
#define LESLIE_LICHESS_SHELL_H_

#include "engine.h"

namespace Leslie {

class LichessShell {
 public:
  LichessShell(int argc, char** argv);
  ~LichessShell() = default;

  int Start();

  const Engine::Options& GetOptions() const;
  Engine::Options& GetOptions();

 private:
  Engine engine_;
};

}  // namespace Leslie

#endif
