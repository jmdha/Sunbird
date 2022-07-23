#include "headers/move_tree_node.hh"

MoveTreeNode::MoveTreeNode(int materialScore, int positionScore) : materialScore(materialScore), positionScore(positionScore) {
    score = materialScore + positionScore;
}

void MoveTreeNode::AppendMoveTreeNode(std::string move, MoveTreeNode node) {
    children.emplace(move, node);
}
