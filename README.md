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
**52m nodes/s**

### Search
**10m nodes/s**

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
