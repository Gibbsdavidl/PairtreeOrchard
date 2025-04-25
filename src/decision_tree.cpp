#include "decision_tree.h"

#include <iostream>
#include <vector>



// RecordStack definitions
// a stack of records
bool RecordStack::is_empty() { return stack.size() == 0; }
void RecordStack::push(Record record) { stack.push_back(record); }
void RecordStack::pop(Record &record) {
  record = stack.back();
  stack.pop_back();
}


// Nodestack definitions
// a stack of nodes
bool NodeStack::is_empty() { return stack.size() == 0; }
void NodeStack::push(Node node) { stack.push_back(node); }
void NodeStack::pop(Node &node) {
  node = stack.back();
  stack.pop_back();
}


// Node definitions
void Node::print_node() {
  std::cout << "~Node~\n\n";
  std::cout << "parent id: " << parent_id << std::endl;
  std::cout << "is leaf: " << is_leaf << std::endl;
  std::cout << "is left: " << is_left << std::endl;
  if (!is_leaf) {
    std::cout << "left child id: " << left_child_id << std::endl;
    std::cout << "right child id: " << right_child_id << std::endl;
  }
  std::cout << "sample index: [ ";
  for (std::size_t i = 0; i < record_.index.size(); i++) {
    std::cout << record_.index[i] << " ";
  }
  std::cout << "]" << std::endl;
}

// Decision Tree definitions
void DecisionTree::create_node(Node& this_node, Record& record, 
                  std::string mode, int node_idx) {

  // create root node
  if (mode == "root") {
    this_node.depth=0;          // depth in the tree
    this_node.self_id=node_idx;        // index into array of nodes
    this_node.parent_id=0;    // index into array of nodes
    this_node.left_child_id=-1; // index to left child
    this_node.right_child_id=-1; // index to right child
    this_node.is_left=false;    // is this node a left child
    this_node.is_leaf=false;    // is this node a leaf
    this_node.record_ = record;   // data in the node  
  } else if (mode == "leaf") {
    this_node.depth=0;          // depth in the tree
    this_node.self_id=node_idx;        // index into array of nodes
    this_node.parent_id=0;    // index into array of nodes
    this_node.left_child_id=-1; // index to left child
    this_node.right_child_id=-1; // index to right child
    this_node.is_left=false;    // is this node a left child
    this_node.is_leaf=false;    // is this node a leaf
    this_node.record_ = record;   // data in the node  
  }

}


void DecisionTree::create_nodes(Node& parent, Node& left, Node& right,
  Record l_rec, Record r_rec) {
    
    // create nodes!
    
  }


// add a node to the DT
int DecisionTree::add_node(Node node) {
  int node_id = nodes.size();
  nodes.emplace_back(node);
  if (!is_built) {
    is_built = true;
  }
  return node_id;
}


// iterate through nodes and call print on each
void DecisionTree::print_tree() {
  if (!is_built) {
    throw std::logic_error(
        "Decision tree must be built before it can be printed.");
  }
  for (std::size_t i = 0; i < nodes.size(); i++) {
    std::cout << "=============================" << std::endl;
    std::cout << "node id: " << i << std::endl;
    nodes[i].print_node();
  }
  std::cout << "=============================" << std::endl;
}


