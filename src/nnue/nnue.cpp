#include "nnue.h"

namespace leslie::nnue {

leslie::nnue::Nnue::Nnue(Position& position_) : position_(position_) {
  accumulator_weights_.resize(INPUT_SIZE, HL_SIZE);
  // TODO read coeff
}

int16_t leslie::nnue::Nnue::Eval(leslie::Position& position) {
  position_ = position;

  // TODO ReLU
  y_ = accumulator_weights_ * InputVector() +
       accumulator_biases_;
  // TODO check turn for ow
  estimation_ = white_output_weights_.transpose() * y_ + output_bias_;
  return estimation_;
}

int16_t leslie::nnue::Nnue::Update(leslie::Move& move) {
  // y_ -четотам + четотам
  // estimation_ = четотам
  return estimation_;
}

int16_t Nnue::Eval() { return estimation_; }

Eigen::Vector<int16_t, INPUT_SIZE> Nnue::InputVector() {
  // TODO
  Eigen::Vector<int16_t, INPUT_SIZE> result;
  result.setZero();
  return result;
}
int16_t Nnue::CReLu(int16_t i, int16_t min, int16_t max) {
  return std::max(min, std::min(i, max));
}

}  // namespace leslie::nnue
