#include "decision_tree.h"

#include <iostream>
#include <vector>



// Nodestack definitions
// a stack of records
bool RecordStack::is_empty() { return stack.size() == 0; }
void RecordStack::push(Record record) { stack.push_back(record); }
void RecordStack::pop(Record &record) {
  record = stack.back();
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


