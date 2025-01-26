#include "decision_tree_classifier.h"

#include <iostream>
#include <vector>


// DT Classifier definitions

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
  std::cout << "DT Classifier" << std::endl;
  std::cout << "Max Depth: " << max_depth_ << std::endl;
  std::cout << "D Tree" << std::endl;
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

}