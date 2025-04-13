#include "nnue.h"

#include <fstream>
#include <stdexcept>

#include "../../libs/json.hpp"

namespace Leslie::nnue {

Leslie::nnue::Nnue::Nnue(Position& position_) : position_(position_) {
  accumulator_weights_.resize(INPUT_SIZE, HL_SIZE);

  try {
    const std::string coeffs_file = "path";  // TODO

    if (coeffs_file.empty()) {
      throw std::invalid_argument("Invalid NNUE coefficients file path");
    }

    std::ifstream file(coeffs_file);
    if (!file.is_open()) {
      throw std::runtime_error("NNUE coefficients file not found: " + coeffs_file);
    }

    nlohmann::json data;
    file >> data;

    if (!data.contains("A") || !data.contains("B") || !data.contains("C_1") ||
        !data.contains("C_2") || !data.contains("d")) {
      throw std::runtime_error("Missing required NNUE coefficients in file");
    }

    std::vector<std::vector<int16_t>> A = data["A"].get<std::vector<std::vector<int16_t>>>();
    for (int i = 0; i < INPUT_SIZE; ++i) {
      for (int j = 0; j < HL_SIZE; ++j) {
        accumulator_weights_(i, j) = A[i][j];
      }
    }

    accumulator_biases_ =
        Eigen::Map<Eigen::Vector<int16_t, HL_SIZE>>(data["B"].get<std::vector<int16_t>>().data());
    white_output_weights_ =
        Eigen::Map<Eigen::Vector<int16_t, HL_SIZE>>(data["C_1"].get<std::vector<int16_t>>().data());
    black_output_weights_ =
        Eigen::Map<Eigen::Vector<int16_t, HL_SIZE>>(data["C_2"].get<std::vector<int16_t>>().data());
    output_bias_ = data["d"].get<int16_t>();
  } catch (const std::exception& e) {
    // std::cerr << "Failed to read coefficients: " << e.what() << std::endl;
    throw std::runtime_error("Failed to load NNUE coefficients");
  }
}

int16_t Leslie::nnue::Nnue::Eval(Leslie::Position& position) {
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

  if (position_.GetMyColor() == WHITE)
    estimation_ = white_output_weights_.transpose() * y_ + output_bias_;
  else
    estimation_ = black_output_weights_.transpose() * y_ + output_bias_;
  return estimation_;
}

int16_t Leslie::nnue::Nnue::Update(Leslie::Move& move) {
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
      int index = 64 * 6 * ColorToInt(piece.color) + 64 * (TypeToInt(piece.type) - 1) + i;
      result[index] = 1;
    }
  }
  return result;
}

int Nnue::ColorToInt(Color color) { return color == WHITE ? 1 : 0; }

int Nnue::TypeToInt(PieceType type) {
  if (type == PAWN) return 1;
  if (type == KNIGHT) return 2;
  if (type == BISHOP) return 3;
  if (type == ROOK) return 4;
  if (type == QUEEN) return 5;
  if (type == KING) return 6;
  return -1;
}

}  // namespace Leslie::nnue
