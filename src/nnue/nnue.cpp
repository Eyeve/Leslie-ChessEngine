#include "nnue.h"

namespace leslie::nnue {

leslie::nnue::Nnue::Nnue(Position& position_) : position_(position_) {
  accumulator_weights_.resize(INPUT_SIZE, HL_SIZE);
  // TODO read coeff
}

int16_t leslie::nnue::Nnue::Eval(leslie::Position& position) {
  position_ = position;

  y_ = accumulator_weights_ * InputVector() + accumulator_biases_;

  // maybe casts to Eigen::Array and back ya hz
  // cwiseMax cwiseMin ваще int возвращают и это я у инта вызывал cwiseMin ахах
  // крч не строка а бред но мб сработает
  y_ = y_.cwiseMax(CRELU_BOT).cwiseMin(CRELU_TOP);

  // лучше так
  // no casts
  //  y_ = y_.unaryExpr([](int16_t x) {
  //    return std::clamp<int16_t>(x, CRELU_BOT, CRELU_TOP);
  //  });

  if (position_.GetMyColor() == Color::kWhite)
    estimation_ = white_output_weights_.transpose() * y_ + output_bias_;
  else
    estimation_ = black_output_weights_.transpose() * y_ + output_bias_;
  return estimation_;
}

int16_t leslie::nnue::Nnue::Update(leslie::Move& move) {
  // y_ -четотам + четотам
  // estimation_ = четотам
  return estimation_;
}

int16_t Nnue::Eval() { return estimation_; }

Eigen::Vector<int16_t, INPUT_SIZE> Nnue::InputVector() {
  Eigen::Vector<int16_t, INPUT_SIZE> result;
  result.setZero();
  for (int i = 0; i < 64; i++) {
    BitboardType bb = 1ull << (7 - (i % 8));
    bb <<= i - (i % 8);

    Piece piece = position_.WhatPieceOnSquare(bb);
    if (TypeToInt(piece.type) > 0) {
      int index = 64 * 6 * ColorToInt(piece.color) +
                  64 * (TypeToInt(piece.type) - 1) + i;
      result[index] = 1;
    }
  }
  return result;
}

int Nnue::ColorToInt(Color color) { return color == Color::kWhite ? 1 : 0; }

int Nnue::TypeToInt(PieceType type) {
  if (type == PieceType::kPawn) return 1;
  if (type == PieceType::kKnight) return 2;
  if (type == PieceType::kBishop) return 3;
  if (type == PieceType::kRook) return 4;
  if (type == PieceType::kQueen) return 5;
  if (type == PieceType::kKing) return 6;
  return -1;
}

}  // namespace leslie::nnue
