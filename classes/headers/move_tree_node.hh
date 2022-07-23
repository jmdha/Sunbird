#ifndef MOVETREENODE
#define MOVETREENODE

#include <string>
#include <map>

#include "move.hh"

class MoveTreeNode {
public:
    int score;
    int materialScore;
    int positionScore;
    std::map<std::string, MoveTreeNode> children;
    MoveTreeNode(int materialScore, int positionScore);

    void AppendMoveTreeNode(std::string move, MoveTreeNode node);

private:
    
};
#endif
