# Tests
## EvaluatePieceCount
add_executable(EvaluatePieceCount tests/evaluator/evaluate_piece_count.cpp "src/board.cpp" "src/board_importer.cpp" "src/evaluator.cpp")

add_test(Evaluator_Bit_Piece_Count_StartPos EvaluatePieceCount 0 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
add_test(Evaluator_Bit_Piece_Count_Rook EvaluatePieceCount 634 "8/8/8/8/8/8/8/R7 w KQkq - 0 1")
add_test(Evaluator_Bit_Piece_Count_QueenPawn EvaluatePieceCount -1128 "8/6P1/8/8/8/8/7q/8 w KQkq - 0 1")

## EvaluatePositionValue
add_executable(EvaluatePositionValue tests/evaluator/evaluate_position_value.cpp "src/board.cpp" "src/board_importer.cpp" "src/evaluator.cpp")

add_test(Evaluator_Position_Value_Pawn_White EvaluatePositionValue 10 "8/8/8/8/8/8/P6P/8 w KQkq - 0 1")
add_test(Evaluator_Position_Value_Pawn_Black EvaluatePositionValue -10 "8/p6p/8/8/8/8/8/8 w KQkq - 0 1")
