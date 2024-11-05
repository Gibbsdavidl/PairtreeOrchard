#include "decision_tree.h"

#include <iostream>
#include <vector>


// Nodestack definitions

bool NodeStack::IsEmpty() { return stack.size() == 0; }

void NodeStack::push(Record record) { stack.push_back(record); }

void NodeStack::pop(Record &record) {
  record = stack.back();
  stack.pop_back();
}


// Node definitions


void Node::Print() {
  std::cout << "parent id: " << parent_id << std::endl;
  std::cout << "criterion: " << impurity << std::endl;
  std::cout << "n_samples: " << n_samples << std::endl;
  std::cout << "is leaf: " << is_leaf << std::endl;
  std::cout << "is left: " << is_left << std::endl;
  if (!is_leaf) {
    // std::cout << "feature: " << split_feature << std::endl;
    // std::cout << "threshold: " << threshold << std::endl;
    std::cout << "left child id: " << left_child_id << std::endl;
    std::cout << "right child id: " << right_child_id << std::endl;
  }
  std::cout << "value: [ ";
  for (std::size_t i = 0; i < value.size(); i++) {
    std::cout << value[i] << " ";
  }
  std::cout << "]" << std::endl;
}




// Decision Tree definitions

int DecisionTree::AddNodeToDT(Node node) {
  int node_id = nodes.size();
  nodes.emplace_back(node);
  return node_id;
}

// iterate through nodes and call print on each
void DecisionTree::Print() {
  if (!is_built) {
    throw std::logic_error(
        "Decision tree must be built before it can be printed.");
  }
  for (std::size_t i = 0; i < nodes.size(); i++) {
    std::cout << "=============================" << std::endl;
    std::cout << "node id: " << i << std::endl;
    nodes[i].Print();
  }
  std::cout << "=============================" << std::endl;
}




// DT Classifier definitions

DecisionTreeClassifier::DecisionTreeClassifier() {
	//ImpurityMeasure impurity_measure = gini,
    max_depth_ = 6;
    min_samples_split_ = 2;
    min_samples_leaf_ = 1;
    max_features_ = 2;
    //MaxFeaturesMethod = NULL;
    min_impurity_split_ = 0.0;
}

// DecisionTreeClassifier::DecisionTreeClassifier(
//     //ImpurityMeasure impurity_measure, 
//     int max_depth, 
//     int min_samples_split,
//     int min_samples_leaf, 
//     int max_features,
//     MaxFeaturesMethod max_features_method, 
//     double min_impurity_split) {
// 	  //impurity_measure_ = impurity_measure;
// 	  max_depth_ = max_depth;
// 	  min_samples_split_ = min_samples_split;
// 	  min_samples_leaf_ = min_samples_leaf;
// 	  max_features_ = max_features;
// 	  max_features_method_ = max_features_method;
// 	  min_impurity_split_ = min_impurity_split;
// }

void DecisionTreeClassifier::PrintTree() { tree_.Print(); }

void DecisionTreeClassifier::AddNode() {

	// Define a distribution
    std::uniform_int_distribution<int> distribution(1, 100);

    // Define a random node
	Node current_node;
	current_node.parent_id = distribution(generator);
    //current_node.impurity = current_record.impurity;
    current_node.is_leaf = 0;
    current_node.is_left = 0;
    current_node.n_samples = distribution(generator);
    //current_node.split_feature = current_split.feature;
    current_node.variable1 = distribution(generator);
    current_node.variable2 = distribution(generator);

    tree_.AddNodeToDT(current_node);
}
