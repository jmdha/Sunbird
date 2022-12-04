#ifndef MOVE_TREE_GEN
#define MOVE_TREE_GEN

#include <stdio.h>
#include <string>

#include "constants.hh"
#include "board.hh"
#include "evaluator.hh"
#include "move_gen.hh"
#include "move.hh"
#include "move_tree_node.hh"

class MoveTreeGenerator {
public:
    MoveTreeGenerator(Board* board, std::string outputPath, bool useAB);
    ~MoveTreeGenerator();

    void GenerateTreeToFile(int depth, int outputDepth);

private:
    Board* board;
    FILE *fp;
    std::string outputPath;
    MoveGen* moveGens[2];
    Evaluator evaluator;
    bool useAB;

    void PrintNode(std::string move, MoveTreeNode node, int indent, bool lastNode);
    MoveTreeNode GenerateMoveTree(int depth, int outputDepth);
    MoveTreeNode NegaMax(int depth, bool useAB, int outputDepth, int alpha, int beta, U64 attacks[2]);

    void PrintIndent(int indent, std::string string);
    void PrintIndentNL(int indent, std::string string);
};
#endif // MOVE_TREE_GEN
