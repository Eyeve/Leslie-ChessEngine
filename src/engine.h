#ifndef LESLIE_ENGINE_H_
#define LESLIE_ENGINE_H_

#include "options.h"
#include "position.h"

namespace leslie {

struct Magic {
  MagicsType rook_magic;
  MagicsType bishop_magic;
};

struct Masks {
  MasksType rook_masks;
  MasksType bishop_masks;
};

class Engine {
 public:
  static Engine& Instance();

  // call to start searching moves
  void Go();
  // call to stop searching moves
  void Stop();

  Position& GetPosition();
  const Position& GetPosition() const;

  Masks& GetMasks();
  const Masks& GetMasks() const;

  Magic& GetMagic();
  const Magic& GetMagic() const;

  void SetPosition(const std::string& fen);

 private:
  Position position_;
  Options options_;
  Magic magic_;
  Masks masks_;
  RaysType rays_;

  Engine();
  ~Engine();

  void InitMagic();
  void InitMasks();

  void InitPieceMagic(const MasksType& masks, MagicsType& magic, Direction d1, Direction d2,
                      Direction d3, Direction d4);
  BitboardType RayTracing(BitboardType blockers, Direction direction,
                          int sq_index);
};

}  // namespace leslie

#endif
