#include "headers/move_tree_node.hh"

MoveTreeNode::MoveTreeNode(int score) : score(score) {}

void MoveTreeNode::AppendMoveTreeNode(std::string move, MoveTreeNode node) {
    children.emplace(move, node);
}
