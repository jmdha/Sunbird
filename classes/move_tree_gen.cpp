#include "headers/move_tree_gen.hh"

MoveTreeGenerator::MoveTreeGenerator(Board* board, std::string outputPath, bool useAB) : board(board), outputPath(outputPath), useAB(useAB) {
    evaluators[(int) Color::White] = new Evaluator(Color::White);
    evaluators[(int) Color::Black] = new Evaluator(Color::Black);
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
    fp = fopen64(outputPath.c_str(), "w");   
}

MoveTreeGenerator::~MoveTreeGenerator() {
    fclose(fp);
}

void MoveTreeGenerator::GenerateTreeToFile(int depth, int outputDepth) {
    MoveTreeNode moveTree = GenerateMoveTree(depth, outputDepth);
    PrintNode("", moveTree, 0, true);
}

void MoveTreeGenerator::PrintNode(std::string move, MoveTreeNode node, int indent, bool lastNode) {
    int newIndent = indent + 2;
    PrintIndentNL(indent, "{");
    PrintIndentNL(newIndent, "\"move\": \"" + move + "\",");
    PrintIndentNL(newIndent, "\"score\": \"" + std::to_string(node.score) + "\",");
    PrintIndentNL(newIndent, "\"materialScore\": \"" + std::to_string(node.materialScore) + "\",");
    PrintIndentNL(newIndent, "\"positionScore\": \"" + std::to_string(node.positionScore) + "\",");

    if (node.children.size() == 0)
        PrintIndentNL(newIndent, "\"moves\": []");
    else {
        PrintIndentNL(newIndent, "\"moves\": [");
        int index = 0;
        for (std::map<std::string, MoveTreeNode>::iterator it = node.children.begin(); it !=node.children.end(); ++it)
            PrintNode(it->first, it->second, newIndent + 2, ++index == node.children.size());
            
        PrintIndentNL(newIndent, "]");
    }
    if (lastNode)
        PrintIndentNL(indent, "}");
    else
        PrintIndentNL(indent, "},");
}

MoveTreeNode MoveTreeGenerator::GenerateMoveTree(int depth, int outputDepth) {
    U64 attacks[2] = { 0, 0 };
    int alpha = -(int) PieceValue::Inf;
    int beta = (int) PieceValue::Inf;
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attacks);
    free(moves);
    MoveTreeNode node = NegaMax(depth, useAB, depth - outputDepth, alpha, beta, attacks);
    return node;
}

MoveTreeNode MoveTreeGenerator::NegaMax(int depth, bool useAB, int outputDepth, int alpha, int beta, U64 attacks[2]) {
    if (depth == 0)
        return MoveTreeNode(evaluators[(int) board->GetOriginalColor()]->EvaluatePieceCount(*board), evaluators[(int) board->GetOriginalColor()]->EvaluatePositionValue(*board));
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attackSquares[2] = { attacks[0], attacks[1] };
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);
    
    MoveTreeNode node = MoveTreeNode(-(int)PieceValue::Inf, depth);

    for (int i = 0; i < moveCount; i++) {
        PieceType capturedPiece = board->DoMove(moves[i]);

        MoveTreeNode childNode = NegaMax(depth - 1, useAB, outputDepth, -beta, -alpha, attackSquares);
        childNode.score *= -1;
        childNode.materialScore *= -1;
        childNode.positionScore *= -1;
        board->UndoMove(moves[i], capturedPiece);
        if (depth >= outputDepth)
            node.AppendMoveTreeNode(moves[i].ToString(), childNode);

        if (childNode.score > node.score) {
            node.score = childNode.score;
            node.materialScore = childNode.materialScore;
            node.positionScore = childNode.positionScore;
        }

        if (useAB) {
            if (alpha >= beta)
                return node;
            if (childNode.score > alpha)
                alpha = childNode.score;
        }
    }
    free(moves);
    return node;
}

void MoveTreeGenerator::PrintIndent(int indent, std::string string) {
    fprintf(fp, "%*s%s", indent, "", string.c_str());
}

void MoveTreeGenerator::PrintIndentNL(int indent, std::string string) {
    PrintIndent(indent, string);
    fprintf(fp, "\n");
}
