#ifndef LESLIE_NETWORK_H_
#define LESLIE_NETWORK_H_

#include <cstdint>
#define INPUT_SIZE 768
#define HL_SIZE 2048

#define SCALE 400
#define QA 255
#define QB 64

namespace leslie::nnue {

class network {
  int16_t accumulator_weights_[INPUT_SIZE][HL_SIZE];
  int16_t accumulator_biases_[HL_SIZE];
  int16_t output_weights_[2 * HL_SIZE];
  int16_t output_bias_;
};

}  // namespace leslie::nnue

#endif  // LESLIE_NETWORK_H_
