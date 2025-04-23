#ifndef LESLIE_ENGINE_H_
#define LESLIE_ENGINE_H_

#include <memory>

#include "position.h"
#include "thread.h"

namespace Leslie {

class Engine {
 public:
  struct Options {
    size_t thread_count;
    size_t max_depth;
  };

  Engine(const Options& options);

  // call to start searching moves
  void Go();
  // call to stop searching moves
  void Stop();

  Position& GetPosition();
  const Position& GetPosition() const;

  void SetPosition(const std::string& fen);

 private:
  Position position_;
  Options options_;
  std::vector<std::unique_ptr<Thread>> threads_;
  float alpha_limit_ = 0.f;
  float beta_limit_ = 0.f;

  void InitMagic();
  void InitMasks();

  void InitPieceMagic(const MasksType& masks, MagicsType& magic, Direction d1, Direction d2,
                      Direction d3, Direction d4) const;
  BitboardType RayTracing(BitboardType blockers, Direction direction, int sq_index) const;
};

}  // namespace Leslie

#endif
