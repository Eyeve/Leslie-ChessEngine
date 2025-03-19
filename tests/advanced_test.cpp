#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {
TEST(advanced, test1) {
  PositionTest("8/nK6/2Nrpq1P/4P3/3P4/2r5/1P4N1/8 w - - 0 1",
               {
                   {PieceType::kKing,
                    Square::kB7,
                    {Square::kB6, Square::kA6, Square::kA7, Square::kA8,
                     Square::kB8, Square::kC8, Square::kC7}},

                   {PieceType::kKnight,
                    Square::kC6,
                    {Square::kB4, Square::kA5, Square::kA7, Square::kB8,
                     Square::kD8, Square::kE7}},

                   {PieceType::kKnight,
                    Square::kG2,
                    {Square::kE1, Square::kE3, Square::kF4, Square::kH4}},

                   {PieceType::kPawn,
                    Square::kB2,
                    {Square::kB3, Square::kB4, Square::kC3}},

                   {PieceType::kPawn, Square::kD4, {Square::kD5}},
                   {PieceType::kPawn, Square::kE5, {Square::kD6, Square::kF6}},
                   {PieceType::kPawn, Square::kH6, {Square::kH7}},
               });
}

TEST(advanced, test2) {
  PositionTest(
      "8/8/3n4/2Q3b1/8/2p2N2/PP6/4KB2 w - - 0 1",
      {
          {PieceType::kKing,
           Square::kE1,
           {Square::kD1, Square::kD2, Square::kE2, Square::kF2}},
          {PieceType::kKnight,
           Square::kF3,
           {Square::kG1, Square::kH2, Square::kH4, Square::kG5, Square::kE5,
            Square::kD4, Square::kD2}},
          {PieceType::kPawn, Square::kA2, {Square::kA3, Square::kA4}},
          {PieceType::kPawn,
           Square::kB2,
           {Square::kB3, Square::kB4, Square::kC3}},
          {PieceType::kBishop,
           Square::kF1,
           {Square::kG2, Square::kH3, Square::kE2, Square::kD3, Square::kC4,
            Square::kB5, Square::kA6}},
          {PieceType::kQueen,
           Square::kC5,
           {Square::kA5, Square::kB5, Square::kD5, Square::kE5, Square::kF5,
            Square::kG5, Square::kC3, Square::kC4, Square::kC6, Square::kC7,
            Square::kC8, Square::kA7, Square::kB6, Square::kD4, Square::kE3,
            Square::kF2, Square::kG1, Square::kA3, Square::kB4, Square::kD6}},
      });
}

TEST(advanced, test3) {
  PositionTest(
      "rnbqkbnr/pp2pppp/8/3p4/1PpPP3/2P5/P4PPP/RNBQKBNR b KQkq b3 0 4",
      {
          {PieceType::kKnight,
           Square::kB8,
           {Square::kA6, Square::kC6, Square::kD7}},
          {PieceType::kBishop,
           Square::kC8,
           {Square::kD7, Square::kE6, Square::kF5, Square::kG4, Square::kH3}},
          {PieceType::kQueen,
           Square::kD8,
           {Square::kD7, Square::kD6, Square::kC7, Square::kB6, Square::kA5}},
          {PieceType::kKing, Square::kE8, {Square::kD7}},
          {PieceType::kKnight, Square::kG8, {Square::kF6, Square::kH6}},

          {PieceType::kPawn, Square::kA7, {Square::kA6, Square::kA5}},
          {PieceType::kPawn, Square::kB7, {Square::kB6, Square::kB5}},
          {PieceType::kPawn, Square::kC4, {Square::kB3}},
          {PieceType::kPawn, Square::kD5, {Square::kE4}},
          {PieceType::kPawn, Square::kE7, {Square::kE6, Square::kE5}},
          {PieceType::kPawn, Square::kF7, {Square::kF6, Square::kF5}},
          {PieceType::kPawn, Square::kG7, {Square::kG6, Square::kG5}},
          {PieceType::kPawn, Square::kH7, {Square::kH6, Square::kH5}},
      });
}

}  // namespace leslie::test
