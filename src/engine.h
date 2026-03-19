#ifndef LESLIE_ENGINE_H_
#define LESLIE_ENGINE_H_

#include <cstddef>
#include <string>

#include "move_table.h"
#include "position.h"

namespace Leslie {

struct Masks {
  MasksType rook_masks{};
  MasksType bishop_masks{};
};

struct Magic {
  MagicsType rook_magic{};
  MagicsType bishop_magic{};
};

class Engine {
 public:
  struct Options {
    std::size_t thread_count = 1;
    std::size_t max_depth = 1;
  };

  static Engine& Instance();

  Engine();
  explicit Engine(const Options& options);

  void Go();
  void Stop();

  Position& GetPosition();
  const Position& GetPosition() const;

  void SetPosition(const std::string& fen);

  const Options& GetOptions() const;
  Options& GetOptions();

  const Masks& GetMasks() const;
  const Magic& GetMagic() const;

 private:
  Position position_;
  Options options_;
  Masks masks_;
  Magic magic_;
};

}  // namespace Leslie

#endif
