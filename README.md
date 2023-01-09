# chess_engine
In short, this project implements a negamax chess engine in cpp.

## Setup
This project has no dependencies, as such simply compile with cmake and then run.

The following commands compiles in debug mode and runs main.

```
mkdir build
cd build
cmake ..
cmake --build .
./main
```

## How it Works
### Board Representation
The engine uses a piece centric board representation, I.e. storing where pieces are, as opposed to what is on squares. This is done through multiple sets of [bitboards](https://en.wikipedia.org/wiki/Bitboard). Where each bitboard stores a binary value for each square, e.g. if pawn on a1, then bitboard equal ...1.

It stores a bitboard for each type of piece, which each represent whether that type is on any given square. Combined with another two bitboards — one for each color — allows the intersection thereof, to give what pieces are on the board of a specific type and color. As an example, taking the intersection of the pawn bitboard and white bitboard, gives the position of all white pawns. These are what constitutes all information for a given board state, but a couple of auxillary bitboards are added.

However, there is also stored information about the game state. That could be whether En Passant or castling is legal.

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/classes/headers/board.hh)

### Move Representation
A move is a 32 bit number, which is split into multiple chunks. Each chunk represents some piece of information, may it be the what square is moved from, or what piece is captured.

The chunks are described below:

[1-4] 4 bit: Move type - Whether it is a quiet move, capture, castle, etc

(4-12] 8 bit: From Square - What square is moved from

(12-20] 8 bit: To Square - What square is moved to

(20-24] 4 bit: Piece Type - What type of piece is moved

(24-28] 4 bit: Disables Castling - Whether the given move disables castling, and which it does, i.e. white/black king/queen castle

(28-32] 4 bit: Disables En Passant - Whether the given move is made when En Passant is legal, and what row it was

() = Not including given bit -> (0-4) = 1,2,3

[] = Includes given bit -> [0-4] = 0,1,2,3,4

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/classes/headers/move.hh)

### Move Generation
TBD

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/classes/move_gen.cpp)

### Search
TBD

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/classes/minimax.cpp)

### Evaluation
TBD

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/classes/evaluator.cpp)
