#ifndef MOVETREEGEN
#define MOVETREEGEN

#include <stdio.h>
#include <string>

#include "constants.hh"
#include "bit_board.hh"
#include "evaluator.hh"
#include "move_gen.hh"
#include "move.hh"
#include "move_tree_node.hh"

class MoveTreeGenerator {
public:
    MoveTreeGenerator(BitBoard* board, std::string outputPath, int HEDepth);
    ~MoveTreeGenerator();

    void GenerateTreeToFile(int depth, int outputDepth);

private:
    BitBoard* board;
    FILE *fp;
    std::string outputPath;
    MoveGen* moveGens[2];
    Evaluator* evaluators[2];
    int HEDepth;

    void PrintNode(std::string move, MoveTreeNode node, int indent, bool lastNode);
    MoveTreeNode GenerateMoveTree(int depth, int outputDepth);
    MoveTreeNode NegaMax(int depth, bool doingHE, int outputDepth, U64 attacks);

    void PrintIndent(int indent, std::string string);
    void PrintIndentNL(int indent, std::string string);
};
#endif
