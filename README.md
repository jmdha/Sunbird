# Sunbird
A UCI chess engine written in CPP.

Play against it [here](https://lichess.org/@/sun_bird).

## Table of Contents
1. [Benchmark](#Benchmark)
2. [Features](#Features)
3. [How to Build](#How-To-Build)

## Benchmark
These are run on a i5-13600k with a single thread.

For the purposes here, a "node" is a move done.

### Perft
**35m nodes/s**

```
32% Chess::Board::UndoMove
32% Chess::Board::DoMove
27% Chess::MoveGen::GenerateMoves
```
As seen, most of the time is spent updating state with each move.

Distribution found with `perf record --call-graph=dwarf ./Benchmark`

### Search
**6m nodes/s**

```
41% Chess::MoveGen::GenerateMoves (Quiesce)
12% Chess::MoveGen::GenerateMoves (Negamax)
21% Chess::Engine::Evaluation::Eval
  12% Chess::Engine::Evaluation::EvalPosition
  9% Chess::Engine::Evaluation::EvalMaterial
8% Chess::Board::DoMove
7% Chess::Board::UndoMove
```
A noticible thing here is the increase in time spent generating moves $27 \rightarrow 53$. This is caused by alpha-beta pruning, as not all generated moves are done, i.e. some are skipped as the branch is pruned. This is the same reason as to why DoMove and UndoMove has a decrease in time.

Distribution found with `perf record --call-graph=dwarf ./engine 7 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"`

## Features
### State
* 16 bit move [[wiki](https://www.chessprogramming.org/Encoding_Moves)]
* Board Representation through Bitboards [[wiki](https://www.chessprogramming.org/Bitboard_Board-Definition)]
### Search
* Negamax [[wiki](https://www.chessprogramming.org/Negamax)]
* Alpha-Beta Pruning [[wiki](https://www.chessprogramming.org/Alpha-Beta)]
* Iterative Deepening [[wiki](https://www.chessprogramming.org/Iterative_Deepening)]
* Quiescence Search [[wiki](https://www.chessprogramming.org/Quiescence_Search)]

### Evaluation
* Material Point Value [[wiki](https://www.chessprogramming.org/Material)]
* Piece-Square Tables [[wiki](https://www.chessprogramming.org/Piece-Square_Tables)]

## How to Build
Navigate to root folder of project, then run the following to build engine.
```
mkdir build &&\
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_BIN=true -DBUILD_ENGINE=true &&\
cmake --build build
```
The executable can then be found in `chess_engine/build/engine/`.
