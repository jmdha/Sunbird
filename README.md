<div align="center">

<h1>Sunbird</h1>

<img src="https://github.com/jamadaha/Sunbird/actions/workflows/ctest.yml/badge.svg">
<img src="https://img.shields.io/github/commits-since/jamadaha/Sunbird/latest">

[![lichess-bullet](https://lichess-shield.vercel.app/api?username=sun_bird&format=bullet)](https://lichess.org/@/sun_bird/perf/bullet)
[![lichess-blitz](https://lichess-shield.vercel.app/api?username=sun_bird&format=blitz)](https://lichess.org/@/sun_bird/perf/blitz)
[![lichess-rapid](https://lichess-shield.vercel.app/api?username=sun_bird&format=rapid)](https://lichess.org/@/sun_bord/perf/rapid)

</div>

A UCI chess engine written in CPP.

Play against it [here](https://lichess.org/@/sun_bird).

## Table of Contents
1. [Features](#Features)
2. [Benchmark](#Benchmark)
3. [How to Build](#How-To-Build)
4. [Comparison](#Comparison)

## Features
### State
* 16 bit move [[wiki](https://www.chessprogramming.org/Encoding_Moves)]
* 80 byte state (I.e. pieces on board, castling rights, EP rights, and hash)
  * Bitboards for pieces [[wiki](https://www.chessprogramming.org/Bitboard_Board-Definition)]
  * Zobrist hash [[wiki](https://www.chessprogramming.org/Zobrist_Hashing)]
* Make/unmake move through stack [[wiki](https://www.chessprogramming.org/Board_Representation)]
  * Make move clones state, then pushes modified state to stack
  * Unmake simply pops from stack
* Custom move generation
  * Moves for sliding pieces are generated in a piecewise manner from a series of expanding rings.

### Search
* Negamax [[wiki](https://www.chessprogramming.org/Negamax)]
* Alpha-Beta Pruning [[wiki](https://www.chessprogramming.org/Alpha-Beta)]
* Iterative Deepening [[wiki](https://www.chessprogramming.org/Iterative_Deepening)]
* Quiescence Search [[wiki](https://www.chessprogramming.org/Quiescence_Search)]
* Move Ordering
  * MVV-LVA [[wiki](https://www.chessprogramming.org/MVV-LVA)]
  * PV [[wiki](https://www.chessprogramming.org/PV-Move)]
* Transposition Table [[wiki](https://www.chessprogramming.org/Transposition_Table)]
    
### Evaluation
* Material Point Value [[wiki](https://www.chessprogramming.org/Material)]
* Piece-Square Tables [[wiki](https://www.chessprogramming.org/Piece-Square_Tables)]
* Tapered by game phase [[wiki](https://www.chessprogramming.org/Tapered_Eval)]

## Benchmark
These are run on a i5-13600k with a single thread.

For the purposes here, a "node" is a move done.

### Perft
**52m nodes/s**

### Search
**10m nodes/s**
## How to Build
Navigate to root folder of project, then run the following to build engine.
```
mkdir build &&\
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DIGNORE_TEST=true &&\
cmake --build build
```
The executable can then be found in `chess_engine/build/engine/`.

## Comparison
The result of games played by the rules specified in [CCRL(blitz)](http://computerchess.org.uk/ccrl/404/). 

Ratings are calculated by [BaysianElo](https://github.com/ddugovic/BayesianElo).
| Rank Name         | Elo  | +   | -   | games | score | oppo. | draws |
|-------------------|------|-----|-----|-------|-------|-------|-------|
| 1 Sunbird_v1.2.2  | 181  | 79  | 66  | 52    | 70%   | 20    | 44%   |
| 2 Sunbird_v1.2.1  | 90   | 71  | 69  | 52    | 54%   | 51    | 46%   |
| 3 Sunbird_v1.2.0  | 48   | 74  | 77  | 52    | 46%   | 62    | 38%   |
| 4 Sunbird_v1.1.2  | -55  | 275 | 172 | 12    | 67%   | -187  | 17%   |
| 5 Sunbird_v1.1.0  | -58  | 268 | 186 | 12    | 63%   | -159  | 8%    |
| 6 Sunbird_v1.1.1  | -113 | 199 | 199 | 12    | 50%   | -125  | 17%   |
| 7 Sunbird_v1.0.6  | -123 | 186 | 268 | 12    | 38%   | -47   | 8%    |
| 8 Sunbird_v1.0.4  | -233 | 178 | 368 | 12    | 21%   | -13   | 8%    |
| 9 Sunbird_v1.0.5  | -284 | 178 | 368 | 12    | 21%   | -64   | 8%    |
| 10 Sunbird_v1.0.0 | -517 | 198 | 55  | 12    | 4%    | 16    | 8%    |
