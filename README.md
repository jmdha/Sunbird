# chess_engine
## Table of Contents
1. [Board Representation](#board-representation)
2. [Search](#search)
3. [Evaluation](#evaluation)
4. [Move Generation](#move-generation)

## Board Representation
The engine uses a piece centric board representation, I.e. storing where pieces are, as opposed to what is on squares. This is done through multiple sets of [bitboards](https://en.wikipedia.org/wiki/Bitboard). Where each bitboard stores a binary value for each square, e.g. if pawn on a1, then bitboard equal ...1.

It stores a bitboard for each type of piece, which each represent whether that type is on any given square. Combined with another two bitboards — one for each color — allows the intersection thereof, to give what pieces are on the board of a specific type and color. As an example, taking the intersection of the pawn bitboard and white bitboard, gives the position of all white pawns. 

These are what constitutes all information for a given board state, but a couple of auxiliary bitboards are added. Namely, there is also stored information about the game state. That could be whether En Passant or castling is legal.

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/src/headers/board.hh).

## Search
In order to find the best move for a given position, a modified version of the [Minimax](https://en.wikipedia.org/wiki/Minimax) algorithm is used called [Negamax](https://en.wikipedia.org/wiki/Negamax).

The Minimax algorithm in essence works by exhaustively checking all possible permutations of moves, evaluating the outcome, and choosing based on that evaluation.
An important note to this is that it always assumes perfect play, as such, it assumes the opponent chooses the best move.

This allows an optimisation called [Alpha-Beta Pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning).
Which disregards moves which can only lead to a suboptimal board evaluation.

However, this still falls short. As [Minimax](https://en.wikipedia.org/wiki/Minimax) searches a fixed depth - the amount of moves ahead - it can fall short of sudden changes.
This phenomenon is called the [Horizon Effect](https://en.wikipedia.org/wiki/Horizon_effect), which is counteracted in this implementation through [Quiescence search](https://en.wikipedia.org/wiki/Quiescence_search).
Which, given the search having reached its terminal node, further evaluates all captures. This greatly improves search stability, where an increase in depth, more rarely changes the best move.

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/src/headers/minimax.hh).

## Evaluation
The evaluation method used is rather simple. It is the combination of two things: piece values(PV), and piece position values(PPV). As can be seen the evaluation is calculated for the white side, but can simply be negated to get blacks evlaution.

$$Eval = PV + PPV$$

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/src/headers/evaluator.hh).

### Piece Values
Let $P = \set{(w_{count}, b_{count}, val)}$ where each element of the set P denotes the number and value of a given piece. Where $w_{count}$ and $b_{count}$ are dependent on the given board, representing the number of white and black pieces respectively of the given type. And the $value$ is the static value of the piece type.

$$PV = \sum_{p \in P} (w_{count} - b_{count}) * p_{val}$$

### Piece Position Values
Let $P_{white} = \set{(type, square)}$ and $P_{black} = \set{(type, square)}$, where each is the set of pieces and their positions for white and black respectively. Then let the $f(color, type, square)$ return the value of a piece for a given square.

$$PPV = \sum_{p \in P_{white}} f(white, p_{type}, p_{square}) - \sum_{p \in P_{black}} f(black, p_{type}, p_{square})$$

### Tuning
In order to find the optimal values for both $PV$ and $PPV$(Only $PV$ tuning is implemented) the [Stockfish tuning algorithm](https://www.chessprogramming.org/Stockfish%27s_Tuning_Method) is used.

This method simply adjusts a set of parameters(some subset of $PV$ and $PPV$) in both the positive and negative direction. Then lets two engines play against eachother, one with the parameters adjusted in the positive direction and the other in the negative.

The matches played are from a set of board position which are apx. equal in evaluation. These positions are each played twice, with both engines playing one match as white and one as black.

If, after these matches, one of the engines have won more than half of the games played. The parameters adjusted are permenantly moved slightly towards the adjusted value. 

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/builds/tune.cpp).

## Move Generation
Only legal moves are generated for a given board state. Which means that applying the move does not endanger the king.

To give an idea of moves are generated, the generation of sliding piece moves is shown.

See implementation [here](https://github.com/jamadaha/chess_engine/tree/main/src/move_gen).

### Sliding Piece Algorithm
The essence of this algorithm is an expanding ring from the position of a given piece, which overlaps with possible pieces to attack. Upon overlapping with any piece, another bitboard storing unblocked squares is updated to remove the given squares.

This can be seen in the below gif(Do note: Very scuffed):
![move_gen](https://user-images.githubusercontent.com/53914190/216725341-04c2a49b-2a96-442d-97c2-b61b04cfdc02.gif)

The gif shows five bitboards: pieces, what pieces are on the board; ring, the ring with an increasing offset; attackboard, static moves possible for the given piece on an empty board; unblocked, possible squares(starting at all but initial); and combined, which shows where all the bitboards overlap.

The advantage of this algorithm is that it can be used regardless of what sliding piece(bishop, rook, or queen. Although, it could be used for knight, king, and some pawn moves. However, the efficiency would be quite low) moves are being generated for.

See implementation [here](https://github.com/jamadaha/chess_engine/blob/main/src/move_gen/piece_gen.cpp).
