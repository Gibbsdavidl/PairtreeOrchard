#include "decision_tree.h"

#include <iostream>
#include <vector>



// RecordStack definitions
// a stack of records
bool RecordStack::isEmpty() { return stack_.size() == 0; }
void RecordStack::push(Record record) { stack_.push_back(record); }
void RecordStack::pop(Record &record) {
  if (stack_.empty()) throw std::runtime_error("Empty record stack");
  record = std::move(stack_.back());
  stack_.pop_back();
}


// Nodestack definitions
// a stack of nodes
bool NodeStack::isEmpty() { return stack_.size() == 0; }
void NodeStack::push(Node node) { stack_.push_back(node); }
void NodeStack::pop(Node &node) {
  if (stack_.empty()) throw std::runtime_error("Empty node stack");
  node = std::move(stack_.back());
  stack_.pop_back();
}


// Node definitions
void Node::printNode() {
  std::cout << "~Node~\n\n";
  std::cout << "parent id: " << parent_id_ << std::endl;
  std::cout << "is leaf: " << is_leaf_ << std::endl;
  std::cout << "is left: " << is_left_ << std::endl;
  if (!is_leaf_) {
    std::cout << "left child id: " << left_child_id_ << std::endl;
    std::cout << "right child id: " << right_child_id_ << std::endl;
  }
  std::cout << "sample index: [ ";
  for (std::size_t i = 0; i < record_.index_.size(); i++) {
    std::cout << record_.index_[i] << " ";
  }
  std::cout << "]" << std::endl;
}

// Decision Tree definitions
void DecisionTree::createNode(Node& this_node, Record& record, 
                  std::string mode, int node_idx) {

  // create root node
  if (mode == "root") {
    this_node.depth_=0;          // depth in the tree
    this_node.self_id_=node_idx;        // index into array of nodes
    this_node.parent_id_=0;    // index into array of nodes
    this_node.left_child_id_=-1; // index to left child
    this_node.right_child_id_=-1; // index to right child
    this_node.is_left_=false;    // is this node a left child
    this_node.is_leaf_=false;    // is this node a leaf
    this_node.record_ = record;   // data in the node  
  } else if (mode == "leaf") {
    this_node.depth_=0;          // depth in the tree
    this_node.self_id_=node_idx;        // index into array of nodes
    this_node.parent_id_=0;    // index into array of nodes
    this_node.left_child_id_=-1; // index to left child
    this_node.right_child_id_=-1; // index to right child
    this_node.is_left_=false;    // is this node a left child
    this_node.is_leaf_=false;    // is this node a leaf
    this_node.record_ = record;   // data in the node  
  }

}


void DecisionTree::createNodes(Node& parent, Node& left, Node& right,
  Record l_rec, Record r_rec) {
    // create nodes!
    // Set up the left child node
    left.depth_ = parent.depth_ + 1;
    left.parent_id_ = parent.self_id_;
    left.is_left_ = true;
    left.record_ = l_rec;
    left.left_child_id_ = -1;  // Initialize child IDs before adding
    left.right_child_id_ = -1;
    // Determine if the child is a leaf node
    left.is_leaf_ = (l_rec.index_.size() == 0 || l_rec.entropy_ == 0.0);
    // Add child to the tree and get its ID
    left.self_id_ = addNode(left);

    // Set up the right child node
    right.depth_ = parent.depth_ + 1;
    right.parent_id_ = parent.self_id_;
    right.is_left_ = false;
    right.record_ = r_rec;
    right.left_child_id_ = -1;  // Initialize child IDs before adding
    right.right_child_id_ = -1;
    // Determine if the child is a leaf node
    right.is_leaf_ = (r_rec.index_.size() == 0 || r_rec.entropy_ == 0.0);
    // Add child to the tree and get its ID
    right.self_id_ = addNode(right);

    // Link the parent node to its new children in the nodes_ vector
    nodes_[parent.self_id_].left_child_id_ = left.self_id_;
    nodes_[parent.self_id_].right_child_id_ = right.self_id_;
    // The parent is no longer a leaf
    nodes_[parent.self_id_].is_leaf_ = false;
  }


// add a node to the DT
int DecisionTree::addNode(Node node) {
  int node_id = nodes_.size();
  nodes_.emplace_back(node);
  if (!is_built_) {
    is_built_ = true;
  }
  return node_id;
}


// iterate through nodes and call print on each
void DecisionTree::printTree() {
  if (!is_built_) {
    throw std::logic_error(
        "Decision tree must be built before it can be printed.");
  }
  for (std::size_t i = 0; i < nodes_.size(); i++) {
    std::cout << "=============================" << std::endl;
    std::cout << "node id: " << i << std::endl;
    nodes_[i].printNode();
  }
  std::cout << "=============================" << std::endl;
}

