#ifndef MOVETREENODE
#define MOVETREENODE

#include <string>
#include <map>

#include "move.hh"

class MoveTreeNode {
public:
    int score;
    std::map<std::string, MoveTreeNode> children;
    MoveTreeNode(int score);

    void AppendMoveTreeNode(std::string move, MoveTreeNode node);

private:
    
};
#endif
