#include "decision_tree_classifier.h"

#include <iostream>
#include <vector>


// DT Classifier definitions

std::vector<int> createVectorFromIToJ(int i, int j) {
    std::vector<int> vec;
    for (int k = i; k <= (j-1); ++k) {
        vec.push_back(k);
    }
    return vec;
}

// default DT classifier
DecisionTreeClassifier::DecisionTreeClassifier() {
    max_depth_ = 6;
    min_samples_split_ = 2;
    min_samples_leaf_ = 1;
    max_features_ = 10000;
    min_impurity_split_ = 0.1;
}


DecisionTreeClassifier::DecisionTreeClassifier(
  int max_depth,
  int min_samples_split,
  int min_samples_leaf, 
  int max_features,
  double min_impurity_split ) {
    max_depth_ = max_depth;
    min_samples_split_ = min_samples_split;
    min_samples_leaf_ = min_samples_leaf;
    max_features_ = max_features;
    min_impurity_split_ = min_impurity_split;  
}


void DecisionTreeClassifier::print() { 
  std::cout << "\nDT Classifier" << std::endl;
  std::cout << "Max Depth: " << max_depth_ << std::endl;
  std::cout << "Decision Tree:" << std::endl;
  tree_.print_tree(); 
}



void DecisionTreeClassifier::BuildTree(
              const std::vector<std::vector<double>> feature_data,
              const std::vector<int> label_data) {

    label_data_ = label_data;
    feature_data_ = feature_data;

    n_samples_ = label_data_.size();
    n_features_ = feature_data.size();

    // assert
    // length of one row of feature_data must equal n_samples_

    // assert
    // labels must be 0s and 1s

    // check
    // that labels have both 0s and 1s

    std::cout << "label length: " << n_samples_ << std::endl;
    std::cout << "features rows: " << n_features_ << std::endl;
    std::cout << "features cols: " << feature_data_[0].size() << std::endl;

    // start with root node
    Record curr = Record();
    curr.n_samples=n_samples_;    // number of samples in the set
    curr.index=createVectorFromIToJ(0, n_samples_);   // index in to data and labels
    curr.entropy=0.0;       // entropy of this node
    int variable1=0;        // index into feature rows
    int variable2=0;        // used for pair comparison v1 > v2
    double prob_v1gtv2=0.0;  // of v1 > v2 / n_samples

    //  push current to stack
    stack_.push(curr);

    // create root node
    Node this_node = Node();
    this_node.depth=0;          // depth in the tree
    this_node.parent_id=0;    // index into array of nodes
    this_node.left_child_id=-1; // index to left child
    this_node.right_child_id=-1; // index to right child
    this_node.is_left=false;    // is this node a left child
    this_node.is_leaf=false;    // is this node a leaf
    this_node.record_ = curr;   // data in the node

    //    push root node
    tree_.add_node(this_node);  

    // for pop stack
    while (!stack_.is_empty()) {
        stack_.pop(curr);
        std::cout << "popped: " << curr.n_samples << std::endl;

        //    if current criteria says to split
        //      split node
  
    }
    //

    //          L and R pushed to stack
    //
    //    else set as leaf.
}