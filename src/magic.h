#ifndef LESLIE_MAGIC_H_
#define LESLIE_MAGIC_H_

#include <immintrin.h>

#include <bit>
#include <unordered_map>

#include "types.h"

namespace leslie {

static BitboardType Rays[8][64];


// inline static void init_rays() {
//   for (int center_x = 0; center_x < 8; ++center_x) {
//     for (int center_y = 0; center_y < 8; ++center_y) {
//       int i = center_x + center_y * 8;
//       const BitboardType center = 1ull << i;
//       BitboardType up = 0ull;
//       BitboardType right = 0ull;
//       BitboardType down = 0ull;
//       BitboardType left = 0ull;
//       BitboardType up_right = 0ull;
//       BitboardType up_left = 0ull;
//       BitboardType down_right = 0ull;
//       BitboardType down_left = 0ull;
//
//       for (int sq_x = 0; sq_x < 8; ++sq_x) {
//         for (int sq_y = 0; sq_y < 8; ++sq_y) {
//           BitboardType sq = 1ull << (sq_x + sq_y * 8);
//           const bool lower = sq < center;
//           const bool bigger = sq > center;
//
//           if (center_x == sq_x) {
//             if (lower)
//               down |= sq;
//             else if (bigger)
//               up |= sq;
//           }
//           if (center_y == sq_y) {
//             if (lower)
//               right |= sq;
//             else if (bigger)
//               left |= sq;
//           }
//           if (sq_y == sq_x + (center_y - center_x)) {
//             if (lower)
//               down_right |= sq;
//             else if (bigger)
//               up_left |= sq;
//           }
//           if (sq_y == -sq_x + (center_y + center_x)) {
//             if (lower)
//               down_left |= sq;
//             else if (bigger)
//               up_right |= sq;
//           }
//         }
//       }
//
//       Rays[Direction::kUp][i] = up;
//       Rays[Direction::kRight][i] = right;
//       Rays[Direction::kDown][i] = down;
//       Rays[Direction::kLeft][i] = left;
//       Rays[Direction::kUpRight][i] = up_right;
//       Rays[Direction::kUpLeft][i] = up_left;
//       Rays[Direction::kDownRight][i] = down_right;
//       Rays[Direction::kDownLeft][i] = down_left;
//       RookMasks[i] = up | right | down | left;
//       BishopMasks[i] = up_right | up_left | down_right | down_left;
//     }
//   }
// }
}  // namespace Leslie

#endif
