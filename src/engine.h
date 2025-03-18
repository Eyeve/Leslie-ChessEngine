#ifndef LESLIE_ENGINE_H_
#define LESLIE_ENGINE_H_

#include "options.h"
#include "position.h"

namespace leslie {

struct Magic {
  std::unordered_map<MagicKeyType, BitboardType> RookMagic[64];
  std::unordered_map<MagicKeyType, BitboardType> BishopMagic[64];
};

struct Masks {
  BitboardType RookMasks[64];
  BitboardType BishopMasks[64];
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

  Engine();
  ~Engine();

  void InitMagic();
  void InitMasks();
};

}  // namespace leslie

#endif
