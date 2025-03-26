#include "network.h"

namespace leslie::nnue {

leslie::nnue::network::network(Position& position_) : position_(position_) {
  // TODO read kef
}

int16_t leslie::nnue::network::Eval(leslie::Position& position) {
  // math math math

  return estimation_;
}

int16_t leslie::nnue::network::Update(leslie::Move& move) {
  // y_ -четотам + четотам
  // estimation_ = четотам
  return estimation_;
}

}  // namespace leslie::nnue
