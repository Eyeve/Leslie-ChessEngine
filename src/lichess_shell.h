#ifndef LESLIE_UCI_H_
#define LESLIE_UCI_H_

#include "engine.h"

namespace leslie {

class LichessShell {
 public:
  LichessShell(int argc, char** argv);
  ~LichessShell();

  int Start();

  const Options& GetOptions() const;
  Options& GetOptions();

 private:
  Engine engine_;
};

}  // namespace leslie

#endif
