# Tests
## EvaluatePieceCount
add_executable(EvaluatePieceCount ${CMAKE_CURRENT_LIST_DIR}/evaluate_piece_count.cpp)
target_link_libraries(EvaluatePieceCount PRIVATE ENGINE)

add_test(Evaluator_Bit_Piece_Count_StartPos EvaluatePieceCount 0 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
add_test(Evaluator_Bit_Piece_Count_Rook EvaluatePieceCount 692 "8/8/8/8/8/8/8/R7 w KQkq - 0 1")
add_test(Evaluator_Bit_Piece_Count_QueenPawn EvaluatePieceCount -1090 "8/6P1/8/8/8/8/7q/8 w KQkq - 0 1")

## EvaluatePositionValue
add_executable(EvaluatePositionValue ${CMAKE_CURRENT_LIST_DIR}/evaluate_position_value.cpp)
target_link_libraries(EvaluatePositionValue PRIVATE ENGINE)

add_test(Evaluator_Position_Value_Pawn_White EvaluatePositionValue 10 "8/8/8/8/8/8/P6P/8 w KQkq - 0 1")
add_test(Evaluator_Position_Value_Pawn_Black EvaluatePositionValue -10 "8/p6p/8/8/8/8/8/8 w KQkq - 0 1")

