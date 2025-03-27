#include "network.h"

namespace leslie::nnue {

leslie::nnue::network::network(Position& position_) : position_(position_) {
  accumulator_weights_.resize(INPUT_SIZE, HL_SIZE);
  // TODO read coeff
}

int16_t leslie::nnue::network::Eval(leslie::Position& position) {
  position_ = position;

  // TODO ReLU
  y_ = InputVector().transpose() * accumulator_weights_ +
       accumulator_biases_.transpose();
  estimation_ = output_weights_.transpose() * y_ + output_bias_;
  return estimation_;
}

int16_t leslie::nnue::network::Update(leslie::Move& move) {
  // y_ -четотам + четотам
  // estimation_ = четотам
  return estimation_;
}

int16_t network::Eval() { return estimation_; }

Eigen::Vector<int16_t, INPUT_SIZE> network::InputVector() {
  // TODO
  Eigen::Vector<int16_t, INPUT_SIZE> result;
  result.setZero();
  return result;
}

}  // namespace leslie::nnue
