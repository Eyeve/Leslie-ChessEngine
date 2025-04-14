#ifndef LESLIE_NNUE_H_
#define LESLIE_NNUE_H_

#include <Eigen>
#include <cstdint>

#include "position.h"

#define INPUT_SIZE 768
#define HL_SIZE 2048
#define CRELU_BOT 0
#define CRELU_TOP 127

namespace Leslie::NNUE {

class NNUE {
 public:
  explicit NNUE(Position& position_);
  EstimationType Eval(Position& position);
  EstimationType Eval();
  EstimationType Update(Move& move);

 private:
  Eigen::Matrix<int16_t, Eigen::Dynamic, Eigen::Dynamic> accumulator_weights_;
  Eigen::Vector<int16_t, HL_SIZE> accumulator_biases_;
  Eigen::Vector<int16_t, HL_SIZE> white_output_weights_;
  Eigen::Vector<int16_t, HL_SIZE> black_output_weights_;
  Eigen::Vector<int16_t, HL_SIZE> y_;
  EstimationType output_bias_;
  Position& position_;
  EstimationType estimation_;

  Eigen::Vector<int16_t, INPUT_SIZE> InputVector();
  int ColorToInt(Color color);
  int TypeToInt(PieceType type);
};

}  // namespace Leslie::NNUE

#endif
