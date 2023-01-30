#ifndef MOVE_TREE_NODE
#define MOVE_TREE_NODE

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
#endif // MOVE_TREE_NODE
