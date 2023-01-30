#include "move_tree_gen.hh"

MoveTreeGenerator::MoveTreeGenerator(Board* board, std::string outputPath, bool useAB) : 
board(board), 
outputPath(outputPath), 
useAB(useAB), 
evaluator(Evaluator(board->GetColor())),
moveGens{MoveGen(Color::White), MoveGen(Color::Black)} {
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
    int alpha = -(int) PieceValue::Inf;
    int beta = (int) PieceValue::Inf;
    MoveTreeNode node = NegaMax(depth, useAB, depth - outputDepth, alpha, beta);
    return node;
}

MoveTreeNode MoveTreeGenerator::NegaMax(int depth, bool useAB, int outputDepth, int alpha, int beta) {
    if (depth == 0)
        return MoveTreeNode(evaluator.EvaluatePieceCount(*board), evaluator.EvaluatePositionValue(*board));
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);
    
    MoveTreeNode node = MoveTreeNode(-(int)PieceValue::Inf, depth);

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);

        MoveTreeNode childNode = NegaMax(depth - 1, useAB, outputDepth, -beta, -alpha);
        childNode.score *= -1;
        childNode.materialScore *= -1;
        childNode.positionScore *= -1;
        board->UndoMove(moves[i]);
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
    return node;
}

void MoveTreeGenerator::PrintIndent(int indent, std::string string) {
    fprintf(fp, "%*s%s", indent, "", string.c_str());
}

void MoveTreeGenerator::PrintIndentNL(int indent, std::string string) {
    PrintIndent(indent, string);
    fprintf(fp, "\n");
}