#ifndef LESLIE_NETWORK_H_
#define LESLIE_NETWORK_H_

#include <Eigen>
#include <cstdint>

#include "position.h"
#define INPUT_SIZE 768
#define HL_SIZE 2048

namespace leslie::nnue {

class network {
  Eigen::Matrix<int16_t, Eigen::Dynamic, Eigen::Dynamic> accumulator_weights_;
  Eigen::Vector<int16_t, HL_SIZE> accumulator_biases_;
  Eigen::Vector<int16_t, HL_SIZE> output_weights_;  // TODO 2 * HL_SIZE
  Eigen::Vector<int16_t, HL_SIZE> y_;
  int16_t output_bias_;
  Position& position_;
  int16_t estimation_;

 public:
  network(Position& position_);
  int16_t Eval(Position& position);
  int16_t Eval();
  int16_t Update(Move& move);

 private:
  Eigen::Vector<int16_t, INPUT_SIZE> InputVector();
};

}  // namespace leslie::nnue

#endif  // LESLIE_NETWORK_H_
