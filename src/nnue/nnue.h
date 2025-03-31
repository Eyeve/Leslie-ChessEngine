#ifndef LESLIE_NNUE_H_
#define LESLIE_NNUE_H_

#include <Eigen>
#include <cstdint>

#include "position.h"

#define INPUT_SIZE 768
#define HL_SIZE 2048

namespace leslie::nnue {

class Nnue {

 public:
  Nnue(Position& position_);
  int16_t Eval(Position& position);
  int16_t Eval();
  int16_t Update(Move& move);

 private:
  Eigen::Matrix<int16_t, Eigen::Dynamic, Eigen::Dynamic> accumulator_weights_;
  Eigen::Vector<int16_t, HL_SIZE> accumulator_biases_;
  Eigen::Vector<int16_t, HL_SIZE> white_output_weights_;
  Eigen::Vector<int16_t, HL_SIZE> black_output_weights_;
  Eigen::Vector<int16_t, HL_SIZE> y_;
  int16_t output_bias_;
  Position& position_;
  int16_t estimation_;

  Eigen::Vector<int16_t, INPUT_SIZE> InputVector();
  int16_t CReLu(int16_t i, int16_t min, int16_t max);
};

}  // namespace leslie::nnue

#endif
